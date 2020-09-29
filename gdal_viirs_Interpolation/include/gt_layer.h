/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  各种层的定义类
* Author:   范俊甫, fanjf@lreis.ac.cn 2010年-12月-13日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTLAYER_H_INCLUDED
#define GTLAYER_H_INCLUDED

#include "gt_geometry.h"
#include "gt_spatialreference.h"
#include "gt_datasource.h"
#include "gt_feature.h"

/*GDAL header files*/
#include "../include_p/gdal/ogr_core.h"
#include "../include_p/gdal/ogr_api.h"
#include "../include_p/gdal/ogrsf_frmts.h"
#include "../include_p/gdal/gdal_priv.h"
#include "../include_p/gdal/gdal_rat.h"

namespace gts
{

class GTDataSource;
class GTRasterDataset;
/*
 *层定义的抽象类, 联合了数据源和空间坐标描述
 */
class GTS_DLL GTLayer
{
public:
	GTLayer(void);
	virtual ~GTLayer(void);

	/*获取层的类型*/
	virtual enumGTLayerType getLayerType() const = 0;

	/*获取层的Envelop*/
	virtual GTEnvelop getLayerEnvelop() const = 0;

	/*获取数据格式驱动名称*/
	virtual const char *getDataDriverName() const = 0;
	
	/*获取数据源信息(文件路径以及数据库连接串)*/
	virtual const char *getDataSourceName() const = 0;
	
	/*清除DadaSource指针*/
	virtual void releaseDataSourceRefPtr() = 0;

	/*返回DadaSource指针*/
	virtual GTDataSource* getDataSourceRefPtr() = 0;

	/*获取层的空间坐标信息指针*/
	virtual GTSpatialReference *getLayerSpatialRefPtr() = 0;
	
	/*获取层名*/
	virtual const char *getLayerName() const = 0;
	/*获取层别名*/
	virtual const char *getLayerAlias() const = 0;
	/*清除本身成员内存*/
	virtual void clearAll() = 0;
	/*获取数据驱动源类型*/
	virtual enumGTDataDriverType getDataDriverType() const = 0;

	/*以下私有函数只是针对成员变量的赋值操作, 与数据源无关*/
protected:
	/*设置层的Envelop*/
	virtual void setLayerEnvelop(const GTEnvelop& envelop) = 0;

	/*设定DadaSource指针*/
	virtual void setDataSourceRefPtr(GTDataSource* pDataSource) = 0;

	/*设定层的空间坐标信息, 内部分配, 内部释放*/
	virtual bool setLayerSpatialRef(GTSpatialReference *spatialRef) = 0;

	/*设置数据格式驱动类型*/
	virtual void setDataDriverName(const char *driverName) = 0;
	
	/*设置数据源信息*/
	virtual void setDataSourceName(const char *sourceName) = 0;

	/*设置层名*/
	virtual void setLayerName(const char *lName) = 0;

	/*设置层别名*/
	virtual void setLayerAlias(const char *lAlias) = 0;

	/*设置数据驱动源类型*/
	virtual void setDataDriverType(enumGTDataDriverType dDriverType) = 0;

protected:
	GTSpatialReference  *poSpatialRef;
	GTEnvelop            lyEnvelop;
	char                *poName;
	char                *poAlias;
	GTDataSource        *pDataSourceRef;
	char                *dataSourceName;
	char                *dataDriverName;
	enumGTDataDriverType dataDriverType;
};

/** @brief GTFeatureLayer 
*/
class GTS_DLL GTFeatureLayer: public GTLayer
{
public:
	friend class GTOGRDataSource;
	friend class GTGDOSMySQLDataSource;
	friend class GTGDOSOCIDataSource;

	GTFeatureLayer();
	virtual ~GTFeatureLayer();

/** @name 由GTLayer继承, 不针对数据源的操作
   *@{*/
	enumGTLayerType getLayerType() const { return GT_LAYER_Feature; }
	GTEnvelop getLayerEnvelop() const { return lyEnvelop; }
	void releaseDataSourceRefPtr();
	GTDataSource* getDataSourceRefPtr() { return pDataSourceRef; }
	GTSpatialReference *getLayerSpatialRefPtr() { return poSpatialRef; }
	const char *getDataDriverName() const { return dataDriverName; }
	const char *getDataSourceName() const { return dataSourceName; }
	const char *getLayerName() const { return poName; }
	const char *getLayerAlias() const { return poAlias; }
	void clearAll();
	enumGTDataDriverType getDataDriverType() const { return dataDriverType; }
	bool rename(const char* new_name);	// by zjw
	
protected:
	void setLayerAlias(const char *lAlias);
	void setLayerName(const char *lName);
	void setDataSourceName(const char *sourceName);
	void setDataDriverName(const char *driverName);
	void setDataDriverType(enumGTDataDriverType dDriverType);
	bool setLayerSpatialRef(GTSpatialReference *spatialRef);
	void setLayerEnvelop(const GTEnvelop& envelop) { lyEnvelop = envelop; }
	void setDataSourceRefPtr(GTDataSource* pDataSource);
	/** @} */

public:
	/** @name 自有成员函数
      *@{*/

	/** @brief 静态清除一个GTFeatureLayer对象的函数*/
	static bool destroyFeatureLayer(GTFeatureLayer *poFeatureLayer);

	/** @brief 是否为更新模式打开*/
	bool isUpdateMode() const { return isUpdated; }

	/** @brief 从数据源更新属性列描述定义*/
	bool updateFieldsDefnRef();
	
	/** @brief 获取属性列描述定义指针*/
	GTFields *getFieldsDefnPtr() { return fieldsDefnRef; }
	const GTFields *getFieldsDefnPtr() const { return fieldsDefnRef; }

	/** @brief 获取对象个数*/
	int getFeatureCount() const { return nFeatureCount; }

	/** @brief 获取Feature对象, iFeature为FID*/
	GTFeature *getFeature(int iFID);

	/** @brief 只获取几何数据
	  * @note 外部调用GTS_DELETE释放*/
	GTGeometry *getGeometry(int iFID);

	/** @brief 获取下一个Feature对象*/
	GTFeature *getNextFeature();

	/** @brief 重新设置Feature对象游标*/
	void resetReading();

	/** @brief 删除Feature到数据源*/
	bool deleteFeature(
		int  iFID,
		bool bForce = false);

	/** @brief 增加Feature到数据源*/
	bool createFeature(GTFeature *pFeature, bool bUpdateEnv);

	/** @brief 更新Feature到数据源*/
	bool setFeature(GTFeature *pFeature, bool bUpdateEnv);

	/** @brief 删除一系列Feature到数据源*/
	bool deleteFeatures(
		const std::vector<int>& fid_list, 
		bool                    bLock = true);

	/** @brief 增加一系列Feature到数据源*/
	bool createFeatures(
		GTFeatureClass& fClass, 
		bool            bLock = true);

	/** @brief 更新一系列Feature到数据源*/
	bool setFeatures(
		GTFeatureClass& fClass, 
		bool            bLock = true);

	/** @brief 获取层的几何类型信息
	  * @note 注意polygon 与multipolygon常处于一层*/
	enumGTWKBGeometryType getGeometryType() const { return eGeometryType; }

	/** @brief 在数据源上创建空间索引
	    @return 不支持或失败返回false*/
	bool createSpatialIndex();

	/** @brief 在数据源上删除空间索引
	    @return 不支持或失败返回false*/
	bool deleteSpatialIndex();

	/** @brief 针对属性列的编辑,增加属性列到数据源,
	    @note 此时fieldsDefnRef不再有效, 需要重新获取\n
	    在此操作之前应该释放所有的与之关联的Feature和FeatureClass\n
	    完成操作之后, 将自动更新fieldsDefnRef*/
	bool createField(const GTField* fieldDef);

	/** @brief 按照名字删除属性列到数据源
	    @note 此时fieldsDefnRef不再有效, 需要重新获取\n
	    在此操作之前应该释放所有的与之关联的Feature和FeatureClass\n
	    完成操作之后, 将自动更新fieldsDefnRef*/
	bool deleteField(const char* fdName);

	/** @brief 增加属性列的索引到数据源*/
	bool createFieldIndex(const char* fdName);

	/** @brief 删除属性列的索引到数据源*/
	bool deleteFieldIndex(const char* fdName);

	/** @brief 更新层的在数据源和内存的Envelop 
	    @note 需要在数据源遍历所有几何对象*/
	void updateEnvelop();

	/** @brief 获取某列的最大值，列的类型必须为数字**/
	bool getFieldMaxValue(const char *fdName, double& dValue) const;

	/** @brief 获取某列的最小值，列的类型必须为数字**/
	bool getFieldMinValue(const char *fdName, double& dValue) const;

	/** @brief 获取某列的平均值，列的类型必须为数字**/
	bool getFieldAvgValue(const char *fdName, double& dValue) const;

	/** @brief 获取某列的标准偏差，列的类型必须为数字**/
	bool getFieldStdValue(const char *fdName, double& dValue) const;

	/** @brief 获取某列的数值和，列的类型必须为数字**/
	bool getFieldSumValue(const char *fdName, double& dValue) const;

	/** @brief 属性条件查询 
	  * @note 输入应为SQL中WHERE后条件表达部分, 需要预先逃逸字符串, 如果为空, 则选取全部数据
	  * @return 返回-1表明有错误发生, 否则返回对象个数, 结果在fClass中\n
	    @note FeatureClass中为Feature指针集合, 其引用了FeatureLayer的Fields指针\n
	    需要在FeatrueLayer之前释放, 调用clearAll成员函数*/
	int getFeaturesByAttributeFilter(
		const char*     sqlFilter, 
		GTFeatureClass& fClass,
		bool bOrderByFID = false);

	/** @brief 属性条件查询, 只返回几何对象, 注意指针链表的释放*/
	int getFeaturesByAttributeFilter(
		const char*                sqlFilter,
		std::vector<GTGeometry *>& geom_list,
		std::vector<int>&          fid_list,
		bool bOrderByFID = false);

	/** @brief 属性条件查询, 只返回id*/
	int getFeaturesByAttributeFilter(
		const char*       sqlFilter,
		std::vector<int>& fid_list);

	/** @brief Envelop位置条件查询
	    @note 由于没有索引OGR读文件应该效率较低, 目前对于OGR只实现Intersect关系\n
	    只根据几何对象的Envelop关系进行查询
	    @return 返回-1表明有错误发生, 否则返回对象个数,结果在fClass中
	    @note FeatureClass中为Feature指针集合, 其引用了FeatureLayer的Fields指针\n
	    需要在FeatrueLayer之前释放, 调用clearAll成员函数*/
	int getFeaturesByEnvelopFilter(
		const GTEnvelop&             gtEnvelopFilter, 
		enumGTSpatialRelation        eSR, 
		GTFeatureClass&              fClass,
		bool bOrderByFID = false);

	/** @brief Envelop位置条件查询
	    @note 只返回几何对象, 注意指针链表的释放*/
	int getFeaturesByEnvelopFilter(
		const GTEnvelop&             gtEnvelopFilter,
		enumGTSpatialRelation        eSR,
		std::vector<GTGeometry *>&   geom_list,
		std::vector<int>&            fid_list,
		bool bOrderByFID = false);
	
	/** @brief Envelop位置条件查询
	    @note 只返回id*/
	int getFeaturesByEnvelopFilter(
		const GTEnvelop&             gtEnvelopFilter,
		enumGTSpatialRelation        eSR,
		std::vector<int>&            fid_list);

	/** @brief 几何位置条件查询
	    @note 只根据几何对象的空间关系进行查询
	    @return 返回-1表明有错误发生, 否则返回对象个数
	    @note FeatureClass中为Feature指针集合, 其引用了FeatureLayer的Fields指针\n
	    需要在FeatrueLayer之前释放, 调用clearAll成员函数*/
	int getFeaturesByGeometryFilter(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		GTFeatureClass&              fClass,
		bool bOrderByFID = false);

	/** @brief 几何位置条件查询, 只返回几何对象, 注意指针链表的释放*/
	int getFeaturesByGeometryFilter(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		std::vector<GTGeometry *>&   geom_list,
		std::vector<int>&            fid_list,
		bool bOrderByFID = false);

	/** @brief 几何位置条件查询, 只返回id*/
	int getFeaturesByGeometryFilter(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		std::vector<int>&            fid_list);

	/** @brief 混合条件查询, 只返回FeatureClass*/
	int getFeaturesByConditions(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		const char*                  sqlFilter,
		GTFeatureClass&              fClass,
		bool bOrderByFID = false);

	/** @brief 混合条件查询, 只返回几何数据, 注意链表指针释放*/
	int getFeaturesByConditions(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		const char*                  sqlFilter,
		std::vector<GTGeometry *>&   geom_list,
		std::vector<int>&            fid_list,
		bool bOrderByFID = false);

	/** @brief 混合条件查询, 只返回id*/
	int getFeaturesByConditions(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
	    const char*                  sqlFilter,
		std::vector<int>&            fid_list);

	/** @brief 获取OGRLayer的指针, 只针对GTOGRDataSource有效, 其它数据源皆为空*/
	OGRLayer *getOGRLayerRef() { return ogrLayer; }
	/** @} */

	/** @brief 重新定义层的投影信息到数据源*/
	bool resetSpatialReference(GTSpatialReference *pSpatialRef);

	/** @brief 生成MySQL风格的数据文件, 用于"LOAD DATA LOCAL INFILE快速导入操作"*/
	bool exportToMySQLDataFile(const char* pszFileName);

	const char* getErrMsg() const;
	int			getErrNo() const { return err_no; }
	void		clearErrNo() { err_no = GT_DS_SUCCESS; }

protected:
	GTFields              *fieldsDefnRef;
	int                    nFeatureCount;
	enumGTWKBGeometryType  eGeometryType;

	/*对象读取游标, 由getNextFeature和resetRead来控制
	 *读取最后一个对象后, 将其设置为开始处0
	 */
	int nFeatureRead;

	/*OGRLayer指针, 只针对GTOGRDataSource有效, 其它数据源皆为空*/
	OGRLayer *ogrLayer;

	bool  isUpdated; /*是否更新标志*/

	int          err_no;
	std::string  err_msg;        /*错误信息*/

protected:
	/*设置更新标志*/
	void setUpdated(bool bUpdate) { isUpdated = bUpdate; }

	/** @brief 创建属性列描述定义*/
	bool setFieldsDefnRef(const GTFields* gtFieldsDef);

	/** @brief 设置对象个数
	    @note 只是内存对象, 与数据源无关*/
	void setFeatureCount(unsigned int fObjCount) { nFeatureCount = fObjCount; }

	/** @brief 设置层的几何类型信息*/
	void setGeometryType(enumGTWKBGeometryType  eGeomType);


/*以下为私有函数, 对应不同DataSource类型的分别实现, 
 *实现函数在各自对应的cpp中
 */
private:
	// zjw
	bool (GTFeatureLayer::*_updateFieldsDefnRef_func)();
	GTFeature* (GTFeatureLayer::*_getFeature_func)(int);
	GTGeometry* (GTFeatureLayer::*_getGeometry_func)(int);
	bool (GTFeatureLayer::*_deleteFeature_func)(int, bool);
	bool (GTFeatureLayer::*_createFeature_func)(GTFeature *, bool);
	bool (GTFeatureLayer::*_setFeature_func)(GTFeature *, bool);
	bool (GTFeatureLayer::*_deleteFeatures_func)(const std::vector<int>&, bool bLock);
	bool (GTFeatureLayer::*_createFeatures_func)(GTFeatureClass&, bool);
	bool (GTFeatureLayer::*_setFeatures_func)(GTFeatureClass&, bool);
	bool (GTFeatureLayer::*_createSpatialIndex_func)();
	bool (GTFeatureLayer::*_deleteSpatialIndex_func)();
	bool (GTFeatureLayer::*_createField_func)(const GTField*);
	bool (GTFeatureLayer::*_deleteField_func)(const char*);
	bool (GTFeatureLayer::*_createFieldIndex_func)(const char*);
	bool (GTFeatureLayer::*_deleteFieldIndex_func)(const char*);
	bool (GTFeatureLayer::*_updateEnvelop_func)();
	int (GTFeatureLayer::*_getFeaturesByAttributeFilter_fc_o_func)(const char*, GTFeatureClass&, bool);
	int (GTFeatureLayer::*_getFeaturesByAttributeFilter_g_f_o_func)(const char*, std::vector<GTGeometry *>&, std::vector<int>&, bool);
	int (GTFeatureLayer::*_getFeaturesByAttributeFilter_f_func)(const char*, std::vector<int>&);
	int (GTFeatureLayer::*_getFeaturesByEnvelopFilter_fc_o_func)(const GTEnvelop&, enumGTSpatialRelation, GTFeatureClass&, bool);
	int (GTFeatureLayer::*_getFeaturesByEnvelopFilter_g_f_o_func)(const GTEnvelop&, enumGTSpatialRelation, std::vector<GTGeometry *>&, std::vector<int>&, bool);
	int (GTFeatureLayer::*_getFeaturesByEnvelopFilter_f_func)(const GTEnvelop&, enumGTSpatialRelation, std::vector<int>&);
	int (GTFeatureLayer::*_getFeaturesByGeometryFilter_fc_o_func)(const GTGeometry*, enumGTSpatialRelation, GTFeatureClass&, bool);
	int (GTFeatureLayer::*_getFeaturesByGeometryFilter_g_f_o_func)(const GTGeometry*, enumGTSpatialRelation, std::vector<GTGeometry *>&, std::vector<int>&, bool);
	int (GTFeatureLayer::*_getFeaturesByGeometryFilter_f_func)(const GTGeometry*, enumGTSpatialRelation, std::vector<int>&);
	int (GTFeatureLayer::*_getFeaturesByConditions_fc_o_func)(const GTGeometry*, enumGTSpatialRelation, const char*, GTFeatureClass&, bool);
	int (GTFeatureLayer::*_getFeaturesByConditions_g_f_o_func)(const GTGeometry*, enumGTSpatialRelation, const char*, std::vector<GTGeometry *>&, std::vector<int>&, bool);
	int (GTFeatureLayer::*_getFeaturesByConditions_f_func)(const GTGeometry*, enumGTSpatialRelation, const char*, std::vector<int>&);
	bool (GTFeatureLayer::*_resetSpatialReference_func)(GTSpatialReference *);

	bool updateFieldsDefnRef_ogr    ();
	bool updateFieldsDefnRef_mysql  ();
	bool updateFieldsDefnRef_beyondb();
	bool updateFieldsDefnRef_oracle ();
	bool updateFieldsDefnRef_postgis();
	bool updateFieldsDefnRef_odbc   ();

	GTFeature *getFeature_ogr    (int iFID);
	GTFeature *getFeature_mysql  (int iFID);
	GTFeature *getFeature_beyondb(int iFID);
	GTFeature *getFeature_oracle (int iFID);
	GTFeature *getFeature_postgis(int iFID);
	GTFeature *getFeature_odbc   (int iFID);

	GTGeometry *getGeometry_ogr    (int iFID);
	GTGeometry *getGeometry_mysql  (int iFID);
	GTGeometry *getGeometry_beyondb(int iFID);
	GTGeometry *getGeometry_oracle (int iFID);
	GTGeometry *getGeometry_postgis(int iFID);
	GTGeometry *getGeometry_odbc   (int iFID);

	bool deleteFeature_ogr    (int iFID, bool bForce = false);
	bool deleteFeature_mysql  (int iFID, bool bForce = false);
	bool deleteFeature_beyondb(int iFID, bool bForce = false);
	bool deleteFeature_oracle (int iFID, bool bForce = false);
	bool deleteFeature_postgis(int iFID, bool bForce = false);
	bool deleteFeature_odbc   (int iFID, bool bForce = false);

	bool createFeature_ogr    (GTFeature *pFeature, bool bUpdateEnv);
	bool createFeature_mysql  (GTFeature *pFeature, bool bUpdateEnv);
	bool createFeature_beyondb(GTFeature *pFeature, bool bUpdateEnv);
	bool createFeature_oracle (GTFeature *pFeature, bool bUpdateEnv);
	bool createFeature_postgis (GTFeature *pFeature, bool bUpdateEnv);
	bool createFeature_odbc   (GTFeature *pFeature, bool bUpdateEnv);

	bool setFeature_ogr    (GTFeature *pFeature, bool bUpdateEnv);
	bool setFeature_mysql  (GTFeature *pFeature, bool bUpdateEnv);
	bool setFeature_beyondb(GTFeature *pFeature, bool bUpdateEnv);
	bool setFeature_oracle (GTFeature *pFeature, bool bUpdateEnv);
	bool setFeature_postgis(GTFeature *pFeature, bool bUpdateEnv);
	bool setFeature_odbc   (GTFeature *pFeature, bool bUpdateEnv);

	bool _makeFeature_ogr(OGRFeature*& pOGRFeature, GTFeature *pFeature);

	bool deleteFeatures_ogr    (const std::vector<int>& fid_list, bool bLock = true);
	bool deleteFeatures_mysql  (const std::vector<int>& fid_list, bool bLock = true);
	bool deleteFeatures_beyondb(const std::vector<int>& fid_list, bool bLock = true);
	bool deleteFeatures_oracle (const std::vector<int>& fid_list, bool bLock = true);
	bool deleteFeatures_postgis(const std::vector<int>& fid_list, bool bLock = true);
	bool deleteFeatures_odbc   (const std::vector<int>& fid_list, bool bLock = true);

	bool createFeatures_ogr    (GTFeatureClass& fClass, bool bLock = true);
	bool createFeatures_mysql  (GTFeatureClass& fClass, bool bLock = true);
	bool createFeatures_beyondb(GTFeatureClass& fClass, bool bLock = true);
	bool createFeatures_oracle (GTFeatureClass& fClass, bool bLock = true);
	bool createFeatures_postgis(GTFeatureClass& fClass, bool bLock = true);
	bool createFeatures_odbc   (GTFeatureClass& fClass, bool bLock = true);

	bool setFeatures_ogr    (GTFeatureClass& fClass, bool bLock = true);
	bool setFeatures_mysql  (GTFeatureClass& fClass, bool bLock = true);
	bool setFeatures_beyondb(GTFeatureClass& fClass, bool bLock = true);
	bool setFeatures_oracle (GTFeatureClass& fClass, bool bLock = true);
	bool setFeatures_postgis(GTFeatureClass& fClass, bool bLock = true);
	bool setFeatures_odbc   (GTFeatureClass& fClass, bool bLock = true);

	bool createSpatialIndex_ogr    ();
	bool createSpatialIndex_mysql  ();
	bool createSpatialIndex_beyondb();
	bool createSpatialIndex_oracle ();
	bool createSpatialIndex_postgis();
	bool createSpatialIndex_odbc   ();

	bool deleteSpatialIndex_ogr    ();
	bool deleteSpatialIndex_mysql  ();
	bool deleteSpatialIndex_beyondb();
	bool deleteSpatialIndex_oracle ();
	bool deleteSpatialIndex_postgis();
	bool deleteSpatialIndex_odbc   ();

	bool createField_ogr    (const GTField* fieldDef);
	bool createField_mysql  (const GTField* fieldDef);
	bool createField_beyondb(const GTField* fieldDef);
	bool createField_oracle (const GTField* fieldDef);
	bool createField_postgis(const GTField* fieldDef);
	bool createField_odbc   (const GTField* fieldDef);

	bool deleteField_ogr    (const char* fdName);
	bool deleteField_mysql  (const char* fdName);
	bool deleteField_beyondb(const char* fdName);
	bool deleteField_oracle (const char* fdName);
	bool deleteField_postgis(const char* fdName);
	bool deleteField_odbc   (const char* fdName);

	bool createFieldIndex_ogr    (const char* fdName);
	bool createFieldIndex_mysql  (const char* fdName);
	bool createFieldIndex_beyondb(const char* fdName);
	bool createFieldIndex_oracle (const char* fdName);
	bool createFieldIndex_postgis(const char* fdName);
	bool createFieldIndex_odbc   (const char* fdName);

	bool deleteFieldIndex_ogr    (const char* fdName);
	bool deleteFieldIndex_mysql  (const char* fdName);
	bool deleteFieldIndex_beyondb(const char* fdName);
	bool deleteFieldIndex_oracle (const char* fdName);
	bool deleteFieldIndex_postgis(const char* fdName);
	bool deleteFieldIndex_odbc   (const char* fdName);

	bool updateEnvelop_ogr    ();
	bool updateEnvelop_mysql  ();
	bool updateEnvelop_beyondb();
	bool updateEnvelop_oracle ();
	bool updateEnvelop_postgis();
	bool updateEnvelop_odbc   ();

	int getFeaturesByAttributeFilter_ogr    (const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_mysql  (const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_beyondb(const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_oracle (const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_postgis(const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_odbc   (const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);

	int getFeaturesByAttributeFilter_ogr    (const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_mysql  (const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_beyondb(const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_oracle (const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_postgis(const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByAttributeFilter_odbc   (const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);

	int getFeaturesByAttributeFilter_ogr    (const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByAttributeFilter_mysql  (const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByAttributeFilter_beyondb(const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByAttributeFilter_oracle (const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByAttributeFilter_postgis(const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByAttributeFilter_odbc   (const char* sqlFilter, std::vector<int>& fid_list);

	int getFeaturesByEnvelopFilter_ogr    (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_mysql  (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_beyondb(const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_oracle (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_postgis(const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_odbc   (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);

	int getFeaturesByEnvelopFilter_ogr    (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_mysql  (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_beyondb(const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_oracle (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_postgis(const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByEnvelopFilter_odbc   (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);

	int getFeaturesByEnvelopFilter_ogr    (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByEnvelopFilter_mysql  (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByEnvelopFilter_beyondb(const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByEnvelopFilter_oracle (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByEnvelopFilter_postgis(const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByEnvelopFilter_odbc   (const GTEnvelop& gtEnvelopFilter, enumGTSpatialRelation eSR, std::vector<int>& fid_list);

	int getFeaturesByGeometryFilter_ogr    (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_mysql  (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_beyondb(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_oracle (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_postgis(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_odbc   (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, GTFeatureClass& fClass, bool bOrderByFID = false);

	int getFeaturesByGeometryFilter_ogr    (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_mysql  (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_beyondb(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_oracle (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_postgis(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByGeometryFilter_odbc   (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);

	int getFeaturesByGeometryFilter_ogr    (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByGeometryFilter_mysql  (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByGeometryFilter_beyondb(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByGeometryFilter_oracle (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByGeometryFilter_postgis(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<int>& fid_list);
	int getFeaturesByGeometryFilter_odbc   (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, std::vector<int>& fid_list);

	int getFeaturesByConditions_ogr    (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByConditions_mysql  (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByConditions_beyondb(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByConditions_oracle (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByConditions_postgis(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);
	int getFeaturesByConditions_odbc   (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, GTFeatureClass& fClass, bool bOrderByFID = false);

	int getFeaturesByConditions_ogr    (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByConditions_mysql  (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByConditions_beyondb(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByConditions_oracle (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByConditions_postgis(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);
	int getFeaturesByConditions_odbc   (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<GTGeometry *>& geom_list, std::vector<int>& fid_list, bool bOrderByFID = false);

	int getFeaturesByConditions_ogr    (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByConditions_mysql  (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByConditions_beyondb(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByConditions_oracle (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByConditions_postgis(const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<int>& fid_list);
	int getFeaturesByConditions_odbc   (const GTGeometry* poGeometry, enumGTSpatialRelation eSR, const char* sqlFilter, std::vector<int>& fid_list);

	bool resetSpatialReference_ogr    (GTSpatialReference *pSpatialRef);
	bool resetSpatialReference_mysql  (GTSpatialReference *pSpatialRef);
	bool resetSpatialReference_beyondb(GTSpatialReference *pSpatialRef);
	bool resetSpatialReference_oracle (GTSpatialReference *pSpatialRef);
	bool resetSpatialReference_postgis(GTSpatialReference *pSpatialRef);
	bool resetSpatialReference_odbc   (GTSpatialReference *pSpatialRef);

	bool exportToMySQLDataFile_org(const char* pszFileName);
};

/*
 *GTTinLayer定义类
 */
class GTS_DLL GTTinLayer: public GTFeatureLayer
{
public:
	friend class GTOGRDataSource;
	friend class GTGDOSMySQLDataSource;

	GTTinLayer();
	virtual ~GTTinLayer();

    enumGTLayerType getLayerType() const { return GT_LAYER_TIN; }

public:
	static bool destroyTinLayer(GTTinLayer *poTinLayer);

	/*以下函数在TinLayer中关闭*/
	bool updateFieldsDefnRef();
	bool deleteFeature(int iFID, bool bForce = false);
	bool setFeature(GTFeature *pFeature);
	bool deleteFeatures(const std::vector<int>& fid_list, const int& fNumber, bool bLock = true);
	bool setFeatures(GTFeatureClass& fClass, bool bLock = true);
	bool createField(GTField* fieldDef);
	bool deleteField(const char* fdName);
	bool createFieldIndex(const char* fdName);
	bool deleteFieldIndex(const char* fdName);

protected:

};

class GTS_DLL GTRasterColorTable
{
public:
	GTRasterColorTable();
	GTRasterColorTable(const GDALColorTable& gdal_ctable);
	~GTRasterColorTable();

	void clearAll() { entry_list.clear(); }
	int getEntryCount() const { return (int)entry_list.size(); }
	GTPaletteInterp getPaletteMode() const { return eInterpMode; }
	void setPaletteMode(GTPaletteInterp eMode) { eInterpMode = eMode; }
	GTColorEntry *getEntryPtrAt(int i);
	void deleteEntryAt(int i);
	void addEntry(const GTColorEntry& eEntry);
	void setEntryAt(const GTColorEntry& eEntry, int i);

protected:
	GTPaletteInterp eInterpMode;
	std::vector<GTColorEntry> entry_list;
	//int entry_count;
};

/*AttributeTable直接使用GDAL的类继承, 需要修改*/
class GTS_DLL GTRasterAttributeTable : public GDALRasterAttributeTable
{
public:
	GTRasterAttributeTable();
	~GTRasterAttributeTable();
};

/** *@brief GTRasterBand类*/
class GTS_DLL GTRasterBand
{
public:
	friend class GTRasterDataset;
	GTRasterBand();
	~GTRasterBand();

	/** @brief 清除GTRasterBand指针的静态函数*/
	static void destroyGTRasterBand(GTRasterBand* poRasterBand);

	/** @brief 直接获取GDALRasterBand的操作指针, 外部可以通过此指针调用GDAL函数进行操作*/
	GDALRasterBand *getGDALRasterBandRef() { return gdalRasterBand; }
	
	/** @name 以下重写GDALRasterBand部分函数
      * @{*/

	/** @brief 获取Metadata, 外部不能释放
	  * @note domainName为空, 则返回metadata目录\n
	  返回指针通过GTString::stringCount获取项的个数*/
	const char **getMetadata(const char* domainName) const;

	/** @brief 获取Category Name*/
	const char **getCategoryNames() const;

	/** @brief 获取数据类型*/
	enumGTDataType getDataType() const;

	/** @brief 获取单位*/
	const char* getUnitType();

	/** @brief 读写数据块
	    @note pData必须由外部分配(malloc或者new)，外部释放
		@param bIsReader 是否写
		@param nXOff x偏移
		@param nYOff y偏移
		@param nXSize x大小
		@param nYSize y大小
		@param pData 目标数据缓存
		@param nBufXSize 目标数据x大小
		@param nBufYSize 目标数据y大小
		@param eBufType 数据类型
		@param nPixelSpace 横向像素需要的偏移量
		@param nLineSpace 纵向像素需要的偏移量*/ 
	bool rasterIO(
		bool  bRead,            /*是否写*/
		int   nXOff,            /*x偏移*/
		int   nYOff,            /*y偏移*/
		int   nXSize,           /*x大小*/
		int   nYSize,           /*y大小*/
		void *pData,            /*目标数据缓存*/
		int   nBufXSize,        /*目标数据x大小*/
		int   nBufYSize,        /*目标数据y大小*/
		enumGTDataType eBufType,/*数据类型*/
		int   nPixelSpace = 0,	/*横向像素需要的偏移量*/
		int   nLineSpace  = 0);	/*纵向像素需要的偏移量*/
		
	/** @brief 将该波段使用指定的数值填充, 相当于初始化
	 *  @param dValue 为指定填充的值, 将自动转换为波段的类型
	 *  @param dImagValue 为图像虚部值, 默认为0.0
	 */
	bool fillBand(double dValue, double dImagValue = 0.0);

	/** @brief 写数据块并处理Nodata数据
	    @note  操作的时候将分配与pData相同的数据块
	    @      用来判断Nodata数据
		@param nXOff x偏移
		@param nYOff y偏移
		@param nXSize x大小
		@param nYSize y大小
		@param pData 目标数据缓存
		@param nBufXSize 目标数据x大小
		@param nBufYSize 目标数据y大小
		@param eBufType  pData数据类型, 必须与band的类型相同
		@param dNodata pData中的Nodata数据,须为double
		@param nPixelSpace 横向像素需要的偏移量
		@param nLineSpace 纵向像素需要的偏移量*/ 
	bool writeWithNodata(
		int    nXOff,
		int    nYOff,
		int    nXSize,
		int    nYSize,
		void  *pData,
		int    nBufXSize,
		int    nBufYSize,
		enumGTDataType eBufType,
		double dNodata,
		int    nPixelSpace = 0,
		int    nLineSpace  = 0);

	/** *@name 读写数据块
	     @note pData必须由外部分配(malloc或者new)，外部释放\n
	     功能同上类似，但可以从金字塔中读取，其中如果nLevel = -1，\n
	     则从原始影像上读取，如果nLevel>=0则从金字塔中读取
		 *@{
	*/
	bool getPixels(int nLevel, int nXOff, int nYOff, int nXSize, void* buffer);
	bool getPixels(int nLevel, int nXOff, int nYOff, int nXSize, int nYSize, void* buffer);
	bool setPixels(int nXOff,  int nYOff, int nXSize, void* buffer);
	bool setPixels(int nXOff,  int nYOff, int nXSize, int nYSize, void* buffer);
	/** @} */

	/** @brief 获取金字塔个数*/
	int getOverviewCount();

	/** @brief 获取第nIndex层的金字塔，返回其GDAL指针, 不需要外部释放*/
	GDALRasterBand* getOverview(int nIndex);

	/** @breif 获取波段像素的size*/
	bool getXYSize(int &xSize, int &ySize);

	/** @breif 获取某个金字塔波段像素的size*/
	bool getOverviewXYSize(const int iOveroiew, int &xSize, int &ySize);

	/** @breif 获取波段的空间分辨率*/
	bool getSpatialResolution(double &xResl, double &yResl) const;

	/** @breif 获取某个金字塔波段的空间分辨率*/
	bool getOverviewSpatialResolution(int iOveroiew, double &xResl, double &yResl) const;

	/** @brief 获取颜色表*/
	GTRasterColorTable* getColorTable();

	/** @brief 设置颜色表*/
	bool setColorTable(GTRasterColorTable* colorTable);
	
	/** @brief 获取栅格属性表
	    @return @ref GTRasterAttributeTable */
	GTRasterAttributeTable* getAttributeTable();
    
	/** @brief 设置栅格属性表 
	    @param attributeTable @ref GTRasterAttributeTable*/
	bool setAttributeTable(GTRasterAttributeTable* attributeTable);

	/*获取nodata*/
	double getNodataValue(int &hasNodata) const;

	/*设置nodata*/
	bool setNodataValue(double dNodata);

	/*创建Mask波段*/
	bool createMaskBand(int iFlag);

protected:
	/** @brief 设置GTRasterDataset的参考指针*/
	void setRasterDatasetRef(GTRasterDataset* poRasterDataset);

protected:
	/*不需要显式释放*/
	GDALRasterBand *gdalRasterBand;

	/*指向Dataset的指针, 不能释放*/
	GTRasterDataset *pRasterDatasetRef;
};

}
#endif
