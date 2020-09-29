/****************************************************************
*
* Project:  ������Ϣ�����߼�
* Purpose:  ���ζ���֮��Ĳ��������
* 
* Author:   ������, fanjf@lreis.ac.cn 2011��-05��-11��
*
****************************************************************
* Copyright (c) All rights reserved.
* ��Ȩ����   (c) ��������Ȩ��
****************************************************************/
#ifndef GTSGEOMETRYBOOLEAN_H_INCLUDED
#define GTSGEOMETRYBOOLEAN_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_geometryanalysis.h"

namespace gts{

class GTS_DLL GTGeometryBoolean
{
public:
	GTGeometryBoolean(void);
	~GTGeometryBoolean(void);

	/*���뻷*/
	static int ringAndRingBoolean(
		GTRing                   *pRing1,
		GTRing                   *pRing2,
		enumGTBooleanType         blType,
		std::vector<GTRing *>    &ring_list,       /*���ָ������, ע���ͷ�*/
		std::vector<int>         &island_mark,     /*0Ϊ�ⲿ, 1Ϊ�ڲ�*/
		bool                      bCheck = false); /*�Ƿ����˻����*/ 
	

private:

};

}
#endif