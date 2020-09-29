/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  �ַ���������Ķ���
* Author:   ������, fanjf@lreis.ac.cn 2011��-01��-31��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����  (c) ��������Ȩ��
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
	/*������תʮ������, ����GTS_FREE�ͷ�*/
	static char *binaryToHex(int nBytes, const unsigned char *pData);

	/*ʮ������ת������, ����GTS_FREE�ͷ�*/
	static unsigned char *hexToBinary(const char *pHex, int *nBytes);

	/*char*�ĳ���*/
	static size_t stringLength(const char* pData);

	/*intתΪ�ַ�, ���bufΪ0������ֵ�������GTS_FREE�ͷ�*/
	static char *intToString(int iValue, char* buf = 0);

	/*floatתΪ�ַ�, ���ո�������, ���bufΪ0���������GTS_FREE�ͷŷ���ֵ*/
	static char *floatToString(float fValue, int iWidth, int iPrecision, char* buf = 0);

	/*doubleתΪ�ַ�, ���ո�������, ���bufΪ0���������GTS_FREE�ͷŷ���ֵ*/
	static char *doubleToString(double dValue, int iWidth, int iPrecision, char* buf = 0);

	/*����ת��ΪYYYY-MM-DD��ʽ�ַ���, ���bufΪ0���������GTS_FREE�ͷŷ���ֵ*/
	static char *dateToString(int year, int month, int day, char* buf = 0);

	/*ʱ��ת��ΪHH::MM::SS��ʽ�ַ���, ���bufΪ0���������GTS_FREE�ͷŷ���ֵ*/
	static char *timeToString(int hour, int minute, int second, char* buf = 0);

	/*����ת��ΪYYYY-MM-DD HH::MM::SS��ʽ�ַ���, ���bufΪ0���������GTS_FREE�ͷŷ���ֵ*/
	static char *dateTimeToString(int year, int month, int day, int hour, int minute, int second, char* buf = 0);

	/*��WKB�����ƴ���ȡ��������*/
	static enumGTWKBGeometryType wkbGeomtryType(const unsigned char *pData);

	/*��WKT�ַ������ȡ��������*/
	static enumGTWKBGeometryType wktGeomtryType(const char *pData);

	/*�����ַ���, ����GTS_FREE�ͷ�*/
	/*nLengthΪ��������, -1����ȫ��*/
	/*nSchemeΪ����: 0-BackslashQuotable, 1-XML, 2-URL, 3-SQL, 4-CVS, 5-MySQL*/
	static char *escapeString(const char* pszInput, int nLength, int nScheme);

	/*�������ַ���, ����GTS_FREE�ͷ�*/
	/*pnLengthΪ���س���*/
	/*nSchemeΪ����: 0-BackslashQuotable, 1-XML, 2-URL, 3-SQL, 4-CVS*/
	static char *unescapeString(const char* pszInput, int* pnLength, int nScheme);

	/*����һ����ά�ַ�����������Ŀ*/
	static int stringCount(char **ppStringList);

	/*���º������ڵ���, ��������*/
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
