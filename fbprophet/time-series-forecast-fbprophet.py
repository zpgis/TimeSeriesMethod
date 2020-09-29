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




