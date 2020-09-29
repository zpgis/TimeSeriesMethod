/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  数据驱动的定义类
* Author:   范俊甫, fanjf@lreis.ac.cn 2010年-12月-12日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTDATADRIVER_H_INCLUDED
#define GTDATADRIVER_H_INCLUDED

#include "gts_port.h"
#include "gt_datasource.h"
// #include "gt_oci.h"

namespace gts{

/*
 *DataDriver的抽象类, 多为静态成员函数, 用于数据源的创建\删除\拷贝
 */
class GTS_DLL GTDataDriver
{
public:
	GTDataDriver(void);
	virtual ~GTDataDriver(void);
	
	/*获取数据驱动类型*/
	//virtual enumGTDataDriverType getDataDriverType() const = 0;
	
	/*创建数据源, 外部调用GTS_DELETE或destroyDataSource释放*/
	//virtual GTDataSource* createDataSource() = 0;

	/*删除数据源*/
	//virtual enumGTDataSourceErr deleteDataSource() = 0;

	/*复制数据源*/
	//virtual GTDataSource* copyDataSource() = 0;
};

/*
 *OGRDataDriver接口类, 多为静态成员函数, 用于数据源的创建\删除\拷贝
 */
class GTS_DLL GTOGRDataDriver: public GTDataDriver
{
public:
	GTOGRDataDriver(void);
	~GTOGRDataDriver(void);

	/*由GTDataDriver继承*/
	static enumGTDataDriverType getDataDriverType();

	/*创建一个新的OGR数据源, 如果是文件型, sourceName实际上为目录明*/
	static GTOGRDataSource* createDataSource(
		const char* sourceName, 
		const char* ogrDriverName,
		char**      options  = 0);

	/*删除OGR数据源, 删除之前最好确定数据源没有被打开, 目前程序没有给出判断*/
	static enumGTDataSourceErr deleteDataSource(
		const char* sourceName, 
		const char* ogrDriverName);

	/*拷贝OGR数据源*/
	static GTOGRDataSource* copyDataSource(
		GTOGRDataSource* fromDataSource, 
		const char*      newDataSourceName);
};

/*
 *GDALDataDriver接口类, 多为静态成员函数, 用于数据源的创建\删除\拷贝
 */
class GTS_DLL GTGDALDataDriver: public GTDataDriver
{
public:
	GTGDALDataDriver(void);
	~GTGDALDataDriver(void);

	static enumGTDataDriverType getDataDriverType();

	/*创建一个数据源文件, 并返回指针*/
	static GTRasterDataset* createDataSource(
		const char* gdalDriverName, /*GDAL驱动类型*/
		const char *sourceName,     /*数据源文件名*/
		int nXSize,                 /*列数*/
		int nYSize,                 /*行数*/
		int nBands,                 /*波段数*/
		enumGTDataType eType,       /*数据类型*/
		char **options  = 0);       /*其它参数*/

	/*按照空间范围和指定分辨率创建一个数据源文件, 并返回指针*/
	static GTRasterDataset* createDataSource(
		const char* gdalDriverName, /*GDAL驱动类型*/
		const char *sourceName,     /*数据源文件名*/
		double dXResl,              /*X空间分辨率*/
		double dYResl,              /*Y空间分辨率*/
		GTEnvelop& env,             /*空间范围*/
		const char* srswkt,         /*空间坐标*/
		int nBands,                 /*波段数*/
		enumGTDataType eType,       /*数据类型*/
		char **options  = 0);       /*其它参数*/

	/*删除一个数据源文件*/
	static enumGTDataSourceErr deleteDataSource(
		const char* sourceName, 
		const char* gdalDriverName);

	/*重新命名数据源(即GDALDataset)*/
	static enumGTDataSourceErr renameDataSource(
		const char* sourceNameOld, 
		const char* sourceNameNew, 
		const char* gdalDriverName);

	/*拷贝数据源(即GDALDataset)*/
	static enumGTDataSourceErr copyDataSource(
		const char* sourceNameOld, 
		const char* sourceNameNew, 
		const char* gdalDriverName);

};

/*
 *gDos MySQL DataDriver类
 *创建schema, 各类系统表
 *删除schema等
 */
class GTS_DLL GTGDOSMySQLDataDriver: public GTDataDriver
{
public:
	friend class GTGDOSMySQLDataSource;

	GTGDOSMySQLDataDriver(void);
	~GTGDOSMySQLDataDriver(void);

	/*由GTDataDriver继承*/
	static enumGTDataDriverType getDataDriverType() { return GT_DATADRIVER_GDOS_MYSQL; }

	/*创建一个新的MySQL数据源*/
	static GTGDOSMySQLDataSource* createDataSource(
// 		const char* db,
		const char* user,
		const char* pwd,
		const char* host,
		int         port,
		const char* schema,
		const char* charSet = NULL);

	/*删除数据源, 删除之前最好确定数据源没有被打开, 目前程序没有给出判断*/
	static bool deleteDataSource(
		const char* db,
		const char* user,
		const char* pwd,
		const char* host,
		int         port,
		const char* schema);

	/*拷贝DataSource, 包括数据源中所有的数据, 表单以及栅格文件*/
	static bool copyDataSource(
		GTGDOSMySQLDataSource* fromDataSource,
		GTGDOSMySQLDataSource* toDataSource);
};

/*
 *gDos Oracle DataDriver类
 *创建user(对应schema), 各类系统表
 *删除user(对应schema)
 */
// class GTS_DLL GTGDOSOCIDataDriver: public GTDataDriver
// {
// public:
// 	friend class GTGDOSOCIDataSource;
// 
// 	GTGDOSOCIDataDriver(void);
// 	~GTGDOSOCIDataDriver(void);
// 
// 	/*由GTDataDriver继承*/
// 	static enumGTDataDriverType getDataDriverType();
// 
// 	/*创建一个新的Oracle数据源, 及创建用户和对应schema
// 	 *oracle在创建db service时需要指定字符集
// 	 *tableSpace不为空时, 将创建表空间(newUser_tb)和临时表空间(newUser_ttb)
// 	 *初始分配大小为50M和30M, 并且设定以相同的额度自动增长
// 	 */
// 	static bool createDataSource(
// 		const char* dbaUser,    /*管理员身份登陆, 如果为空, 则默认sys as sysdba*/
// 		const char* dbaPwd,     /*管理员密码*/
// 		const char* host,       /*主机地址*/
// 		int         port,       /*端口*/
// 		const char* db,         /*oracle service*/
// 		const char* newUser,    /*同时也是schema名字, 授权为connect，resource, dba*/
// 		const char* newUserPwd, /*新用户的pwd*/
// 		const char* tableSpace, /*表空间(包括临时)的目录, 自动生成tableSpace//newUser_data.dbf和tableSpace//newUser_temp.dbf*/
// 		int         iMaxspace = 1024); /*最大表空间, 单位Mega Bytes*/
// 
// 	/*删除数据源, 删除之前最好确定数据源没有被打开, 目前程序没有给出判断*/
// 	static bool deleteDataSource(
// 		const char* dbaUser,    /*管理员身份登陆, 如果为空, 则默认sys as sysdba*/
// 		const char* dbaPwd,     /*管理员密码*/
// 		const char* host,       /*主机地址*/
// 		int         port,       /*端口*/
// 		const char* db,         /*oracle service*/
// 		const char* sourceName);/*用户名字, 同时也是schema名字*/
// 
// 	/*拷贝DataSource, 包括数据源中所有的数据, 表单以及栅格文件*/
// 	static bool copyDataSource(
// 		GTGDOSOCIDataSource* fromDataSource, 
// 		GTGDOSOCIDataSource* toDataSource);
// };

}
#endif
