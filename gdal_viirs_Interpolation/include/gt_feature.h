/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  �ռ�Feature������ض�����Ķ���
* Author:   ������, fanjf@lreis.ac.cn 2010��-11��-29��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
****************************************************************/
#pragma once
#ifndef GTFEATURE_H_INCLUDED
#define GTFEATURE_H_INCLUDED

#include "gt_geometry.h"
#include "gt_spatialreference.h"
#include "../include_p/gdal/ogr_feature.h"

namespace gts{

/**
 *���������е���
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

	/*��������������*/
	void setFieldName(const char *fdName);
	
	/*��ȡ���������ֵ�ָ��, �������ⲿ�ͷ�*/
	const char *getFieldName() const { return fieldName; }

	/*����������*/
	void setFieldType(enumGTFieldType fdType);
	
	/*��ȡ������*/
	enumGTFieldType getFieldType() const { return fieldType; }

	/*���ö��뷽ʽ*/
	void setJustify(unsigned char uJustify);

	/*��ȡ���뷽ʽ*/
	unsigned char getJustify() const { return nJustify; }

	/*���ÿ��*/
	void setWidth(int iWidth);

	/*��ȡ���*/
	int getWidth() const { return nWidth; }
	
	/*���þ���*/
	void setPrecision(int iPrecision);
	
	/*��ȡ����*/
	int getPrecision() const { return nPrecision; }

protected:
	char*           fieldName; /*�е�����*/
	enumGTFieldType fieldType; /*�е�����*/
	int             nWidth;    /*��ֵ�Ϳ��*/
	int             nPrecision;/*��ֵ�;���*/
	unsigned char   nJustify;  /*0 = δ����, 1 = left, 2 = right*/
};

/**
 *�����м����ඨ��
 *��Ӧ���е�һ��, �������ݿ�洢�Ŀռ�������˵, 
 *GTFields�൱��GTFeatureClass
 */
class GTS_DLL GTFields
{
public:
	GTFields();
	GTFields(const GTFields& fieldsObj);
	/*OGR�ӿ�*/
	GTFields(OGRFeatureDefn& ogrFieldDefn);
	~GTFields();

	bool operator == (const GTFields& fieldsObj) const;

	/*�ͷ�GTFields�ľ�̬��Ա����, ������new�����ڴ�*/
	static bool destroyFields(GTFields* fieldsObj);
	
	/*����һ���ж���*/
	bool addField(const GTField* fieldObj);
	
	/*��ȡĳһ�е�ָ��*/
	GTField* getFieldPtr(int iLocation);
	const GTField* getFieldPtr(int iLocation) const;

	/*��ȡ�е�����*/
	int getFieldCount() const { return fieldCount; }

	/*ɾ�������ж���, ���ͷ��ڴ�*/
	void clearAllFields();

	/*��ѯĳ��λ��*/
	int getFieldIndex(const char *fdName) const;

	/*�ͷ�һ���ⲿ���ü���*/
	void releaseReference() { if(refCount > 0) --refCount; }

	/*����һ���ⲿ���ü���*/
	void setReference() { ++refCount; }

	/*��ȡ�ⲿ���ø���*/
	int getReference() const { return refCount; }

protected:
	/*std::vector�޷�����, ���ʹ�ö�άָ��*/
	GTField **fieldList;
	int       fieldCount;
	/*���ü�������*/
	volatile int refCount;
};

/*
 *Ҫ�ض�����,���������Ժ�����ֵ, 
 *���ڰ�����һ��ָ�������Զ���ָ��
 *�����ʹ�õ�ʱ���������new�ֶ���������
 *����Ҫ�ڲ�����Զ����ͷ�֮ǰ���ͷ�Feature����
 *����destroyFeature�ͷ�
 */
class GTS_DLL GTFeature
{
public:
	GTFeature();
	~GTFeature();
	GTFeature(const GTFields* filedsObj);
	GTFeature(const GTFeature& featureObj);

	/*��̬����, ���ⲿ�����ͷ���Layer���ɵ�feature����*/
	static void destroyFeature(GTFeature *featureObj);

	/*���������вο�, ����Ѿ�����, ֱ�ӷ���*/
	bool setFieldsRef(GTFields* filedsObj);

	/*ȡ�������вο�*/
	void unsetFieldsRef();

	/*����GTS_DELETE�ͷ�*/
	GTFeature *clone() const;

	int getFieldCount() const;

	int getFID() const { return iFID; }

	void setFID(int iid); 

	/*���ζ����Ƿ�Ϊ��*/
	bool isGeometryEmpty() const { return poGeometry == 0 || poGeometry->isEmpty(); }

	/*���ƴ���ļ��ζ���*/
	bool setGeometry(const GTGeometry* geometryObj);

	/*ֱ�ӽ��ⲿnew����ļ��ζ�����,�ⲿ�����ͷ�*/
	bool setGeometryDirectly(GTGeometry* geometryObj, bool deleteGeometry = true);

	/*��ȡ���ζ���ָ��, �Ͻ��ⲿ�ͷ�*/
	GTGeometry *getGeometryPtr() { return poGeometry; }
	const GTGeometry *getGeometryPtr() const { return poGeometry; }

	/*��ȡFieldֵ�б�ָ��*/
	GTFields *getFieldsDefPtr() { return poFields; }
	const GTFields *getFieldsDefPtr() const { return poFields; }

	/*��ȡFieldֵ�б�ָ��*/
	unionGTField *getFieldValDefPtr() { return poUfValue; }
	const unionGTField *getFieldValDefPtr() const { return poUfValue; }

	/*field�Ƿ�ʹ��*/
	bool isFieldSet(int iField) const;
	bool _isFieldSet(int iField) const;

	/*ȡ��fieldʹ��*/
	void unsetField(int iField);

	/*��ȡField����ָ��*/
	GTField *getFieldDefPtr(int iField);
	const GTField *getFieldDefPtr(int iField) const;

	/*��ȡĳ�������е�ָ��(�������嶨��)*/
	unionGTField *getFieldPtrAt(int iField);

	/*��ȡ�е�λ��*/
	int getFieldIndex(const char* fdName) const;

	/*���ղ�ͬ�������������еľ�����ֵ
	 *ͬһ��Ŀ��Ը�ֵ, ����������ɹ�
	 */
	bool setFieldValue(int iField, int         iValue);
	bool setFieldValue(int iField, double      dValue);
	bool setFieldValue(int iField, const char* psValue);
	bool setFieldValue(int iField, const GTFDDate& dateValue);
	bool setFieldValue(int iField, const GTFDTime& timeValue);
	bool setFieldValue(int iField, const GTFDDateTime& dateTimeValue);
	bool setFieldValue(int iField, int blobSize, const unsigned char* blobValue);
	bool setFieldValue(int iField, const unionGTField *fieldValue);

	/*ȡ���е�����*/
	bool getFieldAsInt(int iField, int& iValue) const;
	bool getFieldAsDouble(int iField, double& dValue) const;

	/*��ʼ��char* pstr=0, ����&pstr, �ⲿ����GTS_FREE(pstr)�ͷ�*/
	bool getFieldAsString(int iField, char **psValue) const;
	
	bool getFieldAsDate(int iField, GTFDDate& dateValue) const;
	bool getFieldAsTime(int iField, GTFDTime& timeValue) const;
	bool getFieldAsDateTime(int iField, GTFDDateTime& dateTimeValue) const;
	
	/*��ʼ��char* pbina=0, ����&pbina, �ⲿ����GTS_FREE(pbina)�ͷ�*/
	bool getFieldAsBinary(int iField, int& blobSize, unsigned char** blobValue) const;

	enumGTGeometryType getGeometryType() const {
		return poGeometry ? poGeometry->getGeometryType() : GT_Geometry_Unknown;
	}

	const unionGTField *getUnionValues() const { return poUfValue; }

protected:
	/*����Ψһ��ʶ*/
	int iFID;
	/*��������ָ��*/
	GTGeometry *poGeometry;
	bool shouldDeleteGeometry;
	
	/*�����ж���ָ��, ��������û��ʹ��
	 *����ͬһ�����������Feature������ͬһ������ָ�� 
	 *��˲����ɱ������ͷ�, ԭָ���ɲ�ά��*/
	GTFields   *poFields;
	
	/*���������б�*/
	unionGTField *poUfValue;
};

/*
 *FeatureClass������, ʵ����һ��GTFeature�ļ���
 *����Envelop, SpatialRerefence����Ϣ
 *FeatureClass��FeatureLayerͨ����������
 */
class GTS_DLL GTFeatureClass
{
public:
	friend class GTFeatureLayer;

	GTFeatureClass();
	~GTFeatureClass();
	int getFeatureCount() const { return (int)feature_list.size(); }
	void clearAll();

	/*pFeatureһ�����ⲿ��new�����ָ��*/
	/*pFeature�������FeatureClass����, �ⲿ�����ͷ�*/
	/*�������ʧ��, pFeature�������ⲿ�ͷ�*/
	bool addFeature(GTFeature* pFeature);

	/*�ⲿ�����ͷ���ָ��*/
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
