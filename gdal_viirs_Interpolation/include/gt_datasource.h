/****************************************************************
 *
 * Project:  ������Ϣ�����߼�
 * Purpose:  ����Դ�Ķ�����
 * Author:   ������, fanjf@lreis.ac.cn 2010��-12��-13��
 *
 ****************************************************************
 * Copyright (c) All rights reserved.
 * ��Ȩ����  (c) ��������Ȩ��
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
	/*�������ⲿ�ͷ�*/
	virtual const char* getDataDriverName() const { return dataDriverName; }
	virtual const char* getDataSourceName() const { return dataSourceName; }

	/*��һ������Դ������*/
	virtual bool openDataSource(const char* pSourceName, bool bUpdate) = 0;

	/*��������Դ����, ���´�*/
	virtual bool reopenDataSource(bool bUpdate) = 0;

	/*�ر�����Դ����*/
	virtual void closeDataSource() = 0;

	/*�������Դ����*/
	virtual void clearDataSource() = 0;

	/*����Դ�����Ƿ�*/
	virtual bool isDataSourceOpen() const = 0;

	/*����Դ�����Ƿ���Ч*/
	virtual bool isDataSourceValid() const = 0;

	void releaseReference();
	void setReference();
	int  getReference() const;

protected:
	int   refCount;

	/*�������Դ·�������ݿ���schema*/
	char* dataSourceName;
	
	/*����Դ����*/
	char* dataDriverName;
};

/*
 *GTRasterDataset�Ľӿ���
 */
class GTS_DLL GTRasterDataset : public GTDataSource
{
public:
	friend class GTGDALDataDriver;
	friend class GTGDOSMySQLDataSource;

	GTRasterDataset(void);
	~GTRasterDataset(void);

	/*��GTDataSource�̳�*/
	/*������Ҫע�����, ��ʱ���º���ֱ�����GDAL����Դ(�ļ���)���в���*/
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

	/*���г�Ա����*/
	/*����ָ��GTDataSource��ָ��*/
	void releaseDataSourceRefPtr();
	GTDataSource* getDataSourceRefPtr();

	/*�Ƿ����Ӽ�*/
	bool hasSubDatasets() const;

	/*��ȡ�Ӽ���Ŀ*/
	int getSubDatasetsCount() const;

	/*��ȡ�����ĸ�����Դ������*/
	enumGTDataDriverType getDataSourceDriverType() const;

	/*����һ��GTRasterDatasetָ��*/
	static bool destroyGTRasterDataset(GTRasterDataset *pGtRasterDataset);
	
	/*��ȡGDALDatasetָ��, �ⲿ�������ͷ�*/
	GDALDataset *getGDALDatasetRef();
	
	/*��ȡ�ռ�ο�ָ��*/
	GTSpatialReference *getSpatialRefPtr();
	
	/*���ÿռ�ο�����ʵ����Դ, �������ڴ�ռ�ο�, 
	 *����ռ�ο�����, ��������Դ����д���߲�֧��, �򷵻ش���
	 *������³ɹ�, ��ʵ����Ҳû�о���ͶӰת��
	 */
	bool setDataSourceSpatialRef(GTSpatialReference *spatialRef);

	/*��ȡ����任����, �ò������������е�ͶӰת���Ĳ���
	 *padfTransform��Ҫ��ǰ����ռ�
	 */
	bool getGeomTransform(double *padfTransform);

	/*�Ƿ�����ת��Ӱ��*/
	bool isNorthUp();

	/*��ȡ��ת�ĵĻ���, ����Ϊ0, ˳ʱ��Ϊ��, ��ʱ��Ϊ��*/
	bool getRotateAngle(double &rtAngle);

	/*��������Դ�ķ���ת������*/
	bool setDataSourceGeomTransform(double *padfTransform);

	/*��ȡ������任�Ĳ���, �ò���������ͶӰ���굽����ת���Ĳ���
	 *6������padfInTransform��Ҫ��ǰ����ռ�
	 */
	bool getInverseGeomTransform(double *padfInTransform);

	/*��ȡGCP�ĸ���*/
	int getGCPCount();

	/*��ȡGCP�ṹָ��*/
	const GDAL_GCP* getGCPs();

	/*��GCPsת����Ϊ����任����
	 *6������padfInTransform��Ҫ��ǰ����ռ�
	 */
	bool gcpsToGeotransform(
		int             gcpCount, 
		const GDAL_GCP *pasGCPs, 
		double         *padfInTransform);

	/***********************************/
	/*������Ҫ��д����GDALDataset��Ա����*/
	/***********************************/

	/*��ȡMetadata, �ⲿ�����ͷ�*/
	/*domainNameΪ��, �򷵻�metadataĿ¼*/
	/*domainNameͨ��Ϊ: 
	 "IMAGE_STRUCTURE", "SUBDATASETS"
	 "GEOLOCATION", "RPC"��
	 *����ָ��ͨ��GTString::stringCount��ȡ��ĸ���
	 */
	const char **getMetadata(const char* domainName) const;

	/*��ȡRasterBand��Ŀ, -1Ϊ�д�����*/
	int getRasterBandCount() const;

	/*��ȡĳһ�����ε���������, ��1��ʼ����*/
	enumGTDataType getRasterBandDataType(int iBandNum) const;

	/*��ȡ���ص����з�ʽ*/
	enumGTPixelInterleave getPixelInterleave() const;

	/*��ȡ����*/
	int getXSize() const;

	/*��ȡ����*/
	int getYSize() const;

	/*��ȡ�ռ䷶Χ, ����non-north up��Ӱ��, 
	 *��������ת��ķ�Χ, ������ԭʼ�ķ�Χ
	 *ԭʼ�ķ�Χ������getOrgSpatialExtent����*/
	bool getSpatialExtent(GTEnvelop& envelop) const;

	/*
	 *����non-north up��Ӱ�������Ӱ������ĸ��ǵ������ 
	 */
	bool getOrgSpatialExtent(
		GTRawPoint& pt_leftTop, 
		GTRawPoint& pt_leftBottom,
		GTRawPoint& pt_rightTop,
		GTRawPoint& pt_rightBottom) const;

	/*Units value = (raw value * scale) + offset*/
	/*��ȡScale*/
	double getBandValueScale(int iBandNum) const;
	
	/*��ȡVaule Unit*/
	const char* getBandValueUnitType(int iBandNum) const;
	
	/*��ȡValueOffset*/
	double getBandValueOffset(int iBandNum) const;

	/*��ȡĳ���������ص���Сֵ*/
	double getBandMinimum(int iBandNum) const;

	/*��ȡĳ���������ص����ֵ*/
	double getBandMaximum(int iBandNum) const;

	/*��ȡĳ���������صĿհ�ֵ*/
	double getBandNullValue(int iBandNum) const;

	/*��ȡX�ռ�ֱ���*/
	double getXResolution();
	
	/*��ȡY�ռ�ֱ���*/
	double getYResolution();
	
	/*��ȡĳһ�����ε�ָ��, ��1��ʼ����*/
	/*�ⲿ����GTRasterBand::destroyRasterBand�ͷ�*/
	GTRasterBand *getRasterBand(int iBandNum);

	/*��Ҫ���������subdataset�Ĳ���*/
	/*��ȡһ���������ݼ����Ӽ�, iSubNum��1��ʼ*/
	GTRasterDataset *getSubRasterDataset(int iSubNum);

    /*������դ�����ݴ�������������(�������в���)
	 *�˴������ⲿ��������ʽ, ��Eardas��rrd, ��Ӧ���ļ�Ϊ.aux
	 *�������Ϊ1:2�𼶱����, ��������СΪ2:4:8:16:...2^n(bUseFirst������2�������)
	 *����Ĵ�СΪ����256*256
	 *����������GDAL�ṩ, ����:"NEAREST", "GAUSS", "CUBIC", "AVERAGE", "MODE", 
	 *                      "AVERAGE_MAGPHASE" or "NONE
	 */
	bool createPyramid(enumGTPyraResample eResample, bool bUseFirst = false);

	/*�������������*/
	bool clearPyramid();

    /*������դ�����ݴ������������ݣ������������£�
	 *�˴������ⲿ��������ʽ, ��Erdas��rrd, ��Ӧ���ļ�Ϊ.aux
	 *���ȣ�nOverviewsĬ��ֵΪ0����ʱpanOverviewListӦ��ΪNULL����
	 *���Ϊ0�����㷨�Զ�ȷ����Ҫ�ж��ٲ㣬�Լ�panOverviewList��
	 *��Ĭ��ֵΪ2��4��8��16��...
	 *nListBandsĬ��Ϊ0����ʱpanBandListӦ��ΪNULL�������Ϊ0��
	 *Ĭ�Ͻ����в��ξ�������������pfnProgress�����ⲿ�ṩ��ָʾ����
	 *����������GDAL�ṩ, ����:"NEAREST", "GAUSS", "CUBIC", "AVERAGE", "MODE", 
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
	/*���ÿռ�ο����ڴ�, Ӧ������ʵ����Դ����һ��*/
	bool setInternalSpatialRef(GTSpatialReference *spatialRef);

	void setDataSourceRefPtr(GTDataSource* pDataSource);

protected:
	GDALDataset *gdalDataset;
	bool         isOpened;  /*�򿪱�־*/
	bool         isUpdated; /*�Ƿ���±�־*/

	GTSpatialReference *poSpatialRef; /*����ϵ�ο�*/

	/*����GTRasterDataset������GTDataSource����
	 *�����Ҫ����GTDataSource�Ĳο�ָ��
	 */
	GTDataSource *pDataSourceRef;

	/*���HDF4, HDF5, OGDI, netCDF�ȸ��ϸ�ʽ����
	 *��GetMetadata��ȡsubdataset��NAME, ���ݸ�GDALOpen
	 */
	bool hasSubdatasets;
};

/*
 *GDAL-OGR Data Source�Ľӿ���
 */
class GTS_DLL GTOGRDataSource: public GTDataSource
{
public:
	friend class GTFeatureLayer;
	friend class GTOGRDataDriver;
	
	GTOGRDataSource(void);
	~GTOGRDataSource(void);

	/*��GTDataSource�̳�*/
	enumGTDataDriverType getDataDriverType() const { return GT_DATADRIVER_OGR; }
	const char* getDataDriverName() const;
	const char* getDataSourceName() const;
	bool openDataSource(const char* pSourceName, bool bUpdate);
	bool reopenDataSource(bool bUpdate);
	void closeDataSource();
	void clearDataSource();
	bool isDataSourceOpen() const { return isOpened; }
	bool isDataSourceValid() const;
	
	/*���г�Ա����*/
	/*�ͷŶ���ľ�̬����, ���ǰ�ȫ��, Feature���ò�Ϊ0�Ľ�ֹ�ͷ�*/
	static bool destroyDataSource(GTOGRDataSource *pGtDataSource);

	/*��ȡ�Ƿ�Ϊ���·�ʽ��*/
	bool getIsReadOnly() const;

	/*��ȡOGRDataSourceָ��, �ⲿ�������޸�*/
	OGRDataSource* getOgrDataSourceRefPtr() { return ogrDataSource; }

	/*��ȡ������Դ�в����Ŀ*/
	int getLayerCount() const;

	/*����һ��������, ��Ҫ�ⲿ�ͷ�GTS_DELETE, ����GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getLayer(int iLayer);

	/*����һ��������, ��Ҫ�ⲿ�ͷ�GTS_DELETE, ����GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getLayerByName(const char* layerName);

	/*ɾ����*/
	enumGTDataSourceErr deleteLayer(int iLayer);
	enumGTDataSourceErr deleteLayerByName(const char* layerName);

	/*������*/
	GTFeatureLayer* createLayer(const char* layerName, 
		                        GTSpatialReference* spatialRef, 
							    enumGTWKBGeometryType geomType,
								char**                options = 0);

protected:
	OGRDataSource *ogrDataSource;
	bool           isOpened;  /*�򿪱�־*/
	bool           isUpdated; /*�Ƿ���±�־*/
};

#ifdef GDOS_DRAW_QUERY
typedef boost::function<GTGeometry*(enumGTWKBGeometryType, const OGRGeometry*, bool)>	GeometryFactoryFunction;
#endif

/*
 *MySQL Data source�Ľӿ���, ��Ա���SQL, �Լ�դ���ļ�
 */
class GTS_DLL GTGDOSMySQLDataSource : public GTDataSource
{
public:
	friend class GTFeatureLayer;
	friend class GTGDOSMySQLDataDriver;

	GTGDOSMySQLDataSource(void);
	virtual ~GTGDOSMySQLDataSource(void);

	/*��GTDataSource�̳�*/
	enumGTDataDriverType getDataDriverType() const { return GT_DATADRIVER_GDOS_MYSQL; }
// 	const char* getDataDriverName() const;
// 	const char* getDataSourceName() const;
	bool openDataSource(const char* pSourceName, bool bUpdate);/*Deprecated, �����к������*/
	bool reopenDataSource(bool bUpdate);
	void closeDataSource();
	void clearDataSource();
	bool isDataSourceOpen() const { return isOpened; }
	bool isDataSourceValid() const;
	bool changeSource(const char* sourceName);

	MYSQL*      getMySQLPtr() { return mysqlDataSource; }/*��ȡMYSQL����ָ��*/

	/*���г�Ա����*/
	const char* getErrMsg() const;
	int			getErrNo() const { return err_no; }
	void		clearErrNo() { err_no = GT_DS_SUCCESS; }
	const char* getHostName() const;
	const char* getUsername() const;
	const char* getPassword() const;
	const char* getDBName()   const;
	int         getPortNumber() const;

	/*�ͷ�����Դ�ľ�̬����*/
	static bool destroyDataSource(GTGDOSMySQLDataSource *pGtDataSource);

	/*�������ӳ�ʱ����, ��λΪ��*/
	void setTimeoutLength(int outLen);
	int  getTimeoutLength() const;

	/*��ȡ�����ַ�������*/
	const char* getCharacterSet() const;

	/*sourceName��ΪMySQL��schema����*/
	bool openDataSource(const char* user, const char* pwd, 
		                const char* host, int port, const char* sourceName);
	
	/*Ϊĳһ�����ݲ�������������Ϣ*/
	bool setOwner(const char* lName, const char* owner, enumGTLayerType lType);

	/*��ȡĳ�����ߵ����������б�, ���ظ���*/
	int getLayerNamesByOwner(
		const char* owner, 
		enumGTLayerType lType, 
		std::vector<std::string> &lname_list);

	/*ɾ��ĳ�����ߵ���������, ����ɾ�������ݵĸ���*/
	int deleteLayersByOwner(
		const char* owner, 
		enumGTLayerType lType);

	/*ִ���û������SQL���, ���Բ�ֻ�����һ��schema*/
	bool executeSQL(const char* sqlStr);

    /*
	 *��ȡĳ�����ļ�¼����
	 *-1Ϊ���ִ���, ����Ϊ��¼��
	 */
	int getRecordCountFromTable(const char* tableName);

	/*
	 *��ȡĳ�������еĸ���
	 *-1Ϊ���ִ���, ����Ϊ��¼��
	 */
	int getFieldCountFromTable(const char* tableName);

	/*��ѯĳ�����Ƿ����*/
	bool existTable(const char* tableName);

	/*
	 *����ͼ������ȡ����
	 *tableNameΪ0ʱ���������GTS_FREE�ͷŷ���ֵ
	 *�ⲿ����tableNameʱӦȷ��tableName��С����Ϊ65
	 *����ֵΪ0ʱ��ʾʧ��
	 */
	const char* getTableNameByLayerName(const char* layerName, char* tableName = 0);

	/*************************************************************
	 *���¶�ӦFeature���ݵĲ���
	 *************************************************************/
	/*��ȡ�����Ŀ*/
	int getFeatureLayerCount();

	/*��ȡ���в�����ֵļ���, ����-1����Դ����, ����: �����Ŀ*/
	int getFeatureLayerNames(std::vector<std::string> &lname_list);

	/*����һ��������, ��Ҫ�ⲿ�ͷ�GTS_DELETE, ����GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getFeatureLayer(
		int  iLayer, 
		bool bUpdate = false);

	/*����һ��������, ��Ҫ�ⲿ�ͷ�GTS_DELETE, ����GTFeatureLayer::destroyFeatureLayer*/
	GTFeatureLayer* getFeatureLayerByName(
		const char* layerName, 
		bool        bUpdate = false);

	/*����һ��TIN������, ��Ҫ�ⲿ�ͷ�GTS_DELETE, ����GTTinLayer::destroyTinLayer*/
	GTTinLayer* getTinLayerByName(const char* layerName);

	/*ɾ����*/
	enumGTDataSourceErr deleteFeatureLayer(int iLayer);
	enumGTDataSourceErr deleteFeatureLayerByName(const char* layerName);


	/*����һ���յĲ�, ֻ��������Ϣ, ��ͶӰ��Ϣsrid*/
	GTFeatureLayer *createFeatureLayer(
		const char* layerName, 
		GTSpatialReference* sRef, 
		GTFields* fieldsDefn, 
		enumGTWKBGeometryType geomType);

	/*����һ����, ����������*/
	bool addFeatureLayer(
		GTFeatureLayer* pFeatureLayer, 
		const char* layerName, 
		bool bLock = true);

	/*����һ���յ�Tin��*/
	GTTinLayer *createTinLayer(
		const char* layerName, 
		GTSpatialReference* sRef);

	/*����һ����, ���������ļ���������, �ݲ�֧��*/
	bool addFeatureLayerFromDataFile(
		GTFeatureLayer* pFeatureLayer, 
		const char*     layerName,
		const char*     dataFile);

	/*��Feanture�ı���ѯ��¼����, ���µ�ϵͳ��, ͬʱ���ؼ�¼��*/
	int updateFeatureCount(const char* layerName);

	/*������Feature Layer*/
	bool renameFeatureLayer(
		const char* layerNameOld, 
		const char* layerNameNew);

	/*����Feature Layer*/
	bool copyFeatureLayer(
		const char* layerNameFrom, 
		const char* layerNameTo);

	/*���ò�Ŀռ�ο���Ϣ*/
	bool defineLayerSpatialReference(
		const char* layerName,
		const char* srsWKT);

#ifdef GDOS_DRAW_QUERY
	/*
	 *Feature���ۺϲ�ѯ����, �Ӹ�����ʸ����ı��л�ȡ����
	 *������ص���FeatureClass, �û��ⲿ���봫��һ��GTFields�Ĳο�ָ��
	 *���ػ�ȡ����ĸ���, -1�����д�����
	 *�����뼸������ΪAND, ��������֮��ΪOR
	 */
// 	int queryFeaturesFromLayerTable(
// 		const char                  *tableName,            /*��ѯ�����ڵı���*/
// 		unsigned int                 uFlag,                /*0-ֻ�������, 1-feature, 2-geom, 3-fid, 4-geom+fid*/
// 	    const char                  *sqlFilter,            /*���Բ�ѯ���, SQL��WHERE����Ĳ���*/
// 	    const GTEnvelop             *pEnvelopFilter,       /*��ѯ���οռ䷶Χ����*/
// 	    const enumGTSpatialRelation *pSPR1,                /*�ռ��ϵ�������*/
// 	    int                          nEnvelopCount,        /*pEnvelopFilter��pSPR1�ĸ���*/
// 	    const GTGeometry           **ppGeometryFilter,     /*��ѯ���οռ䷶Χָ������*/
// 		const enumGTSpatialRelation *pSPR2,                /*�ռ��ϵ�������*/
// 	    int                          nGeometryFilterCount, /*ppGeometryFilter��pSPR2�ĸ���*/
// 		const GTFields              *pFields,              /*���Featureʱ��Ҫ�������Զ���ָ��, !!!�������ͷ�Feature֮ǰ�ͷ�!!!*/
// 	    bool                         bSortFid,             /*�Ƿ��fid����������*/
// 	    std::vector<GTGeometry*>    *pGeomResult,          /*��uFlag�����Ƿ�������û������ͷ�*/
// 	    std::vector<int>            *pFidResult,           /*��uFlag�����Ƿ�������û������ͷ�*/
// 	    GTFeatureClass              *pFeatureClass);       /*��uFlag�����Ƿ�������û������ͷ�*/

	struct QueryOptions {
		const char					*tableName;				/*��ѯ�����ڵı���*/
		unsigned int				 uFlag;					/*0-ֻ�������, 1-feature, 2-geom, 3-fid, 4-geom+fid*/
		bool						 bUpdateEnv;			/*�Ƿ�����������*/
		const char					*sqlFilter;				/*���Բ�ѯ���, SQL��WHERE����Ĳ���*/
		const GTEnvelop				*pEnvelopFilter;		/*��ѯ���οռ䷶Χ����*/
		const enumGTSpatialRelation	*pSPR1;					/*�ռ��ϵ�������*/
		int							 nEnvelopCount;			/*pEnvelopFilter��pSPR1�ĸ���*/
		const GTGeometry		   **ppGeometryFilter;		/*��ѯ���οռ䷶Χָ������*/
		const enumGTSpatialRelation	*pSPR2;					/*�ռ��ϵ�������*/
		int							 nGeometryFilterCount;	/*ppGeometryFilter��pSPR2�ĸ���*/
		const GTFields				*pFields;				/*���Featureʱ��Ҫ�������Զ���ָ��, !!!�������ͷ�Feature֮ǰ�ͷ�!!!*/
		int*						 nStartFid;				/*��ʼFID�����ڷֶ�β�ѯʱ��ÿ�β�ѯ���޸ģ��´β�ѯ��ֱ��ʹ�ã�uFlagΪ0��2ʱ���⣩*/
		int							 nRecordLimit;			/*���β�ѯ��������*/
		bool						 bSortFid;				/*�Ƿ��fid����������nRecordLimit��0ʱ��Ȼ���򣬻���Ը�ֵ*/
		GeometryFactoryFunction*	 geomFac;				/*���ι���*/
		std::vector<GTGeometry*>	*pGeomResult;			/*��uFlag�����Ƿ���������û�����*/
		std::vector<int>			*pFidResult;			/*��uFlag�����Ƿ���������û�����*/
		const std::vector<const char*> *pFieldNames;		/*��Ҫ��ȡ������������uFlagΪ2~4ʱ��Ч*/
		std::vector<std::string>	*pFieldValues;			/*��ȡ������ֵ����$����Ϊ�ָ���������ֵ�ڳ���$ʱ����\ת�塣uFlagΪ2~4ʱ��Ч*/
		GTFeatureClass				*pFeatureClass;			/*��uFlag�����Ƿ���������û�����*/
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
	/*����һ��Feature���ݼ�¼, bCheck�����Ƿ�������ݵ���Ч�Լ��*/
	bool addFeatureToLayer(
		const char* layerName, 
		GTFeature*  featureObj, 
		bool        bCheck = false,
		bool        bForce = true);

	/*����һϵ��Feature���ݼ�¼*/
	int addFeaturesToLayer(
		const char*     layerName, 
		const GTFeatureClass& fClass, 
		bool            bLock = true);
	
	/*ɾ��Feature���ݼ�¼, ����ID*/
	int deleteFeatureFromLayer(
		const char* layerName, 
		int         FID, 
		bool        bCheck = false,
		bool        bForce = false);

	/*ɾ��һϵ��Feature���ݼ�¼, ����ID, �������м�¼����*/
	int deleteFeaturesFromLayer(
		const char*             layerName, 
		const std::vector<int>& fid_list, 
		bool                    bLock = true);

	/*ɾ��һϵ��Feature���ݼ�¼, ����IDһ���Բ���*/
	int deleteFeaturesFromLayerBatch(
		const char*             layerName, 
		const std::vector<int>& fid_list, 
		bool                    bLock = true);

	/*����һ��Feature���ݼ�¼, ����ID*/
	bool setFeatureToLayer(
		const char*      layerName, 
		const GTFeature* featureObj, 
		bool             bCheck = false);

	/*����һϵ��Feature���ݼ�¼, ����ID, �������м�¼����*/
	int setFeaturesToLayer(
		const char*     layerName, 
		const GTFeatureClass& fClass, 
		bool            bLock = true);

	/*Ϊĳһ���㴴���ռ�����(geom)������*/
	/*һ��Ӧ���ڵ�����������֮��, ����Ӱ���ٶ�*/
	bool addFeatureSpatialIndex(const char* layerName);

	/*Ϊĳһ����ɾ���ռ�����(geom)������*/
	bool deleteFeatureSpatialIndex(const char* layerName);

	/*Ϊĳһ����������д�����������, ����ΪfdName_idx*/
	/*һ��Ӧ���ڵ�����������֮��, ����Ӱ���ٶ�*/
	bool addFeatureFieldIndex(
		const char* layerName, 
		const char* fieldName);

	/*ɾ��ĳһ�����ĳ�������е�����*/
	bool deleteFeatureFieldIndex(
		const char* layerName,  
		const char* fieldName);

	/*����ĳһ�е��ƶ���*/
	bool addFeatureField(
		const char* layerName, 
		const GTField*    pField);

	/*��һ������ɾ��ĳһ����*/
	bool deleteFeatureField(
		const char* layerName, 
		const char* fieldName);

public:
	/*******************************************************************************
	 *���¶�ӦRaster���ݵĲ���, DataSource����դ�����ΪGTRasterDataset(��ӦGDALDataset)
	 *******************************************************************************/
	int getRasterDatasetCount();
	
	/*��ȡ����դ�����ݵ����ֵļ���, ����-1����Դ����, ����: ���ݵ���Ŀ*/
	int getRasterDatasetNames(std::vector<std::string> &rname_list);

	/*����դ������id, ��ȡһ��դ�����ݾ��, bUpdate�����Ƿ�����ݽ���д����*/
	GTRasterDataset *getRasterDataset(
		int  rID, 
		bool bUpdate);
	
	/*����դ����������, ��ȡһ��դ�����ݾ��, bUpdate�����Ƿ�����ݽ���д����*/
	GTRasterDataset *getRasterDatasetByName(
		const char* poName, 
		bool        bUpdate);
	
	/*�����յ�դ������, ���ؾ��, ���Ҳ�֧��*/
	GTRasterDataset* createRasterDataset();

	/*��ѯդ�������Ƿ����*/
	// zjw
	bool existRasterDataset(const char *poName, bool& exist);
	
	/*������դ�����ݼ������ݿ�, ���Խ��ⲿ�����ȿ�����Ӳ��, Ȼ��ʹ��GDAL��, Ȼ����ϵͳ����ע��*/
	bool addRasterDataset(
		const GTRasterDataset* pDataset, 
		const char           * poName);

	/*������դ�����ݼ������ݿ�, ����������������(�������в���)
	 *�˴������ⲿ��������ʽ, ��Eardas��rrd, ��Ӧ���ļ�Ϊ.aux
	 *�������Ϊ1:2�𼶱����, ��������СΪ2:4:8:16:...2^n
	 *����Ĵ�СΪ����64*64
	 *����������GDAL�ṩ, ����:"NEAREST", "GAUSS", "CUBIC", "AVERAGE", "MODE", 
	 *                      "AVERAGE_MAGPHASE" or "NONE
	 *����ֵ: -1:����ע�᲻�ɹ�, 0:ע��ɹ�, ����������ʧ��, 1:ȫ���ɹ�
	 */
	int addRasterDatasetPyramid(
		GTRasterDataset   *pDataset, 
		const char        *poName, 
		enumGTPyraResample eResample);

	/*ɾ��һ��դ�����ݼ�¼, bDeleteSourceָʾ�Ƿ�ɾ����������*/
	/*����ֵ: -1:���ݲ����ڻ����������ݿ����, 0:���¼ɾ��, �ļ�û��ɾ��, 1:ȫ���ɹ�*/
	int deleteRasterDataset(
		const char* poName, 
		bool        bDeleteSource = false);

protected:
	/*������ݿ��������ӵ���Ϣ��¼*/
	char       dbName[65];         /*���ݿ�����*/
	char       *userName;       /*�û���*/
	char       *pswString;      /*����*/
	char       *hostName;       /*������ַ*/
	int         portNumber;     /*�˿ں�*/
	bool        isOpened;       /*�򿪱�־*/
	MYSQL      *mysqlDataSource;/*MYSQL�ӿ�ָ��*/
	int         timeoutLength;  /*���ӳ�ʱ����, ��λΪ��, ͬ����, ����д���г�ʱ����*/
	int			err_no;
	std::string err_msg;        /*������Ϣ*/
	const char *charSet;        /*�ַ�������, ����utf8, gbk��*/
};

/*
 *Oracle Data source�Ľӿ���, ��Ա���SQL, �Լ�դ���ļ�
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
// 	/*��GTDataSource�̳�*/
// 	enumGTDataDriverType getDataDriverType() const;
// 	const char* getDataDriverName() const;
// 	const char* getDataSourceName() const;
// 	bool openDataSource(const char* pSourceName, bool bUpdate);/*Deprecated, �����к������*/
// 	bool reopenDataSource(bool bUpdate);
// 	void closeDataSource();
// 	void clearDataSource();
// 	bool isDataSourceOpen() const;
// 	bool isDataSourceValid() const;
// 
// 	/*���г�Ա����*/
// 	const char* getOCIErrMsg() const; /*��ȡ���һ��OCIִ�еĴ�����Ϣ*/ 
// 	const char* getHostName() const;
// 	const char* getUsername() const;
// 	const char* getPassword() const;
// 	const char* getDBName()   const;
// 	int         getPortNumber() const;
// 
// 	/*�ͷ�����Դ�ľ�̬����*/
// 	static bool destroyDataSource(GTGDOSOCIDataSource *pGtDataSource);
// 
// 	bool openDataSource(
// 		const char* user,    /*��½�û�, ͬʱҲ��schema��datasourcename*/
// 		const char* pwd,     /*����*/
// 		const char* host,    /*������ַ*/
// 		int         port,    /*�˿�*/
// 		const char* db,      /*oracle service, Ҳ��dbName*/
// 		int         iMode=2);/*��½���, Ĭ��ΪDBA*/
// 
// 	/*ִ���û������SQL���*/
// 	bool executeSQL(const char* sqlStr);
// 
// 	/*
// 	 *��ȡĳ�����ļ�¼����
// 	 *-1Ϊ���ִ���, ����Ϊ��¼��
// 	 */
// 	int getRecordCountFromTable(const char* tableName);
// 
// 	/*
// 	 *��ȡĳ�������еĸ���
// 	 *-1Ϊ���ִ���, ����Ϊ��¼��
// 	 */
// 	int getFieldCountFromTable(const char* tableName);
// 
// 	/*��ѯĳ�����Ƿ����*/
// 	bool existTable(const char* tableName);
// 
// 	/*************************************************************
// 	 *���¶�ӦFeature���ݵĲ���
// 	 *************************************************************/
// 	/*��ȡ�����Ŀ*/
//     int getFeatureLayerCount();
// 	
//     /*ɾ����, iLayer��1��ʼ*/
// 	bool deleteFeatureLayer(int iLayer);
// 	bool deleteFeatureLayerByName(const char* layerName);
// 
// 	/*����һ���յĲ�, ֻ��������Ϣ, ��ͶӰ��Ϣsrid*/
// 	GTFeatureLayer *createFeatureLayer(
// 		const char* layerName, 
// 		GTSpatialReference* sRef, 
// 		GTFields* fieldsDefn, 
// 		enumGTWKBGeometryType geomType);
// 
// protected:
//     /*Ϊĳһ���㴴���ռ�����(geom)������*/
// 	/*һ��Ӧ���ڵ�����������֮��, ����Ӱ���ٶ�*/
// 	bool addFeatureSpatialIndex(const char* layerName);
// 
// 	/*Ϊĳһ����ɾ���ռ�����(geom)������*/
// 	bool deleteFeatureSpatialIndex(const char* layerName);
// 
// protected:
// 	/*������ݿ��������ӵ���Ϣ��¼*/
// 	char    *dbName;          /*���ݿ�����*/
// 	char    *userName;        /*�û���, ��dataSourceName��ͬ*/
// 	char    *pswString;       /*����*/
// 	char    *hostName;        /*������ַ*/
// 	int      portNumber;      /*�˿ں�*/
// 	bool     isOpened;        /*�򿪱�־*/
// // 	GTOCISession *pOCISession;/*oci�ӿ�ָ��*/
// };

}
#endif
