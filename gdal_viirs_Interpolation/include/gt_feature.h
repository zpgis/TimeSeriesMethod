/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  空间Feature及其相关对象类的定义
* Author:   范俊甫, fanjf@lreis.ac.cn 2010年-11月-29日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTFEATURE_H_INCLUDED
#define GTFEATURE_H_INCLUDED

#include "gt_geometry.h"
#include "gt_spatialreference.h"
#include "../include_p/gdal/ogr_feature.h"

namespace gts{

/**
 *单个属性列的类
 */
class GTS_DLL GTField
{
public:
	//friend class GTFields;
	GTField(const char *fdName, enumGTFieldType fdType);
	~GTField();

	GTField(const GTField& fieldObj);
	GTField(OGRFieldDefn& ogrFieldObj);

	bool operator == (const GTField& fieldObj) const;

	/*设置列属性名字*/
	void setFieldName(const char *fdName);
	
	/*获取列属性名字的指针, 不允许外部释放*/
	const char *getFieldName() const { return fieldName; }

	/*设置列类型*/
	void setFieldType(enumGTFieldType fdType);
	
	/*获取列类型*/
	enumGTFieldType getFieldType() const { return fieldType; }

	/*设置对齐方式*/
	void setJustify(unsigned char uJustify);

	/*获取对齐方式*/
	unsigned char getJustify() const { return nJustify; }

	/*设置宽度*/
	void setWidth(int iWidth);

	/*获取宽度*/
	int getWidth() const { return nWidth; }
	
	/*设置精度*/
	void setPrecision(int iPrecision);
	
	/*获取精度*/
	int getPrecision() const { return nPrecision; }

protected:
	char*           fieldName; /*列的名字*/
	enumGTFieldType fieldType; /*列的类型*/
	int             nWidth;    /*数值型宽度*/
	int             nPrecision;/*数值型精度*/
	unsigned char   nJustify;  /*0 = 未定义, 1 = left, 2 = right*/
};

/**
 *属性列集合类定义
 *对应表中的一行, 对于数据库存储的空间数据来说, 
 *GTFields相当于GTFeatureClass
 */
class GTS_DLL GTFields
{
public:
	GTFields();
	GTFields(const GTFields& fieldsObj);
	/*OGR接口*/
	GTFields(OGRFeatureDefn& ogrFieldDefn);
	~GTFields();

	bool operator == (const GTFields& fieldsObj) const;

	/*释放GTFields的静态成员函数, 必须由new分配内存*/
	static bool destroyFields(GTFields* fieldsObj);
	
	/*增加一个列定义*/
	bool addField(const GTField* fieldObj);
	
	/*获取某一列的指针*/
	GTField* getFieldPtr(int iLocation);
	const GTField* getFieldPtr(int iLocation) const;

	/*获取列的数量*/
	int getFieldCount() const { return fieldCount; }

	/*删除所有列定义, 并释放内存*/
	void clearAllFields();

	/*查询某列位置*/
	int getFieldIndex(const char *fdName) const;

	/*释放一个外部引用计数*/
	void releaseReference() { if(refCount > 0) --refCount; }

	/*增加一个外部引用计数*/
	void setReference() { ++refCount; }

	/*获取外部引用个数*/
	int getReference() const { return refCount; }

protected:
	/*std::vector无法构造, 因此使用二维指针*/
	GTField **fieldList;
	int       fieldCount;
	/*引用计数管理*/
	volatile int refCount;
};

/*
 *要素定义类,包含了属性和特征值, 
 *由于包含了一个指向层的属性定义指针
 *因此在使用的时候必须利用new手动创建对象
 *并且要在层的属性对象释放之前来释放Feature对象
 *利用destroyFeature释放
 */
class GTS_DLL GTFeature
{
public:
	GTFeature();
	~GTFeature();
	GTFeature(const GTFields* filedsObj);
	GTFeature(const GTFeature& featureObj);

	/*静态函数, 供外部调用释放由Layer生成的feature对象*/
	static void destroyFeature(GTFeature *featureObj);

	/*设置属性列参考, 如果已经存在, 直接返回*/
	bool setFieldsRef(GTFields* filedsObj);

	/*取消属性列参考*/
	void unsetFieldsRef();

	/*调用GTS_DELETE释放*/
	GTFeature *clone() const;

	int getFieldCount() const;

	int getFID() const { return iFID; }

	void setFID(int iid); 

	/*几何对象是否为空*/
	bool isGeometryEmpty() const { return poGeometry == 0 || poGeometry->isEmpty(); }

	/*复制传入的几何对象*/
	bool setGeometry(const GTGeometry* geometryObj);

	/*直接将外部new分配的几何对象传入,外部不能释放*/
	bool setGeometryDirectly(GTGeometry* geometryObj, bool deleteGeometry = true);

	/*获取几何对象指针, 严禁外部释放*/
	GTGeometry *getGeometryPtr() { return poGeometry; }
	const GTGeometry *getGeometryPtr() const { return poGeometry; }

	/*获取Field值列表指针*/
	GTFields *getFieldsDefPtr() { return poFields; }
	const GTFields *getFieldsDefPtr() const { return poFields; }

	/*获取Field值列表指针*/
	unionGTField *getFieldValDefPtr() { return poUfValue; }
	const unionGTField *getFieldValDefPtr() const { return poUfValue; }

	/*field是否使用*/
	bool isFieldSet(int iField) const;
	bool _isFieldSet(int iField) const;

	/*取消field使用*/
	void unsetField(int iField);

	/*获取Field定义指针*/
	GTField *getFieldDefPtr(int iField);
	const GTField *getFieldDefPtr(int iField) const;

	/*获取某个属性列的指针(包含具体定义)*/
	unionGTField *getFieldPtrAt(int iField);

	/*获取列的位置*/
	int getFieldIndex(const char* fdName) const;

	/*按照不同类型设置属性列的具体数值
	 *同一类的可以赋值, 否则操作不成功
	 */
	bool setFieldValue(int iField, int         iValue);
	bool setFieldValue(int iField, double      dValue);
	bool setFieldValue(int iField, const char* psValue);
	bool setFieldValue(int iField, const GTFDDate& dateValue);
	bool setFieldValue(int iField, const GTFDTime& timeValue);
	bool setFieldValue(int iField, const GTFDDateTime& dateTimeValue);
	bool setFieldValue(int iField, int blobSize, const unsigned char* blobValue);
	bool setFieldValue(int iField, const unionGTField *fieldValue);

	/*取出列的数据*/
	bool getFieldAsInt(int iField, int& iValue) const;
	bool getFieldAsDouble(int iField, double& dValue) const;

	/*初始化char* pstr=0, 传入&pstr, 外部调用GTS_FREE(pstr)释放*/
	bool getFieldAsString(int iField, char **psValue) const;
	
	bool getFieldAsDate(int iField, GTFDDate& dateValue) const;
	bool getFieldAsTime(int iField, GTFDTime& timeValue) const;
	bool getFieldAsDateTime(int iField, GTFDDateTime& dateTimeValue) const;
	
	/*初始化char* pbina=0, 传入&pbina, 外部调用GTS_FREE(pbina)释放*/
	bool getFieldAsBinary(int iField, int& blobSize, unsigned char** blobValue) const;

	enumGTGeometryType getGeometryType() const {
		return poGeometry ? poGeometry->getGeometryType() : GT_Geometry_Unknown;
	}

	const unionGTField *getUnionValues() const { return poUfValue; }

protected:
	/*对象唯一标识*/
	int iFID;
	/*几何数据指针*/
	GTGeometry *poGeometry;
	bool shouldDeleteGeometry;
	
	/*属性列定义指针, 具体数据没有使用
	 *并且同一个层里的所有Feature都引用同一个定义指针 
	 *因此不能由本对象释放, 原指针由层维护*/
	GTFields   *poFields;
	
	/*属性数据列表*/
	unionGTField *poUfValue;
};

/*
 *FeatureClass定义类, 实际是一个GTFeature的集合
 *包含Envelop, SpatialRerefence等信息
 *FeatureClass由FeatureLayer通过条件生成
 */
class GTS_DLL GTFeatureClass
{
public:
	friend class GTFeatureLayer;

	GTFeatureClass();
	~GTFeatureClass();
	int getFeatureCount() const { return (int)feature_list.size(); }
	void clearAll();

	/*pFeature一定是外部由new分配的指针*/
	/*pFeature加入后由FeatureClass管理, 外部不能释放*/
	/*如果加入失败, pFeature必须由外部释放*/
	bool addFeature(GTFeature* pFeature);

	/*外部不能释放其指针*/
	GTFeature *getFeatureAt(int atLocation) {
		return (atLocation >= 0 && atLocation < getFeatureCount()) ? feature_list[atLocation] : 0;
	}
	const GTFeature *getFeatureAt(int atLocation) const {
		return (atLocation >= 0 && atLocation < getFeatureCount()) ? feature_list[atLocation] : 0;
	}
	void deleteFeatureAt(int atLocation);
	void updateEnvelop();
	GTEnvelop getEnvelop() const { return envelop; }
	void setSpatialReference(GTSpatialReference *spatialRef);
	GTSpatialReference *getSpatialReference() { return poSpatialRef; }
	void setGeometryType(enumGTWKBGeometryType geoType) { wkb_type = geoType; }
	enumGTWKBGeometryType getGeometryType() const { return wkb_type; }

protected:
	std::vector<GTFeature *> feature_list;
	//int featureCount;	// by zjw
	GTEnvelop envelop;
	GTSpatialReference *poSpatialRef;
	enumGTWKBGeometryType wkb_type;
};

}
#endif
