# @Time    : 2020/9/15 15:00
# @Author  : zhangpeng
# @Email   : 1970187812@qq.com
# @Content : 使用 fbprophet 库对低纬度地区的缺失月份 2018年7月至12月的数据进行估计


from matplotlib import pyplot as plt
from fbprophet import Prophet

import math


import gdal
import glob
import os
import pandas as pd

import numpy as np
import matplotlib.pylab as plt

from multiprocessing import Pool
import time

# 全局变量 nodata
NoData = 0
ImProj = 0
ImGeotrans = 0

def eye_stationarity(timeseries):
    #这里以一年为一个窗口，每一个时间t的值由它前面的12个月的均值替代，标准差同理

    rolmean = pd.Series.rolling(timeseries, window=12,center=False).mean()
    rolstd  = pd.Series.rolling(timeseries, window=12).std()

    #plot
    orig = plt.plot(timeseries, color='blue',label='Original')
    mean = plt.plot(rolmean, color='red', label='Rolling Mean')
    std = plt.plot(rolstd, color='black', label = 'Rolling Std')
    plt.legend(loc='best')
    plt.title('Rolling Mean & Standard Deviation')
    plt.show(block=False)

# 平稳转换，消除趋势和季节性  一阶差分
def transform_stationary(ts, degree):
    '''

    :param ts:
    :param degree: 差分的阶数 1 代表进行1阶差分 2代表进行二阶差分
    :return:
    '''
    # 利用log 降低异方差性
    ts_log = ts #np.log(ts)

    # 对于高季节性的时间序列来说，需要先采用简单的低阶差分将信息提取充分，通过周期步长差分将季节信息提取，剩下的残差序列就是一个平稳序列
    # 1阶差分  周期步长为 12

    # rs_diff0 = (ts_log - ts_log.shift(periods=12)) - (ts_log.shift() - ts_log.shift().shift(periods=12))
    # print("rs_diff0 = \n", rs_diff0)
    ts_log_diff = ts_log.diff()#  = ts_log - ts_log.shift()

    while degree > 1:
        ts_log_diff = ts_log_diff.diff()
        degree = degree - 1
    # print("ts_log_diff = \n", ts_log_diff)

    rs_log_diff = ts_log_diff - ts_log_diff.shift(periods=12)
    # print("rs_log_diff = \n", rs_log_diff)
    rs_log_diff.dropna(inplace=True)

    return  ts_log_diff, rs_log_diff

# 读取TIFF文件
def read_geotiff(filename):
    global NoData
    global ImProj
    global ImGeotrans
    ds = gdal.Open(filename)
    width = ds.RasterXSize  # 数据宽度
    height = ds.RasterYSize  # 影像高度
    band = ds.RasterCount  # 波段数目
    im_geotrans = ds.GetGeoTransform()  # 获得仿射矩阵信息
    im_proj = ds.GetProjection()  # 获取投影信息
    ImProj = im_proj
    ImGeotrans = im_geotrans
    # 读取波段信息
    in_band = ds.GetRasterBand(1)
    NoData = in_band.GetNoDataValue()
    im_data = in_band.ReadAsArray(0, 0, width, height)
    del ds

    return im_proj, im_geotrans, im_data

# 写入TIFF文件
def write_geotiff(filename, im_proj, im_geotrans, im_data, nodata):
    datatype = gdal.GDT_Float32

    im_band, (im_height, im_width) = 1, im_data.shape  # row， column
    # 创建文件
    driver = gdal.GetDriverByName("GTiff")
    ds = driver.Create(filename, im_width, im_height, im_band, datatype)

    ds.SetGeoTransform(im_geotrans) # 写入放射变换参数
    ds.SetProjection(im_proj)       # 写入投影
    ds.GetRasterBand(1).SetNoDataValue(nodata) # 设置nodata值
    ds.GetRasterBand(1).WriteArray(im_data)
    del ds

# 从文件夹中加载数据
def load_data(in_path):

    arr_data = []

    # in_path = r"F:\04-dissertation\02-VIIRS\viirs_data\city_world_raster\Shanghai"
    rasters = glob.glob(in_path + "\\*.tif")
    rasters = rasters[0:75]
    dtindex = {"ds": []}

    for raster in rasters:

        dtindex['ds'].append(str(os.path.basename(raster)[0:6]))

        im_proj, im_geotrans, im_data = read_geotiff(raster)
        arr_data.append(im_data)
        # print("type(im_data) = ", type(im_data))
        # print("im_data.shape = ", im_data.shape)
    image_row, image_col = arr_data[0].shape

    print(dtindex)

    time_series = []
    first = True
    for ras in arr_data:

        index = 0
        # 遍历一个月份的VIIRS
        for row in ras:
            for cell in row:
                if first:
                    ts = {
                        "y":[],
                    }
                    # if cell < 0:
                    #     cell = 0.001
                    ts['y'].append(cell)
                    time_series.append(ts)
                else:
                    # if cell < 0:
                    #     cell = 0.001
                    time_series[index]['y'].append(cell)
                index = index + 1

        first = False

    # print("type(time_series) = ", type(time_series))
    return dtindex, time_series, image_row, image_col

def to_series(value, dtindex):
    """

    :param value:
    :param dtindex:
    :return:
    """
    value.update(dtindex)
    df = pd.DataFrame(data=value)
    # df = df.set_index(['ds'])
    df['ds'] = pd.to_datetime(df['ds'], format='%Y%m')

    # ts = df['DN']
    return df

def my_arima(time_series, dtindex, pid):
    month_7 = []
    month_8 = []
    month_9 = []
    month_10 = []
    month_11 = []
    month_12  = []
    lower = []
    upper = []
    index  = 0
    for item in time_series:

        # 1 加载数据
        ts = to_series(item, dtindex)
        value = ts.head(1).values

        if value[0][0] == NoData:
            month_7.append(NoData)
            month_8.append(NoData)
            month_9.append(NoData)
            month_10.append(NoData)
            month_11.append(NoData)
            month_12.append(NoData)

            print("pid = ", pid, " index = ", index, " length = ", len(time_series))
            index = index + 1
            continue

        # fbprophet
        # ts['y'] = np.log(ts['y'])
        m = Prophet( interval_width=0.95)
        m.fit(ts)
        future = m.make_future_dataframe(periods=12, freq='MS')
        forecast = m.predict(future)
        # print("forecast['yhat'] = \n", forecast['yhat'])
        # # 2 检测数据的平稳性, 如果平稳可直接进行预测，否则需先将不平稳的数据变得稳定
        # #  并返回需要差分的阶数 d
        # d = get_diff_order(ts)
        #
        # # 3 如果数据已经稳定，可直接进行处理；如果不稳定，处理时序数据变成稳定的数据,  返回值依次为原数据的取对数，一阶差分，周期步长差分
        # order, seasonal_order = get_sarima_params(ts, d)
        # # print("order = ", order)
        # # print("seasonal_order = ", seasonal_order)
        #
        # # 4 预测 模型定阶之后，开始进行预测。预测所需的数据并不是进行一阶差分、周期差分之后的数据，而是只进行了取对数之后的数据，因为SARIMAX方法可以自动进行差分并复原
        # fittedvalues, fc, conf, results_fitted = draw_future_plot(ts, order, seasonal_order, 6)
        month_7.append(forecast['yhat'][75])
        month_8.append(forecast['yhat'][76])
        month_9.append(forecast['yhat'][77])
        month_10.append(forecast['yhat'][78])
        month_11.append(forecast['yhat'][79])
        month_12.append(forecast['yhat'][80])

        # lower.append(forecast['yhat'][76])
        # upper.append(forecast['yhat'][77])
        print("pid = ", pid,  " index = ", index, " length = ", len(time_series))
        index  = index +1

    return month_7, month_8, month_9,month_10,month_11,month_12

# 将一个list尽量均分成n份，限制len(list)==n，份数大于原list内元素个数则分配空list[]
def divideIntoNstrand(listTemp, n):

    arr_len = math.ceil(len(listTemp) / n)

    for i in range(0, len(listTemp), arr_len):
        yield listTemp[i:i + arr_len]

def write_series(im_data, ts, filename, image_col):

    im_row = 0
    im_col = 0

    for item in ts:
        im_data[im_row, im_col] = item
        im_col = im_col + 1
        if image_col == im_col:
            im_col = 0
            im_row = im_row + 1
    out_path = r"city_world_raster\ts_result\\" + filename
    write_geotiff(out_path, ImProj, ImGeotrans, im_data, NoData)

if __name__ == '__main__':

    in_path = r"city_world_raster\Shanghai"

    dtindex, time_series, image_row, image_col = load_data(in_path)
    print("dtindex           = ", dtindex)
    print("type(time_series) = ", type(time_series))
    print("len(time_series)  = ", len(time_series))

    # 输出图像的数据
    im_data = np.array([[ NoData for i in range(image_col)] for j in range(image_row)])
    im_data2 = np.array([[ NoData for i in range(image_col)] for j in range(image_row)])
    im_data3 = np.array([[ NoData for i in range(image_col)] for j in range(image_row)])
    im_data4 = np.array([[ NoData for i in range(image_col)] for j in range(image_row)])
    im_data5 = np.array([[ NoData for i in range(image_col)] for j in range(image_row)])
    im_data6 = np.array([[ NoData for i in range(image_col)] for j in range(image_row)])
    print("im_data.shape = ", im_data.shape)
    im_row = 0
    im_col = 0

    # forecast_series = my_arima(time_series, dtindex, 0)
    # 多进程处理
    process_num = 16
    process_list = []
    pool = Pool(processes=process_num)

    start = time.time()  # 开始计时
    new_time_series = divideIntoNstrand(time_series, process_num)
    pid = 0
    for ts in new_time_series:
        p = pool.apply_async(my_arima, (ts,dtindex, pid))
        process_list.append(p)
        pid = pid + 1
    pool.close()
    pool.join()

    print(f'multi-process time cost: {time.time() - start}s', "  process_num = ", process_num)
    month_7_series = []
    month_8_series = []
    month_9_series = []
    month_10_series = []
    month_11_series = []
    month_12_series  = []
    for i in process_list:
        month_7, month_8, month_9,month_10,month_11,month_12 = i.get()
        month_7_series = month_7_series + month_7
        month_8_series = month_8_series + month_8
        month_9_series = month_9_series + month_9
        month_10_series = month_10_series + month_10
        month_11_series = month_11_series + month_11
        month_12_series = month_12_series + month_12


    write_series(im_data, month_7_series, "201807_Shanghai_95.tif", image_col)
    write_series(im_data2, month_8_series, "201808_Shanghai_95.tif", image_col)
    write_series(im_data3, month_9_series, "201809_Shanghai_95.tif", image_col)
    write_series(im_data4, month_10_series, "201810_Shanghai_95.tif", image_col)
    write_series(im_data5, month_11_series, "201811_Shanghai_95.tif", image_col)
    write_series(im_data6, month_12_series, "201812_Shanghai_95.tif", image_col)


    print("finish!!!")




