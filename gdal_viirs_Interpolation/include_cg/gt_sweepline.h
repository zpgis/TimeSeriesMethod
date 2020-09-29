/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  扫描线
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-09-21
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c) 保留所有权利
****************************************************************/
#ifndef GT_SWEEPLINE_H_INCLUDED
#define GT_SWEEPLINE_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_rbtree.h"

namespace gts{

#define LEFT    0
#define RIGHT   1
 
// EventQueue Class
	
// Event element data struct
typedef struct _event Event;
struct _event {
    int      edge;         // polygon edge i is V[i] to V[i+1]
    int      type;         // event type: LEFT or RIGHT vertex
	GTRawPoint*   eV;           // event vertex
};

// the EventQueue is a presorted array (no insertions needed)
class EventQueue {
    int      ne;               // total number of events in array
    int      ix;               // index of next event on queue
    Event*   Edata;            // array of all events
    Event**  Eq;               // sorted list of event pointers
public:
    EventQueue(GTRing& P);     // constructor
    ~EventQueue(void)          // destructor
        { delete Eq; delete Edata;}

    Event*   next();                    // next event on queue
};


//===================================================================
 

// SweepLine Class

// SweepLine segment data struct
typedef struct _SL_segment SLseg;
struct _SL_segment {
    int			edge;         // polygon edge i is V[i] to V[i+1]
    GTRawPoint	lP;           // leftmost vertex point
    GTRawPoint	rP;           // rightmost vertex point
    SLseg*		above;        // segment above this one
    SLseg*		below;        // segment below this one
};

// the Sweep Line itself
class GTS_DLL GTSweepLine {
    int      nv;           // number of vertices in polygon
    GTRing*  Pn;           // initial Polygon
    GTRBTree<SLseg*>      Tree;         // balanced binary tree
public:
    GTSweepLine(GTRing& P)           // constructor
	{ nv = P.getPointCount(); Pn = &P; }
    ~GTSweepLine(void)               // destructor
	{ Tree.ClearTree();}

    SLseg*   add( Event* );
    SLseg*   find( Event* );
    int      intersect( SLseg*, SLseg* );
    void     remove( SLseg* );
};

}
#endif