/****************************************************************
 *
 * Project:  地理信息处理工具集
 * Purpose:  数据源的定义类
 * Author:   范俊甫, fanjf@lreis.ac.cn 2010年-12月-13日
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * 版权所有  (c) 保留所有权利
 ****************************************************************/
#pragma once
#ifndef GTDATASOURCE_H_INCLUDED
#define GTDATASOURCE_H_INCLUDED

#ifdef GDOS_DRAW_QUERY
#include <boost/function.hpp>
#endif
#include "gt_string.h"
#include "gt_layer.h"
#include "gt_feature.h"
#include "gt_spatialreference.h"

/*GDAL header files*/
#include "../include_p/gdal/ogr_core.h"
#include "../include_p/gdal/ogr_api.h"
#include "../include_p/gdal/ogrsf_frmts.h"
#include "../include_p/gdal/gdal_priv.h"

/*MySQL header files*/
#include "../include_p/mysql/my_global.h"
#include "../include_p/mysql/mysql.h"

/*BeyonDB header files*/

/*ORACLE oci header files*/
// #include "../include_p/oracle_11g/oci.h"
// #include "gt_oci.h"

/*ODBC header files*/

namespace gts {
class GTFields;
class GTFeatureLayer;
class GTTinLayer;
class GTRasterBand;
class GTS_DLL GTOGRDataDriver;
class GTS_DLL GTGDALDataDriver;
class GTS_DLL GTRasterDataset;
class GTS_DLL GTGDOSMySQLDataDriver;
class GTS_DLL GTGDOSMySQLDataSource;
class GTS_DLL GTGDOSOCIDataDriver;
class GTS_DLL GTGDOSOCIDataSource;

class GTS_DLL GTDataSource
{
public:
	GTDataSource(void);
	virtual ~GTDataSource(void);

	virtual enumGTDataDriverType getDataDriverType() const = 0;
	/*不允许外部释放*/
	virtual const char* getDataDriverName() const { return dataDriverName; }
	virtual const char* getDataSourceName() const { return dataSourceName; }

	/*打开一个数据源的连接*/
	virtual bool openDataSource(const char* pSourceName, bool bUpdate) = 0;

	/*根据数据源名称, 重新打开*/
	virtual bool reopenDataSource(bool bUpdate) = 0;

	/*关闭数据源连接*/
	virtual void closeDataSource() = 0;

	/*清除数据源连接*/
	virtual void clearDataSource() = 0;

	/*数据源连接是否活动*/
	virtual bool isDataSourceOpen() const = 0;

	/*数据源连接是否有效*/
	virtual bool isDataSourceValid() const = 0;

	void releaseReference();
	void setReference();
	int  getReference() const;

protected:
	int   refCount;

	/*存放数据源路径或数据库中schema*/
	char* dataSourceName;
	
	/*数据源类型*/
	char* dataDriverName;
};

/*
 *GTRasterDataset的接口类
 */
class GTS_DLL GTRasterDataset : public GTDataSource
{
public:
	friend class GTGDALDataDriver;
	friend class GTGDOSMySQLDataSource;

	GTRasterDataset(void);
	~GTRasterDataset(void);

	/*由GTDataSource继承*/
	/*但是需要注意的是, 此时以下函数直接针对GDAL数据源(文件型)进行操作*/
	enumGTDataDriverType getDataDriverType() const { return GT_DATADRIVER_GDAL; }
// 	const char* getDataDriverName() const;
// 	const char* getDataSourceName() const;
	bool openDataSource(
		const char* pSourceName, 
		bool        bUpdate);
	bool reopenDataSource(bool bUpdate);
	void closeDataSource();
	void clearDataSource();
	bool isDataSourceOpen() const { return isOpened; }
	bool isDataSourceValid() const;
	bool isUpdateMode() const { return isUpdated; }

	/*自有成员函数*/
	/*操作指向GTDataSource的指针*/
	void releaseDataSourceRefPtr();
	GTDataSource* getDataSourceRefPtr();

	/*是否有子集*/
	bool hasSubDatasets() const;

	/*获取子集数目*/
	int getSubDatasetsCount() const;

	/*获取是由哪个数据源驱动的*/
	enumGTDataDriverType getDataSourceDriverType() const;

	/*销毁一个GTRasterDataset指针*/
	static bool destroyGTRasterDataset(GTRasterDataset *pGtRasterDataset);
	
	/*获取GDALDataset指针, 外部不允许释放*/
	GDALDataset *getGDALDatasetRef();
	
	/*获取空间参考指针*/
	GTSpatialReference *getSpatialRefPtr();
	
	/*设置空间参考到真实数据源, 并更新内存空间参考, 
	 *如果空间参考错误, 或者数据源不可写或者不支持, 则返回错误
	 *即便更新成功, 真实数据也没有经过投影转换
	 */
	bool setDataSourceSpatialRef(GTSpatialReference *spatialRef);

	/*获取仿射变换参数, 该参数描述由行列到投影转换的参数
	 *padfTransform需要提前分配空间
	 */
	bool getGeomTransform(double *padfTransform);

	/*是否是旋转的影像*/
	bool isNorthUp();

	/*获取旋转的的弧度, 正北为0, 顺时针为负, 逆时针为正*/
	bool getRotateAngle(double &rtAngle);

	/*设置数据源的仿射转换参数*/
	bool setDataSourceGeomTransform(double *padfTransform);

	/*获取仿射逆变换的参数, 该参数描述由投影坐标到行列转换的参数
	 *6参数的padfInTransform需要提前分配空间
	 */
	bool getInverseGeomTransform(double *padfInTransform);

	/*获取GCP的个数*/
	int getGCPCount();

	/*获取GCP结构指针*/
	const GDAL_GCP* getGCPs();

	/*由GCPs转换成为仿射变换参数
	 *6参数的padfInTransform需要提前分配空间
	 */
	bool gcpsToGeotransform(
		int             gcpCount, 
		const GDAL_GCP *pasGCPs, 
		double         *padfInTransform);

	/***********************************/
	/*以下需要重写部分GDALDataset成员函数*/
	/***********************************/

	/*获取Metadata, 外部不能释放*/
	/*domainName为空, 则返回metadata目录*/
	/*domainName通常为: 
	 "IMAGE_STRUCTURE", "SUBDATASETS"
	 "GEOLOCATION", "RPC"等
	 *返回指针通过GTString::stringCount获取项的个数
	 */
	const char **getMetadata(const char* domainName) const;

	/*获取RasterBand数目, -1为有错误发生*/
	int getRasterBandCount() const;

	/*获取某一个波段的数据类型, 从1开始计数*/
	enumGTDataType getRasterBandDataType(int iBandNum) const;

	/*获取像素的排列方式*/
	enumGTPixelInterleave getPixelInterleave() const;

	/*获取列数*/
	int getXSize() const;

	/*获取行数*/
	int getYSize() const;

	/*获取空间范围, 对于non-north up的影像, 
	 *将给出旋转后的范围, 而不是原始的范围
	 *原始的范围可以由getOrgSpatialExtent给出*/
	bool getSpatialExtent(GTEnvelop& envelop) const;

	/*
	 *对于non-north up的影像和正轴影像给出四个角点的坐标 
	 */
	bool getOrgSpatialExtent(
		GTRawPoint& pt_leftTop, 
		GTRawPoint& pt_leftBottom,
		GTRawPoint& pt_rightTop,
		GTRawPoint& pt_rightBottom) const;

	/*Units value = (raw value * scale) + offset*/
	/*获取Scale*/
	double getBandValueScale(int iBandNum) const;
	
	/*获取Vaule Unit*/
	const char* getBandValueUnitType(int iBandNum) const;
	
	/*获取ValueOffset*/
	double getBandValueOffset(int iBandNum) const;

	/*获取某个波段像素的最小值*/
	double getBandMinimum(int iBandNum) const;

	/*获取某个波段像素的最大值*/
	double getBandMaximum(int iBandNum) const;

	/*获取某个波段像素的空白值*/
	double getBandNullValue(int iBandNum) const;

	/*获取X空间分辨率*/
	double getXResolution();
	
	/*获取Y空间分辨率*/
	double getYResolution();
	
	/*获取某一个波段的指针, 从1开始计数*/
	/*外部调用GTRasterBand::destroyRasterBand释放*/
	GTRasterBand *getRasterBand(int iBandNum);

	/*需要开发针对有subdataset的操作*/
	/*获取一个复合数据集的子集, iSubNum从1开始*/
	GTRasterDataset *getSubRasterDataset(int iSubNum);

    /*将本地栅格数据创建金字塔数据(包括所有波段)
	 *此处采用外部金子塔方式, 即Eardas的rrd, 相应的文件为.aux
	 *采样层次为1:2逐级别采样, 即比例大小为2:4:8:16:...2^n(bUseFirst将忽略2这个级别)
	 *顶层的大小为近似256*256
	 *采样方法由GDAL提供, 包括:"NEAREST", "GAUSS", "CUBIC", "AVERAGE", "MODE", 
	 *                      "AVERAGE_MAGPHASE" or "NONE
	 */
	bool createPyramid(enumGTPyraResample eResample, bool bUseFirst = false);

	/*清除金字塔数据*/
	bool clearPyramid();

    /*将本地栅格数据创建金字塔数据，参数意义如下：
	 *此处采用外部金子塔方式, 即Erdas的rrd, 相应的文件为.aux
	 *首先，nOverviews默认值为0（此时panOverviewList应该为NULL），
	 *如果为0则由算法自动确定需要有多少层，以及panOverviewList，
	 *其默认值为2，4，8，16，...
	 *nListBands默认为0（此时panBandList应该为NULL），如果为0则
	 *默认将所有波段均建立金字塔，pfnProgress可以外部提供，指示进度
	 *采样方法由GDAL提供, 包括:"NEAREST", "GAUSS", "CUBIC", "AVERAGE", "MODE", 
	 *                      "AVERAGE_MAGPHASE" or "NONE
	 */
	bool buildOverviews(enumGTPyraResample eResample = GT_PYRA_NEAREST,
		int  	nOverviews      = 0,
		int *  	panOverviewList = NULL,
		int  	nListBands      = 0,
		int *  	panBandList     = NULL,
		GDALProgressFunc  pfnProgress = NULL,
		void *  pProgressData = NULL);

protected:
	/*设置空间参考到内存, 应该与真实数据源保持一致*/
	bool setInternalSpatialRef(GTSpatialReference *spatialRef);

	void setDataSourceRefPtr(GTDataSource* pDataSource);

protected:
	GDALDataset *gdalDataset;
	bool         isOpened;  /*打开标志*/
	bool         isUpdated; /*是否更新标志*/

	GTSpatialReference *poSpatialRef; /*坐标系参考*/

	/*由于GTRasterDataset可能由GTDataSource管理
	 *因此需要加入GTDataSource的参考指针
	 */
	GTDataSource *pDataSourceRef;

	/*针对HDF4, HDF5, OGDI, netCDF等复合格式操作
	 *由GetMetadata获取subdataset的NAME, 传递给GDALOpen
	 */
	bool hasSubdatasets;
};

/*
 *GDAL-OGR Data Source的接口类
 */
class GTS_DLL GTOGRDataSource: public GTDataSource
{
public:
	friend class GTFeatureLayer;
	friend class GTOGRDataDriver;
	
	GTOGRDataSource(void);
	~GTOGRDataSource(void);

	/*由GTDataSource继承*/
	enumGTDataDriverType getDataDriverType() const { return GT_DATADRIVER_OGR; }
	const char* getDataDriverName() const;
	const char* getDataSourceName() const;
	bool openDataSource(const char* pSourceName, bool bUpdate);
	bool reopenDataSource(bool bUpdate);
	void closeDataSource();
	void clearDataSource();
	bool isDataSourceOpen() const { return isOpened; }
	bool isDataSourceValid() const;
	
	/*自有成员函数*/
	/*释放对象的静态函数, 考虑安全性, Feature引用不为0的禁止释放*/
	static bool destroyDataSource(GTOGRDataSource *pGtDataSource);

	/*获取是否为更新方式打开*/
	bool getIsReadOnly() const;

	/*获取OGRDataSource指针, 外部不允许修改*/
	OGRDataSource* getOgrDataSourceRefPtr() { return ogrDataSource; }

	/*获取该数据源中层的数目*/
	int getLayerCount() const;

	/*返回一个层数据, 需要外部释放GTS_DELETE, 或者GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getLayer(int iLayer);

	/*返回一个层数据, 需要外部释放GTS_DELETE, 或者GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getLayerByName(const char* layerName);

	/*删除层*/
	enumGTDataSourceErr deleteLayer(int iLayer);
	enumGTDataSourceErr deleteLayerByName(const char* layerName);

	/*创建层*/
	GTFeatureLayer* createLayer(const char* layerName, 
		                        GTSpatialReference* spatialRef, 
							    enumGTWKBGeometryType geomType,
								char**                options = 0);

protected:
	OGRDataSource *ogrDataSource;
	bool           isOpened;  /*打开标志*/
	bool           isUpdated; /*是否更新标志*/
};

#ifdef GDOS_DRAW_QUERY
typedef boost::function<GTGeometry*(enumGTWKBGeometryType, const OGRGeometry*, bool)>	GeometryFactoryFunction;
#endif

/*
 *MySQL Data source的接口类, 针对表单的SQL, 以及栅格文件
 */
class GTS_DLL GTGDOSMySQLDataSource : public GTDataSource
{
public:
	friend class GTFeatureLayer;
	friend class GTGDOSMySQLDataDriver;

	GTGDOSMySQLDataSource(void);
	virtual ~GTGDOSMySQLDataSource(void);

	/*由GTDataSource继承*/
	enumGTDataDriverType getDataDriverType() const { return GT_DATADRIVER_GDOS_MYSQL; }
// 	const char* getDataDriverName() const;
// 	const char* getDataSourceName() const;
	bool openDataSource(const char* pSourceName, bool bUpdate);/*Deprecated, 用自有函数替代*/
	bool reopenDataSource(bool bUpdate);
	void closeDataSource();
	void clearDataSource();
	bool isDataSourceOpen() const { return isOpened; }
	bool isDataSourceValid() const;
	bool changeSource(const char* sourceName);

	MYSQL*      getMySQLPtr() { return mysqlDataSource; }/*获取MYSQL连接指针*/

	/*自有成员函数*/
	const char* getErrMsg() const;
	int			getErrNo() const { return err_no; }
	void		clearErrNo() { err_no = GT_DS_SUCCESS; }
	const char* getHostName() const;
	const char* getUsername() const;
	const char* getPassword() const;
	const char* getDBName()   const;
	int         getPortNumber() const;

	/*释放数据源的静态函数*/
	static bool destroyDataSource(GTGDOSMySQLDataSource *pGtDataSource);

	/*设置连接超时步长, 单位为秒*/
	void setTimeoutLength(int outLen);
	int  getTimeoutLength() const;

	/*获取编码字符集类型*/
	const char* getCharacterSet() const;

	/*sourceName即为MySQL中schema名字*/
	bool openDataSource(const char* user, const char* pwd, 
		                const char* host, int port, const char* sourceName);
	
	/*为某一个数据层设置所有者信息*/
	bool setOwner(const char* lName, const char* owner, enumGTLayerType lType);

	/*获取某所有者的所有数据列表, 返回个数*/
	int getLayerNamesByOwner(
		const char* owner, 
		enumGTLayerType lType, 
		std::vector<std::string> &lname_list);

	/*删除某所有者的所有数据, 返回删除层数据的个数*/
	int deleteLayersByOwner(
		const char* owner, 
		enumGTLayerType lType);

	/*执行用户传入的SQL语句, 可以不只是针对一个schema*/
	bool executeSQL(const char* sqlStr);

    /*
	 *获取某个表单的记录个数
	 *-1为出现错误, 其他为记录数
	 */
	int getRecordCountFromTable(const char* tableName);

	/*
	 *获取某个表单的列的个数
	 *-1为出现错误, 其他为记录数
	 */
	int getFieldCountFromTable(const char* tableName);

	/*查询某个表单是否存在*/
	bool existTable(const char* tableName);

	/*
	 *根据图层名获取表名
	 *tableName为0时，必须调用GTS_FREE释放返回值
	 *外部传入tableName时应确保tableName大小至少为65
	 *返回值为0时表示失败
	 */
	const char* getTableNameByLayerName(const char* layerName, char* tableName = 0);

	/*************************************************************
	 *以下对应Feature数据的操作
	 *************************************************************/
	/*获取层的数目*/
	int getFeatureLayerCount();

	/*获取所有层的名字的集合, 返回-1数据源错误, 其它: 层的数目*/
	int getFeatureLayerNames(std::vector<std::string> &lname_list);

	/*返回一个层数据, 需要外部释放GTS_DELETE, 或者GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getFeatureLayer(
		int  iLayer, 
		bool bUpdate = false);

	/*返回一个层数据, 需要外部释放GTS_DELETE, 或者GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getFeatureLayerByName(
		const char* layerName, 
		bool        bUpdate = false);

	/*返回一个TIN层数据, 需要外部释放GTS_DELETE, 或者GTTinLayer::destroyTinLayer*/
	GTTinLayer* getTinLayerByName(const char* layerName);

	/*删除层*/
	enumGTDataSourceErr deleteFeatureLayer(int iLayer);
	enumGTDataSourceErr deleteFeatureLayerByName(const char* layerName);


	/*创建一个空的层, 只有属性信息, 和投影信息srid*/
	GTFeatureLayer *createFeatureLayer(
		const char* layerName, 
		GTSpatialReference* sRef, 
		GTFields* fieldsDefn, 
		enumGTWKBGeometryType geomType);

	/*创建一个层, 并导入数据*/
	bool addFeatureLayer(
		GTFeatureLayer* pFeatureLayer, 
		const char* layerName, 
		bool bLock = true);

	/*创建一个空的Tin层*/
	GTTinLayer *createTinLayer(
		const char* layerName, 
		GTSpatialReference* sRef);

	/*创建一个层, 并从数据文件导入数据, 暂不支持*/
	bool addFeatureLayerFromDataFile(
		GTFeatureLayer* pFeatureLayer, 
		const char*     layerName,
		const char*     dataFile);

	/*由Feanture的表单查询记录个数, 更新到系统表, 同时返回记录数*/
	int updateFeatureCount(const char* layerName);

	/*重命名Feature Layer*/
	bool renameFeatureLayer(
		const char* layerNameOld, 
		const char* layerNameNew);

	/*拷贝Feature Layer*/
	bool copyFeatureLayer(
		const char* layerNameFrom, 
		const char* layerNameTo);

	/*设置层的空间参考信息*/
	bool defineLayerSpatialReference(
		const char* layerName,
		const char* srsWKT);

#ifdef GDOS_DRAW_QUERY
	/*
	 *Feature的综合查询函数, 从给定的矢量层的表中获取数据
	 *如果返回的是FeatureClass, 用户外部必须传入一个GTFields的参考指针
	 *返回获取对象的个数, -1表明有错误发生
	 *属性与几何条件为AND, 几何条件之间为OR
	 */
// 	int queryFeaturesFromLayerTable(
// 		const char                  *tableName,            /*查询所基于的表名*/
// 		unsigned int                 uFlag,                /*0-只输出个数, 1-feature, 2-geom, 3-fid, 4-geom+fid*/
// 	    const char                  *sqlFilter,            /*属性查询表达, SQL中WHERE后面的部分*/
// 	    const GTEnvelop             *pEnvelopFilter,       /*查询矩形空间范围数组*/
// 	    const enumGTSpatialRelation *pSPR1,                /*空间关系表达数组*/
// 	    int                          nEnvelopCount,        /*pEnvelopFilter和pSPR1的个数*/
// 	    const GTGeometry           **ppGeometryFilter,     /*查询几何空间范围指针数组*/
// 		const enumGTSpatialRelation *pSPR2,                /*空间关系表达数组*/
// 	    int                          nGeometryFilterCount, /*ppGeometryFilter和pSPR2的个数*/
// 		const GTFields              *pFields,              /*输出Feature时需要传入属性定义指针, !!!不能在释放Feature之前释放!!!*/
// 	    bool                         bSortFid,             /*是否对fid按升序排序*/
// 	    std::vector<GTGeometry*>    *pGeomResult,          /*由uFlag控制是否输出，用户负责释放*/
// 	    std::vector<int>            *pFidResult,           /*由uFlag控制是否输出，用户负责释放*/
// 	    GTFeatureClass              *pFeatureClass);       /*由uFlag控制是否输出，用户负责释放*/

	struct QueryOptions {
		const char					*tableName;				/*查询所基于的表名*/
		unsigned int				 uFlag;					/*0-只输出个数, 1-feature, 2-geom, 3-fid, 4-geom+fid*/
		bool						 bUpdateEnv;			/*是否更新外包矩形*/
		const char					*sqlFilter;				/*属性查询表达, SQL中WHERE后面的部分*/
		const GTEnvelop				*pEnvelopFilter;		/*查询矩形空间范围数组*/
		const enumGTSpatialRelation	*pSPR1;					/*空间关系表达数组*/
		int							 nEnvelopCount;			/*pEnvelopFilter和pSPR1的个数*/
		const GTGeometry		   **ppGeometryFilter;		/*查询几何空间范围指针数组*/
		const enumGTSpatialRelation	*pSPR2;					/*空间关系表达数组*/
		int							 nGeometryFilterCount;	/*ppGeometryFilter和pSPR2的个数*/
		const GTFields				*pFields;				/*输出Feature时需要传入属性定义指针, !!!不能在释放Feature之前释放!!!*/
		int*						 nStartFid;				/*起始FID，用于分多次查询时，每次查询后被修改，下次查询可直接使用（uFlag为0或2时例外）*/
		int							 nRecordLimit;			/*单次查询行数限制*/
		bool						 bSortFid;				/*是否对fid按升序排序，nRecordLimit非0时必然排序，会忽略该值*/
		GeometryFactoryFunction*	 geomFac;				/*几何工厂*/
		std::vector<GTGeometry*>	*pGeomResult;			/*由uFlag控制是否输出，由用户传入*/
		std::vector<int>			*pFidResult;			/*由uFlag控制是否输出，由用户传入*/
		const std::vector<const char*> *pFieldNames;		/*需要提取的属性列名，uFlag为2~4时有效*/
		std::vector<std::string>	*pFieldValues;			/*提取的属性值，以$符号为分隔符，属性值内出现$时会以\转义。uFlag为2~4时有效*/
		GTFeatureClass				*pFeatureClass;			/*由uFlag控制是否输出，由用户传入*/
	};

	int queryFeaturesFromLayerTable(QueryOptions* queryOptions);

	typedef bool (*SelectCallbackProc)(GTGeometry*&, void* data);
	struct SelectOptions {
		const char					*tableName;
		const GTEnvelop				*pEnvelopFilter;
		const enumGTSpatialRelation	*pSPR;
		std::vector<int>			*pFidResult;
		int							 nLimit;
		SelectCallbackProc			 pCallback;
		void						*pCallbackData;
	};

	int selectFeaturesFromLayerTable(SelectOptions* selectOptions);
#endif

	bool lockTable(const char* layerName, bool bWrite = true);
	bool unlockTables();

protected:
	/*加入一条Feature数据记录, bCheck表明是否进行数据的有效性检查*/
	bool addFeatureToLayer(
		const char* layerName, 
		GTFeature*  featureObj, 
		bool        bCheck = false,
		bool        bForce = true);

	/*加入一系列Feature数据记录*/
	int addFeaturesToLayer(
		const char*     layerName, 
		const GTFeatureClass& fClass, 
		bool            bLock = true);
	
	/*删除Feature数据记录, 按照ID*/
	int deleteFeatureFromLayer(
		const char* layerName, 
		int         FID, 
		bool        bCheck = false,
		bool        bForce = false);

	/*删除一系列Feature数据记录, 按照ID, 返回现有记录个数*/
	int deleteFeaturesFromLayer(
		const char*             layerName, 
		const std::vector<int>& fid_list, 
		bool                    bLock = true);

	/*删除一系列Feature数据记录, 按照ID一次性操作*/
	int deleteFeaturesFromLayerBatch(
		const char*             layerName, 
		const std::vector<int>& fid_list, 
		bool                    bLock = true);

	/*更新一条Feature数据记录, 按照ID*/
	bool setFeatureToLayer(
		const char*      layerName, 
		const GTFeature* featureObj, 
		bool             bCheck = false);

	/*更新一系列Feature数据记录, 按照ID, 返回现有记录个数*/
	int setFeaturesToLayer(
		const char*     layerName, 
		const GTFeatureClass& fClass, 
		bool            bLock = true);

	/*为某一个层创建空间数据(geom)的索引*/
	/*一般应该在导入所有数据之后, 否则影响速度*/
	bool addFeatureSpatialIndex(const char* layerName);

	/*为某一个层删除空间数据(geom)的索引*/
	bool deleteFeatureSpatialIndex(const char* layerName);

	/*为某一个层的属性列创建数据索引, 名字为fdName_idx*/
	/*一般应该在导入所有数据之后, 否则影响速度*/
	bool addFeatureFieldIndex(
		const char* layerName, 
		const char* fieldName);

	/*删除某一个层的某个属性列的索引*/
	bool deleteFeatureFieldIndex(
		const char* layerName,  
		const char* fieldName);

	/*增加某一列到制定表*/
	bool addFeatureField(
		const char* layerName, 
		const GTField*    pField);

	/*从一个表中删除某一个列*/
	bool deleteFeatureField(
		const char* layerName, 
		const char* fieldName);

public:
	/*******************************************************************************
	 *以下对应Raster数据的操作, DataSource操作栅格对象为GTRasterDataset(对应GDALDataset)
	 *******************************************************************************/
	int getRasterDatasetCount();
	
	/*获取所有栅格数据的名字的集合, 返回-1数据源错误, 其它: 数据的数目*/
	int getRasterDatasetNames(std::vector<std::string> &rname_list);

	/*按照栅格数据id, 获取一个栅格数据句柄, bUpdate表明是否对数据进行写操作*/
	GTRasterDataset *getRasterDataset(
		int  rID, 
		bool bUpdate);
	
	/*按照栅格数据名称, 获取一个栅格数据句柄, bUpdate表明是否对数据进行写操作*/
	GTRasterDataset *getRasterDatasetByName(
		const char* poName, 
		bool        bUpdate);
	
	/*创建空的栅格数据, 返回句柄, 暂且不支持*/
	GTRasterDataset* createRasterDataset();

	/*查询栅格数据是否存在*/
	// zjw
	bool existRasterDataset(const char *poName, bool& exist);
	
	/*将本地栅格数据加入数据库, 可以将外部数据先拷贝到硬盘, 然后使用GDAL打开, 然后在系统表中注册*/
	bool addRasterDataset(
		const GTRasterDataset* pDataset, 
		const char           * poName);

	/*将本地栅格数据加入数据库, 并创建金字塔数据(包括所有波段)
	 *此处采用外部金子塔方式, 即Eardas的rrd, 相应的文件为.aux
	 *采样层次为1:2逐级别采样, 即比例大小为2:4:8:16:...2^n
	 *顶层的大小为近似64*64
	 *采样方法由GDAL提供, 包括:"NEAREST", "GAUSS", "CUBIC", "AVERAGE", "MODE", 
	 *                      "AVERAGE_MAGPHASE" or "NONE
	 *返回值: -1:数据注册不成功, 0:注册成功, 金字塔创建失败, 1:全部成功
	 */
	int addRasterDatasetPyramid(
		GTRasterDataset   *pDataset, 
		const char        *poName, 
		enumGTPyraResample eResample);

	/*删除一个栅格数据记录, bDeleteSource指示是否删除磁盘数据*/
	/*返回值: -1:数据不存在或者其他数据库错误, 0:表记录删除, 文件没有删除, 1:全部成功*/
	int deleteRasterDataset(
		const char* poName, 
		bool        bDeleteSource = false);

protected:
	/*针对数据库数据增加的信息记录*/
	char       dbName[65];         /*数据库名称*/
	char       *userName;       /*用户名*/
	char       *pswString;      /*口令*/
	char       *hostName;       /*主机地址*/
	int         portNumber;     /*端口号*/
	bool        isOpened;       /*打开标志*/
	MYSQL      *mysqlDataSource;/*MYSQL接口指针*/
	int         timeoutLength;  /*连接超时上限, 单位为秒, 同样的, 读和写都有超时设置*/
	int			err_no;
	std::string err_msg;        /*错误信息*/
	const char *charSet;        /*字符集编码, 常用utf8, gbk等*/
};

/*
 *Oracle Data source的接口类, 针对表单的SQL, 以及栅格文件
 */
// class GTS_DLL GTGDOSOCIDataSource: public GTDataSource
// {
// public:
// 
// 	friend class GTFeatureLayer;
// 	friend class GTGDOSOCIDataDriver;
// 
// 	GTGDOSOCIDataSource(void);
// 	~GTGDOSOCIDataSource(void);
// 
// 	/*由GTDataSource继承*/
// 	enumGTDataDriverType getDataDriverType() const;
// 	const char* getDataDriverName() const;
// 	const char* getDataSourceName() const;
// 	bool openDataSource(const char* pSourceName, bool bUpdate);/*Deprecated, 用自有函数替代*/
// 	bool reopenDataSource(bool bUpdate);
// 	void closeDataSource();
// 	void clearDataSource();
// 	bool isDataSourceOpen() const;
// 	bool isDataSourceValid() const;
// 
// 	/*自有成员函数*/
// 	const char* getOCIErrMsg() const; /*获取最近一次OCI执行的错误信息*/ 
// 	const char* getHostName() const;
// 	const char* getUsername() const;
// 	const char* getPassword() const;
// 	const char* getDBName()   const;
// 	int         getPortNumber() const;
// 
// 	/*释放数据源的静态函数*/
// 	static bool destroyDataSource(GTGDOSOCIDataSource *pGtDataSource);
// 
// 	bool openDataSource(
// 		const char* user,    /*登陆用户, 同时也是schema和datasourcename*/
// 		const char* pwd,     /*密码*/
// 		const char* host,    /*主机地址*/
// 		int         port,    /*端口*/
// 		const char* db,      /*oracle service, 也是dbName*/
// 		int         iMode=2);/*登陆身份, 默认为DBA*/
// 
// 	/*执行用户传入的SQL语句*/
// 	bool executeSQL(const char* sqlStr);
// 
// 	/*
// 	 *获取某个表单的记录个数
// 	 *-1为出现错误, 其他为记录数
// 	 */
// 	int getRecordCountFromTable(const char* tableName);
// 
// 	/*
// 	 *获取某个表单的列的个数
// 	 *-1为出现错误, 其他为记录数
// 	 */
// 	int getFieldCountFromTable(const char* tableName);
// 
// 	/*查询某个表单是否存在*/
// 	bool existTable(const char* tableName);
// 
// 	/*************************************************************
// 	 *以下对应Feature数据的操作
// 	 *************************************************************/
// 	/*获取层的数目*/
//     int getFeatureLayerCount();
// 	
//     /*删除层, iLayer从1开始*/
// 	bool deleteFeatureLayer(int iLayer);
// 	bool deleteFeatureLayerByName(const char* layerName);
// 
// 	/*创建一个空的层, 只有属性信息, 和投影信息srid*/
// 	GTFeatureLayer *createFeatureLayer(
// 		const char* layerName, 
// 		GTSpatialReference* sRef, 
// 		GTFields* fieldsDefn, 
// 		enumGTWKBGeometryType geomType);
// 
// protected:
//     /*为某一个层创建空间数据(geom)的索引*/
// 	/*一般应该在导入所有数据之后, 否则影响速度*/
// 	bool addFeatureSpatialIndex(const char* layerName);
// 
// 	/*为某一个层删除空间数据(geom)的索引*/
// 	bool deleteFeatureSpatialIndex(const char* layerName);
// 
// protected:
// 	/*针对数据库数据增加的信息记录*/
// 	char    *dbName;          /*数据库名称*/
// 	char    *userName;        /*用户名, 与dataSourceName相同*/
// 	char    *pswString;       /*口令*/
// 	char    *hostName;        /*主机地址*/
// 	int      portNumber;      /*端口号*/
// 	bool     isOpened;        /*打开标志*/
// // 	GTOCISession *pOCISession;/*oci接口指针*/
// };

}
#endif
