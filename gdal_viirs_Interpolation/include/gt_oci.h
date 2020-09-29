/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  OCI接口定义类, 参考GDAL_OGR
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-10月-19日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#ifndef GTOCI_H_INCLUDED
#define GTOCI_H_INCLUDED

#include "../include_p/oracle_11g/oratypes.h"
#include "../include_p/oracle_11g/ort.h"
#include "../include_p/oracle_11g/oci.h"
#include "gt_feature.h"

#define GT_TYPE_OWNER                 "MDSYS"
#define GT_SDO_GEOMETRY               "MDSYS.SDO_GEOMETRY"

#define GT_ORA_GTYPE_MATCH(a,b)      ( ((a) % 100) == ((b) % 100))
#define GT_ORA_GTYPE_UNKNOWN         0
#define GT_ORA_GTYPE_POINT           1
#define GT_ORA_GTYPE_LINESTRING      2    // or curve
#define GT_ORA_GTYPE_POLYGON         3    // or surface
#define GT_ORA_GTYPE_COLLECTION      4
#define GT_ORA_GTYPE_MULTIPOINT      5 
#define GT_ORA_GTYPE_MULTILINESTRING 6    // or multicurve
#define GT_ORA_GTYPE_MULTIPOLYGON    7    // or multisurface
#define GT_ORA_GTYPE_SOLID           8
#define GT_ORA_GTYPE_MULTISOLID      9

namespace gts{

typedef struct SDO_POINT_TYPE
{
	OCINumber x;
	OCINumber y;
	OCINumber z;
} sdo_point_type;

typedef OCIArray sdo_elem_info_array;
typedef OCIArray sdo_ordinate_array;

typedef struct SDO_POINT_TYPE_IND
{
	OCIInd _atomic;
	OCIInd x;
	OCIInd y;
	OCIInd z;
} sdo_point_type_ind;

typedef struct SDO_GEOMETRY_IND
{
	OCIInd                    _atomic;
	OCIInd                    sdo_gtype;
	OCIInd                    sdo_srid;
	sdo_point_type_ind        sdo_point;
	OCIInd                    sdo_elem_info;
	OCIInd                    sdo_ordinates;
} SDO_GEOMETRY_ind;

/*
 *OCISession接口类
 */
class GTOCISession{
public:
	GTOCISession();
	virtual ~GTOCISession();

	/*
	 *建立oci连接
	 *iMode: 0-OCI_DEFAULT, 1-OCI_MIGRATE, 2-OCI_SYSDBA
	         3-OCI_SYSOPER, 4-OCI_PRELIM_AUTH, other-OCI_DEFAULT 
	 */
	int establishSession(
		const char *pszUserid,
		const char *pszPassword,
		const char *pszDatabase,
		int         iMode     = 0,
		bool        bGeoCheck = false);

	int failed(
		sword nStatus, 
		const char *pszFunction = NULL );

	/*解析OCI参数为GTFiled的描述
	 *pFiledDefn为外部传入, 成员值将被改变
	 */
	bool getParmInfo(
		OCIParam     *hParmDesc, 
		GTField      *pFiledDefn,
		ub2          *pnOCIType, 
		ub4          *pnOCILen );

	/*修改为Oracle支持的命名*/
	void cleanName(char * pszName);

	/*由名字获取类型描述*/
	OCIType *pinTDO(const char *pszType);

	/*外部调用GTS_FREE释放*/
	const char* getErrMsg(){return szErrorMsg;}

public:
	OCIEnv     *hEnv;
	OCIError   *hError;
	OCISvcCtx  *hSvcCtx;
	OCIServer  *hServer;
	OCISession *hSession;
	OCIDescribe*hDescribe;
	OCIType    *hGeometryTDO;
	OCIType    *hOrdinatesTDO;
	OCIType    *hElemInfoTDO;

	char       *pszUserid;
	char       *pszPassword;
	char       *pszDatabase;

	char szErrorMsg[1000];
};

class GTOCIStatement{
public:
	GTOCIStatement(GTOCISession *poSessionIn);
	virtual ~GTOCIStatement();

	OCIStmt *getStatement() { return hStatement; }

	bool bindScalar( 
		const char *pszPlaceName, 
		void       *pData, 
		int         nDataLen, 
		int         nSQLType,
		sb2        *paeInd = NULL );

	bool bindObject( 
		const char *pszPlaceName, 
		void       *pahObject,
		OCIType    *hTDO, 
		void      **papIndicators );

	char *pszCommandText;

	bool prepare( const char * pszStatement );
	bool execute( 
		const char * pszStatement,
		int nMode = -1);

	void   clean();

	GTFields *getResultDefn() { return poDefn; }

	char **simpleFetchRow();

	int getResultColumnCount() { return nRawColumnCount;}

private:    
	GTOCISession  *poSession;
	OCIStmt       *hStatement;
	GTFields      *poDefn;
	char         **papszCurColumn;
	char         **papszCurImage;
	sb2           *panCurColumnInd;
	int            nRawColumnCount;
	int           *panFieldMap;
};

class GTOCIStringBuf 
{
public:
	GTOCIStringBuf();
	~GTOCIStringBuf();

	void makeRoomFor(  int nCharacters );
	void append( const char *pszNewText );
	void appendf( int nMax, const char *pszFormat, ... );
	char *stealString();

	char getLast();
	char *getEnd() { updateEnd(); return pszString + nLen; }
	char *getString() { return pszString; }
	void clear();
	void updateEnd();

public:
	char *pszString;
	int  nLen;
	int  nBufSize;
};

}
#endif