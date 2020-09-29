/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ֲ�Ķ�����
* Author:   ������, fanjf@lreis.ac.cn 2010��-12��-13��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
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
 *�㶨��ĳ�����, ����������Դ�Ϳռ���������
 */
class GTS_DLL GTLayer
{
public:
	GTLayer(void);
	virtual ~GTLayer(void);

	/*��ȡ�������*/
	virtual enumGTLayerType getLayerType() const = 0;

	/*��ȡ���Envelop*/
	virtual GTEnvelop getLayerEnvelop() const = 0;

	/*��ȡ���ݸ�ʽ��������*/
	virtual const char *getDataDriverName() const = 0;
	
	/*��ȡ����Դ��Ϣ(�ļ�·���Լ����ݿ����Ӵ�)*/
	virtual const char *getDataSourceName() const = 0;
	
	/*���DadaSourceָ��*/
	virtual void releaseDataSourceRefPtr() = 0;

	/*����DadaSourceָ��*/
	virtual GTDataSource* getDataSourceRefPtr() = 0;

	/*��ȡ��Ŀռ�������Ϣָ��*/
	virtual GTSpatialReference *getLayerSpatialRefPtr() = 0;
	
	/*��ȡ����*/
	virtual const char *getLayerName() const = 0;
	/*��ȡ�����*/
	virtual const char *getLayerAlias() const = 0;
	/*��������Ա�ڴ�*/
	virtual void clearAll() = 0;
	/*��ȡ��������Դ����*/
	virtual enumGTDataDriverType getDataDriverType() const = 0;

	/*����˽�к���ֻ����Գ�Ա�����ĸ�ֵ����, ������Դ�޹�*/
protected:
	/*���ò��Envelop*/
	virtual void setLayerEnvelop(const GTEnvelop& envelop) = 0;

	/*�趨DadaSourceָ��*/
	virtual void setDataSourceRefPtr(GTDataSource* pDataSource) = 0;

	/*�趨��Ŀռ�������Ϣ, �ڲ�����, �ڲ��ͷ�*/
	virtual bool setLayerSpatialRef(GTSpatialReference *spatialRef) = 0;

	/*�������ݸ�ʽ��������*/
	virtual void setDataDriverName(const char *driverName) = 0;
	
	/*��������Դ��Ϣ*/
	virtual void setDataSourceName(const char *sourceName) = 0;

	/*���ò���*/
	virtual void setLayerName(const char *lName) = 0;

	/*���ò����*/
	virtual void setLayerAlias(const char *lAlias) = 0;

	/*������������Դ����*/
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

/** @name ��GTLayer�̳�, ���������Դ�Ĳ���
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
	/** @name ���г�Ա����
      *@{*/

	/** @brief ��̬���һ��GTFeatureLayer����ĺ���*/
	static bool destroyFeatureLayer(GTFeatureLayer *poFeatureLayer);

	/** @brief �Ƿ�Ϊ����ģʽ��*/
	bool isUpdateMode() const { return isUpdated; }

	/** @brief ������Դ������������������*/
	bool updateFieldsDefnRef();
	
	/** @brief ��ȡ��������������ָ��*/
	GTFields *getFieldsDefnPtr() { return fieldsDefnRef; }
	const GTFields *getFieldsDefnPtr() const { return fieldsDefnRef; }

	/** @brief ��ȡ�������*/
	int getFeatureCount() const { return nFeatureCount; }

	/** @brief ��ȡFeature����, iFeatureΪFID*/
	GTFeature *getFeature(int iFID);

	/** @brief ֻ��ȡ��������
	  * @note �ⲿ����GTS_DELETE�ͷ�*/
	GTGeometry *getGeometry(int iFID);

	/** @brief ��ȡ��һ��Feature����*/
	GTFeature *getNextFeature();

	/** @brief ��������Feature�����α�*/
	void resetReading();

	/** @brief ɾ��Feature������Դ*/
	bool deleteFeature(
		int  iFID,
		bool bForce = false);

	/** @brief ����Feature������Դ*/
	bool createFeature(GTFeature *pFeature, bool bUpdateEnv);

	/** @brief ����Feature������Դ*/
	bool setFeature(GTFeature *pFeature, bool bUpdateEnv);

	/** @brief ɾ��һϵ��Feature������Դ*/
	bool deleteFeatures(
		const std::vector<int>& fid_list, 
		bool                    bLock = true);

	/** @brief ����һϵ��Feature������Դ*/
	bool createFeatures(
		GTFeatureClass& fClass, 
		bool            bLock = true);

	/** @brief ����һϵ��Feature������Դ*/
	bool setFeatures(
		GTFeatureClass& fClass, 
		bool            bLock = true);

	/** @brief ��ȡ��ļ���������Ϣ
	  * @note ע��polygon ��multipolygon������һ��*/
	enumGTWKBGeometryType getGeometryType() const { return eGeometryType; }

	/** @brief ������Դ�ϴ����ռ�����
	    @return ��֧�ֻ�ʧ�ܷ���false*/
	bool createSpatialIndex();

	/** @brief ������Դ��ɾ���ռ�����
	    @return ��֧�ֻ�ʧ�ܷ���false*/
	bool deleteSpatialIndex();

	/** @brief ��������еı༭,���������е�����Դ,
	    @note ��ʱfieldsDefnRef������Ч, ��Ҫ���»�ȡ\n
	    �ڴ˲���֮ǰӦ���ͷ����е���֮������Feature��FeatureClass\n
	    ��ɲ���֮��, ���Զ�����fieldsDefnRef*/
	bool createField(const GTField* fieldDef);

	/** @brief ��������ɾ�������е�����Դ
	    @note ��ʱfieldsDefnRef������Ч, ��Ҫ���»�ȡ\n
	    �ڴ˲���֮ǰӦ���ͷ����е���֮������Feature��FeatureClass\n
	    ��ɲ���֮��, ���Զ�����fieldsDefnRef*/
	bool deleteField(const char* fdName);

	/** @brief ���������е�����������Դ*/
	bool createFieldIndex(const char* fdName);

	/** @brief ɾ�������е�����������Դ*/
	bool deleteFieldIndex(const char* fdName);

	/** @brief ���²��������Դ���ڴ��Envelop 
	    @note ��Ҫ������Դ�������м��ζ���*/
	void updateEnvelop();

	/** @brief ��ȡĳ�е����ֵ���е����ͱ���Ϊ����**/
	bool getFieldMaxValue(const char *fdName, double& dValue) const;

	/** @brief ��ȡĳ�е���Сֵ���е����ͱ���Ϊ����**/
	bool getFieldMinValue(const char *fdName, double& dValue) const;

	/** @brief ��ȡĳ�е�ƽ��ֵ���е����ͱ���Ϊ����**/
	bool getFieldAvgValue(const char *fdName, double& dValue) const;

	/** @brief ��ȡĳ�еı�׼ƫ��е����ͱ���Ϊ����**/
	bool getFieldStdValue(const char *fdName, double& dValue) const;

	/** @brief ��ȡĳ�е���ֵ�ͣ��е����ͱ���Ϊ����**/
	bool getFieldSumValue(const char *fdName, double& dValue) const;

	/** @brief ����������ѯ 
	  * @note ����ӦΪSQL��WHERE��������ﲿ��, ��ҪԤ�������ַ���, ���Ϊ��, ��ѡȡȫ������
	  * @return ����-1�����д�����, ���򷵻ض������, �����fClass��\n
	    @note FeatureClass��ΪFeatureָ�뼯��, ��������FeatureLayer��Fieldsָ��\n
	    ��Ҫ��FeatrueLayer֮ǰ�ͷ�, ����clearAll��Ա����*/
	int getFeaturesByAttributeFilter(
		const char*     sqlFilter, 
		GTFeatureClass& fClass,
		bool bOrderByFID = false);

	/** @brief ����������ѯ, ֻ���ؼ��ζ���, ע��ָ��������ͷ�*/
	int getFeaturesByAttributeFilter(
		const char*                sqlFilter,
		std::vector<GTGeometry *>& geom_list,
		std::vector<int>&          fid_list,
		bool bOrderByFID = false);

	/** @brief ����������ѯ, ֻ����id*/
	int getFeaturesByAttributeFilter(
		const char*       sqlFilter,
		std::vector<int>& fid_list);

	/** @brief Envelopλ��������ѯ
	    @note ����û������OGR���ļ�Ӧ��Ч�ʽϵ�, Ŀǰ����OGRֻʵ��Intersect��ϵ\n
	    ֻ���ݼ��ζ����Envelop��ϵ���в�ѯ
	    @return ����-1�����д�����, ���򷵻ض������,�����fClass��
	    @note FeatureClass��ΪFeatureָ�뼯��, ��������FeatureLayer��Fieldsָ��\n
	    ��Ҫ��FeatrueLayer֮ǰ�ͷ�, ����clearAll��Ա����*/
	int getFeaturesByEnvelopFilter(
		const GTEnvelop&             gtEnvelopFilter, 
		enumGTSpatialRelation        eSR, 
		GTFeatureClass&              fClass,
		bool bOrderByFID = false);

	/** @brief Envelopλ��������ѯ
	    @note ֻ���ؼ��ζ���, ע��ָ��������ͷ�*/
	int getFeaturesByEnvelopFilter(
		const GTEnvelop&             gtEnvelopFilter,
		enumGTSpatialRelation        eSR,
		std::vector<GTGeometry *>&   geom_list,
		std::vector<int>&            fid_list,
		bool bOrderByFID = false);
	
	/** @brief Envelopλ��������ѯ
	    @note ֻ����id*/
	int getFeaturesByEnvelopFilter(
		const GTEnvelop&             gtEnvelopFilter,
		enumGTSpatialRelation        eSR,
		std::vector<int>&            fid_list);

	/** @brief ����λ��������ѯ
	    @note ֻ���ݼ��ζ���Ŀռ��ϵ���в�ѯ
	    @return ����-1�����д�����, ���򷵻ض������
	    @note FeatureClass��ΪFeatureָ�뼯��, ��������FeatureLayer��Fieldsָ��\n
	    ��Ҫ��FeatrueLayer֮ǰ�ͷ�, ����clearAll��Ա����*/
	int getFeaturesByGeometryFilter(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		GTFeatureClass&              fClass,
		bool bOrderByFID = false);

	/** @brief ����λ��������ѯ, ֻ���ؼ��ζ���, ע��ָ��������ͷ�*/
	int getFeaturesByGeometryFilter(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		std::vector<GTGeometry *>&   geom_list,
		std::vector<int>&            fid_list,
		bool bOrderByFID = false);

	/** @brief ����λ��������ѯ, ֻ����id*/
	int getFeaturesByGeometryFilter(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		std::vector<int>&            fid_list);

	/** @brief ���������ѯ, ֻ����FeatureClass*/
	int getFeaturesByConditions(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		const char*                  sqlFilter,
		GTFeatureClass&              fClass,
		bool bOrderByFID = false);

	/** @brief ���������ѯ, ֻ���ؼ�������, ע������ָ���ͷ�*/
	int getFeaturesByConditions(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
		const char*                  sqlFilter,
		std::vector<GTGeometry *>&   geom_list,
		std::vector<int>&            fid_list,
		bool bOrderByFID = false);

	/** @brief ���������ѯ, ֻ����id*/
	int getFeaturesByConditions(
		const GTGeometry*            poGeometry,
		enumGTSpatialRelation        eSR,
	    const char*                  sqlFilter,
		std::vector<int>&            fid_list);

	/** @brief ��ȡOGRLayer��ָ��, ֻ���GTOGRDataSource��Ч, ��������Դ��Ϊ��*/
	OGRLayer *getOGRLayerRef() { return ogrLayer; }
	/** @} */

	/** @brief ���¶�����ͶӰ��Ϣ������Դ*/
	bool resetSpatialReference(GTSpatialReference *pSpatialRef);

	/** @brief ����MySQL���������ļ�, ����"LOAD DATA LOCAL INFILE���ٵ������"*/
	bool exportToMySQLDataFile(const char* pszFileName);

	const char* getErrMsg() const;
	int			getErrNo() const { return err_no; }
	void		clearErrNo() { err_no = GT_DS_SUCCESS; }

protected:
	GTFields              *fieldsDefnRef;
	int                    nFeatureCount;
	enumGTWKBGeometryType  eGeometryType;

	/*�����ȡ�α�, ��getNextFeature��resetRead������
	 *��ȡ���һ�������, ��������Ϊ��ʼ��0
	 */
	int nFeatureRead;

	/*OGRLayerָ��, ֻ���GTOGRDataSource��Ч, ��������Դ��Ϊ��*/
	OGRLayer *ogrLayer;

	bool  isUpdated; /*�Ƿ���±�־*/

	int          err_no;
	std::string  err_msg;        /*������Ϣ*/

protected:
	/*���ø��±�־*/
	void setUpdated(bool bUpdate) { isUpdated = bUpdate; }

	/** @brief ������������������*/
	bool setFieldsDefnRef(const GTFields* gtFieldsDef);

	/** @brief ���ö������
	    @note ֻ���ڴ����, ������Դ�޹�*/
	void setFeatureCount(unsigned int fObjCount) { nFeatureCount = fObjCount; }

	/** @brief ���ò�ļ���������Ϣ*/
	void setGeometryType(enumGTWKBGeometryType  eGeomType);


/*����Ϊ˽�к���, ��Ӧ��ͬDataSource���͵ķֱ�ʵ��, 
 *ʵ�ֺ����ڸ��Զ�Ӧ��cpp��
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
 *GTTinLayer������
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

	/*���º�����TinLayer�йر�*/
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

/*AttributeTableֱ��ʹ��GDAL����̳�, ��Ҫ�޸�*/
class GTS_DLL GTRasterAttributeTable : public GDALRasterAttributeTable
{
public:
	GTRasterAttributeTable();
	~GTRasterAttributeTable();
};

/** *@brief GTRasterBand��*/
class GTS_DLL GTRasterBand
{
public:
	friend class GTRasterDataset;
	GTRasterBand();
	~GTRasterBand();

	/** @brief ���GTRasterBandָ��ľ�̬����*/
	static void destroyGTRasterBand(GTRasterBand* poRasterBand);

	/** @brief ֱ�ӻ�ȡGDALRasterBand�Ĳ���ָ��, �ⲿ����ͨ����ָ�����GDAL�������в���*/
	GDALRasterBand *getGDALRasterBandRef() { return gdalRasterBand; }
	
	/** @name ������дGDALRasterBand���ֺ���
      * @{*/

	/** @brief ��ȡMetadata, �ⲿ�����ͷ�
	  * @note domainNameΪ��, �򷵻�metadataĿ¼\n
	  ����ָ��ͨ��GTString::stringCount��ȡ��ĸ���*/
	const char **getMetadata(const char* domainName) const;

	/** @brief ��ȡCategory Name*/
	const char **getCategoryNames() const;

	/** @brief ��ȡ��������*/
	enumGTDataType getDataType() const;

	/** @brief ��ȡ��λ*/
	const char* getUnitType();

	/** @brief ��д���ݿ�
	    @note pData�������ⲿ����(malloc����new)���ⲿ�ͷ�
		@param bIsReader �Ƿ�д
		@param nXOff xƫ��
		@param nYOff yƫ��
		@param nXSize x��С
		@param nYSize y��С
		@param pData Ŀ�����ݻ���
		@param nBufXSize Ŀ������x��С
		@param nBufYSize Ŀ������y��С
		@param eBufType ��������
		@param nPixelSpace ����������Ҫ��ƫ����
		@param nLineSpace ����������Ҫ��ƫ����*/ 
	bool rasterIO(
		bool  bRead,            /*�Ƿ�д*/
		int   nXOff,            /*xƫ��*/
		int   nYOff,            /*yƫ��*/
		int   nXSize,           /*x��С*/
		int   nYSize,           /*y��С*/
		void *pData,            /*Ŀ�����ݻ���*/
		int   nBufXSize,        /*Ŀ������x��С*/
		int   nBufYSize,        /*Ŀ������y��С*/
		enumGTDataType eBufType,/*��������*/
		int   nPixelSpace = 0,	/*����������Ҫ��ƫ����*/
		int   nLineSpace  = 0);	/*����������Ҫ��ƫ����*/
		
	/** @brief ���ò���ʹ��ָ������ֵ���, �൱�ڳ�ʼ��
	 *  @param dValue Ϊָ������ֵ, ���Զ�ת��Ϊ���ε�����
	 *  @param dImagValue Ϊͼ���鲿ֵ, Ĭ��Ϊ0.0
	 */
	bool fillBand(double dValue, double dImagValue = 0.0);

	/** @brief д���ݿ鲢����Nodata����
	    @note  ������ʱ�򽫷�����pData��ͬ�����ݿ�
	    @      �����ж�Nodata����
		@param nXOff xƫ��
		@param nYOff yƫ��
		@param nXSize x��С
		@param nYSize y��С
		@param pData Ŀ�����ݻ���
		@param nBufXSize Ŀ������x��С
		@param nBufYSize Ŀ������y��С
		@param eBufType  pData��������, ������band��������ͬ
		@param dNodata pData�е�Nodata����,��Ϊdouble
		@param nPixelSpace ����������Ҫ��ƫ����
		@param nLineSpace ����������Ҫ��ƫ����*/ 
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

	/** *@name ��д���ݿ�
	     @note pData�������ⲿ����(malloc����new)���ⲿ�ͷ�\n
	     ����ͬ�����ƣ������Դӽ������ж�ȡ���������nLevel = -1��\n
	     ���ԭʼӰ���϶�ȡ�����nLevel>=0��ӽ������ж�ȡ
		 *@{
	*/
	bool getPixels(int nLevel, int nXOff, int nYOff, int nXSize, void* buffer);
	bool getPixels(int nLevel, int nXOff, int nYOff, int nXSize, int nYSize, void* buffer);
	bool setPixels(int nXOff,  int nYOff, int nXSize, void* buffer);
	bool setPixels(int nXOff,  int nYOff, int nXSize, int nYSize, void* buffer);
	/** @} */

	/** @brief ��ȡ����������*/
	int getOverviewCount();

	/** @brief ��ȡ��nIndex��Ľ�������������GDALָ��, ����Ҫ�ⲿ�ͷ�*/
	GDALRasterBand* getOverview(int nIndex);

	/** @breif ��ȡ�������ص�size*/
	bool getXYSize(int &xSize, int &ySize);

	/** @breif ��ȡĳ���������������ص�size*/
	bool getOverviewXYSize(const int iOveroiew, int &xSize, int &ySize);

	/** @breif ��ȡ���εĿռ�ֱ���*/
	bool getSpatialResolution(double &xResl, double &yResl) const;

	/** @breif ��ȡĳ�����������εĿռ�ֱ���*/
	bool getOverviewSpatialResolution(int iOveroiew, double &xResl, double &yResl) const;

	/** @brief ��ȡ��ɫ��*/
	GTRasterColorTable* getColorTable();

	/** @brief ������ɫ��*/
	bool setColorTable(GTRasterColorTable* colorTable);
	
	/** @brief ��ȡդ�����Ա�
	    @return @ref GTRasterAttributeTable */
	GTRasterAttributeTable* getAttributeTable();
    
	/** @brief ����դ�����Ա� 
	    @param attributeTable @ref GTRasterAttributeTable*/
	bool setAttributeTable(GTRasterAttributeTable* attributeTable);

	/*��ȡnodata*/
	double getNodataValue(int &hasNodata) const;

	/*����nodata*/
	bool setNodataValue(double dNodata);

	/*����Mask����*/
	bool createMaskBand(int iFlag);

protected:
	/** @brief ����GTRasterDataset�Ĳο�ָ��*/
	void setRasterDatasetRef(GTRasterDataset* poRasterDataset);

protected:
	/*����Ҫ��ʽ�ͷ�*/
	GDALRasterBand *gdalRasterBand;

	/*ָ��Dataset��ָ��, �����ͷ�*/
	GTRasterDataset *pRasterDatasetRef;
};

}
#endif
