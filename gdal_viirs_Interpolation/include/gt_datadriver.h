/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���������Ķ�����
* Author:   ������, fanjf@lreis.ac.cn 2010��-12��-12��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
****************************************************************/
#pragma once
#ifndef GTDATADRIVER_H_INCLUDED
#define GTDATADRIVER_H_INCLUDED

#include "gts_port.h"
#include "gt_datasource.h"
// #include "gt_oci.h"

namespace gts{

/*
 *DataDriver�ĳ�����, ��Ϊ��̬��Ա����, ��������Դ�Ĵ���\ɾ��\����
 */
class GTS_DLL GTDataDriver
{
public:
	GTDataDriver(void);
	virtual ~GTDataDriver(void);
	
	/*��ȡ������������*/
	//virtual enumGTDataDriverType getDataDriverType() const = 0;
	
	/*��������Դ, �ⲿ����GTS_DELETE��destroyDataSource�ͷ�*/
	//virtual GTDataSource* createDataSource() = 0;

	/*ɾ������Դ*/
	//virtual enumGTDataSourceErr deleteDataSource() = 0;

	/*��������Դ*/
	//virtual GTDataSource* copyDataSource() = 0;
};

/*
 *OGRDataDriver�ӿ���, ��Ϊ��̬��Ա����, ��������Դ�Ĵ���\ɾ��\����
 */
class GTS_DLL GTOGRDataDriver: public GTDataDriver
{
public:
	GTOGRDataDriver(void);
	~GTOGRDataDriver(void);

	/*��GTDataDriver�̳�*/
	static enumGTDataDriverType getDataDriverType();

	/*����һ���µ�OGR����Դ, ������ļ���, sourceNameʵ����ΪĿ¼��*/
	static GTOGRDataSource* createDataSource(
		const char* sourceName, 
		const char* ogrDriverName,
		char**      options  = 0);

	/*ɾ��OGR����Դ, ɾ��֮ǰ���ȷ������Դû�б���, Ŀǰ����û�и����ж�*/
	static enumGTDataSourceErr deleteDataSource(
		const char* sourceName, 
		const char* ogrDriverName);

	/*����OGR����Դ*/
	static GTOGRDataSource* copyDataSource(
		GTOGRDataSource* fromDataSource, 
		const char*      newDataSourceName);
};

/*
 *GDALDataDriver�ӿ���, ��Ϊ��̬��Ա����, ��������Դ�Ĵ���\ɾ��\����
 */
class GTS_DLL GTGDALDataDriver: public GTDataDriver
{
public:
	GTGDALDataDriver(void);
	~GTGDALDataDriver(void);

	static enumGTDataDriverType getDataDriverType();

	/*����һ������Դ�ļ�, ������ָ��*/
	static GTRasterDataset* createDataSource(
		const char* gdalDriverName, /*GDAL��������*/
		const char *sourceName,     /*����Դ�ļ���*/
		int nXSize,                 /*����*/
		int nYSize,                 /*����*/
		int nBands,                 /*������*/
		enumGTDataType eType,       /*��������*/
		char **options  = 0);       /*��������*/

	/*���տռ䷶Χ��ָ���ֱ��ʴ���һ������Դ�ļ�, ������ָ��*/
	static GTRasterDataset* createDataSource(
		const char* gdalDriverName, /*GDAL��������*/
		const char *sourceName,     /*����Դ�ļ���*/
		double dXResl,              /*X�ռ�ֱ���*/
		double dYResl,              /*Y�ռ�ֱ���*/
		GTEnvelop& env,             /*�ռ䷶Χ*/
		const char* srswkt,         /*�ռ�����*/
		int nBands,                 /*������*/
		enumGTDataType eType,       /*��������*/
		char **options  = 0);       /*��������*/

	/*ɾ��һ������Դ�ļ�*/
	static enumGTDataSourceErr deleteDataSource(
		const char* sourceName, 
		const char* gdalDriverName);

	/*������������Դ(��GDALDataset)*/
	static enumGTDataSourceErr renameDataSource(
		const char* sourceNameOld, 
		const char* sourceNameNew, 
		const char* gdalDriverName);

	/*��������Դ(��GDALDataset)*/
	static enumGTDataSourceErr copyDataSource(
		const char* sourceNameOld, 
		const char* sourceNameNew, 
		const char* gdalDriverName);

};

/*
 *gDos MySQL DataDriver��
 *����schema, ����ϵͳ��
 *ɾ��schema��
 */
class GTS_DLL GTGDOSMySQLDataDriver: public GTDataDriver
{
public:
	friend class GTGDOSMySQLDataSource;

	GTGDOSMySQLDataDriver(void);
	~GTGDOSMySQLDataDriver(void);

	/*��GTDataDriver�̳�*/
	static enumGTDataDriverType getDataDriverType() { return GT_DATADRIVER_GDOS_MYSQL; }

	/*����һ���µ�MySQL����Դ*/
	static GTGDOSMySQLDataSource* createDataSource(
// 		const char* db,
		const char* user,
		const char* pwd,
		const char* host,
		int         port,
		const char* schema,
		const char* charSet = NULL);

	/*ɾ������Դ, ɾ��֮ǰ���ȷ������Դû�б���, Ŀǰ����û�и����ж�*/
	static bool deleteDataSource(
		const char* db,
		const char* user,
		const char* pwd,
		const char* host,
		int         port,
		const char* schema);

	/*����DataSource, ��������Դ�����е�����, ���Լ�դ���ļ�*/
	static bool copyDataSource(
		GTGDOSMySQLDataSource* fromDataSource,
		GTGDOSMySQLDataSource* toDataSource);
};

/*
 *gDos Oracle DataDriver��
 *����user(��Ӧschema), ����ϵͳ��
 *ɾ��user(��Ӧschema)
 */
// class GTS_DLL GTGDOSOCIDataDriver: public GTDataDriver
// {
// public:
// 	friend class GTGDOSOCIDataSource;
// 
// 	GTGDOSOCIDataDriver(void);
// 	~GTGDOSOCIDataDriver(void);
// 
// 	/*��GTDataDriver�̳�*/
// 	static enumGTDataDriverType getDataDriverType();
// 
// 	/*����һ���µ�Oracle����Դ, �������û��Ͷ�Ӧschema
// 	 *oracle�ڴ���db serviceʱ��Ҫָ���ַ���
// 	 *tableSpace��Ϊ��ʱ, ��������ռ�(newUser_tb)����ʱ��ռ�(newUser_ttb)
// 	 *��ʼ�����СΪ50M��30M, �����趨����ͬ�Ķ���Զ�����
// 	 */
// 	static bool createDataSource(
// 		const char* dbaUser,    /*����Ա��ݵ�½, ���Ϊ��, ��Ĭ��sys as sysdba*/
// 		const char* dbaPwd,     /*����Ա����*/
// 		const char* host,       /*������ַ*/
// 		int         port,       /*�˿�*/
// 		const char* db,         /*oracle service*/
// 		const char* newUser,    /*ͬʱҲ��schema����, ��ȨΪconnect��resource, dba*/
// 		const char* newUserPwd, /*���û���pwd*/
// 		const char* tableSpace, /*��ռ�(������ʱ)��Ŀ¼, �Զ�����tableSpace//newUser_data.dbf��tableSpace//newUser_temp.dbf*/
// 		int         iMaxspace = 1024); /*����ռ�, ��λMega Bytes*/
// 
// 	/*ɾ������Դ, ɾ��֮ǰ���ȷ������Դû�б���, Ŀǰ����û�и����ж�*/
// 	static bool deleteDataSource(
// 		const char* dbaUser,    /*����Ա��ݵ�½, ���Ϊ��, ��Ĭ��sys as sysdba*/
// 		const char* dbaPwd,     /*����Ա����*/
// 		const char* host,       /*������ַ*/
// 		int         port,       /*�˿�*/
// 		const char* db,         /*oracle service*/
// 		const char* sourceName);/*�û�����, ͬʱҲ��schema����*/
// 
// 	/*����DataSource, ��������Դ�����е�����, ���Լ�դ���ļ�*/
// 	static bool copyDataSource(
// 		GTGDOSOCIDataSource* fromDataSource, 
// 		GTGDOSOCIDataSource* toDataSource);
// };

}
#endif
