/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  字符串处理类的定义
* Author:   范俊甫, fanjf@lreis.ac.cn 2011年-01月-31日
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有  (c) 保留所有权利
****************************************************************/
#pragma once
#ifndef GTSTRING_H_INCLUDED
#define GTSTRING_H_INCLUDED

#include "gts_port.h"

#ifndef EQUAL
#if defined(WIN32) || defined(WIN32CE)
#  define EQUALN(a,b,n)           (strnicmp(a,b,n)==0)
#  define EQUAL(a,b)              (stricmp(a,b)==0)
#else
#  define EQUALN(a,b,n)           (strncasecmp(a,b,n)==0)
#  define EQUAL(a,b)              (strcasecmp(a,b)==0)
#endif
#endif

namespace gts{

class GTS_DLL GTString
{
public:
	GTString(void);
	~GTString(void);

public:
	/*二进制转十六进制, 调用GTS_FREE释放*/
	static char *binaryToHex(int nBytes, const unsigned char *pData);

	/*十六进制转二进制, 调用GTS_FREE释放*/
	static unsigned char *hexToBinary(const char *pHex, int *nBytes);

	/*char*的长度*/
	static size_t stringLength(const char* pData);

	/*int转为字符, 如果buf为0，返回值必须调用GTS_FREE释放*/
	static char *intToString(int iValue, char* buf = 0);

	/*float转为字符, 按照给定精度, 如果buf为0，必须调用GTS_FREE释放返回值*/
	static char *floatToString(float fValue, int iWidth, int iPrecision, char* buf = 0);

	/*double转为字符, 按照给定精度, 如果buf为0，必须调用GTS_FREE释放返回值*/
	static char *doubleToString(double dValue, int iWidth, int iPrecision, char* buf = 0);

	/*日期转换为YYYY-MM-DD格式字符串, 如果buf为0，必须调用GTS_FREE释放返回值*/
	static char *dateToString(int year, int month, int day, char* buf = 0);

	/*时间转换为HH::MM::SS格式字符串, 如果buf为0，必须调用GTS_FREE释放返回值*/
	static char *timeToString(int hour, int minute, int second, char* buf = 0);

	/*日期转换为YYYY-MM-DD HH::MM::SS格式字符串, 如果buf为0，必须调用GTS_FREE释放返回值*/
	static char *dateTimeToString(int year, int month, int day, int hour, int minute, int second, char* buf = 0);

	/*由WKB二进制串获取几何类型*/
	static enumGTWKBGeometryType wkbGeomtryType(const unsigned char *pData);

	/*由WKT字符数组获取几何类型*/
	static enumGTWKBGeometryType wktGeomtryType(const char *pData);

	/*逃逸字符串, 调用GTS_FREE释放*/
	/*nLength为保留长度, -1表明全部*/
	/*nScheme为类型: 0-BackslashQuotable, 1-XML, 2-URL, 3-SQL, 4-CVS, 5-MySQL*/
	static char *escapeString(const char* pszInput, int nLength, int nScheme);

	/*反逃逸字符串, 调用GTS_FREE释放*/
	/*pnLength为返回长度*/
	/*nScheme为类型: 0-BackslashQuotable, 1-XML, 2-URL, 3-SQL, 4-CVS*/
	static char *unescapeString(const char* pszInput, int* pnLength, int nScheme);

	/*返回一个二维字符数组的项的数目*/
	static int stringCount(char **ppStringList);

	/*以下函数正在调试, 暂且勿用*/
	static void UTF8ToUnicode(wchar_t* pOut,char *pText);
	static void UnicodeToUTF8(char* pOut,wchar_t* pText);  
	static void UnicodeToGB2312(char* pOut,wchar_t uData);  
	static void GB2312ToUnicode(wchar_t* pOut,char *gbBuffer);  
	static void GB2312ToUTF8(std::string& pOut, char *pText, int pLen);  
	static void UTF8ToGB2312(std::string& pOut, char *pText, int pLen);
	static void CharToWChar(wchar_t* pOut, char *pText);
	static void WCharToChar(char* pOut, wchar_t *pText);
};

}
#endif
