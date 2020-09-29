
/****************************************************************************
* MODULE:       R-Tree library 
*              
* AUTHOR(S):    Antonin Guttman - original code
*               Daniel Green (green@superliminal.com) - major clean-up
*                               and implementation of bounding spheres
*               
* PURPOSE:      Multidimensional index
*
* COPYRIGHT:    (C) 2001 by the GRASS Development Team
*
*               This program is free software under the GNU General Public
*               License (>=v2). Read the file COPYING that comes with GRASS
*               for details.
*****************************************************************************/
/***************Modified by Ting Ma, 12-09-2010******************************/

#ifndef GTRT_INDEX_H_INCLUDED
#define GTRT_INDEX_H_INCLUDED

#ifdef _MSC_VER
#   pragma warning(disable: 4996)
#   define _SCL_SECURE_NO_WARNING
#   define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef GRT_DLL
#if defined(__WIN32__) || defined(_WIN32) || defined(_WINDOWS)//for window c++ compiler
#   define GRT_DLL __declspec(dllexport)
#else              //for gcc
#   define GRT_DLL __attribute__ ((visibility("default")))
#endif
#endif

/* PGSIZE is normally the natural page size of the machine */
#define PGSIZE	512

//#define NUMDIMS	3		/* number of dimensions */

#define NUMDIMS 2 /*修改为支持2维矩形*/

/* typedef float RectReal; */
typedef double RectReal;

/*-----------------------------------------------------------------------------
| Global definitions.
-----------------------------------------------------------------------------*/

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define NUMSIDES 2*NUMDIMS

struct Rect
{
    RectReal boundary[NUMSIDES];	/* xmin,ymin,...,xmax,ymax,... */
};

struct Node;

struct Branch
{
    struct Rect rect;
    struct Node *child;
};

/* max branching factor of a node */
#define MAXCARD (int)((PGSIZE-(2*sizeof(int))) / sizeof(struct Branch))

struct Node
{
    int count;
    int level;			/* 0 is leaf, others positive */
    struct Branch branch[MAXCARD];
};

struct ListNode
{
    struct ListNode *next;
    struct Node *node;
};

/*
 * If passed to a tree search, this callback function will be called
 * with the ID of each data rect that overlaps the search rect
 * plus whatever user specific pointer was passed to the search.
 * It can terminate the search early by returning 0 in which case
 * the search will return the number of hits found up to that point.
 */
#ifdef __cplusplus
extern "C" {
#endif

//typedef int (*SearchHitCallback) (int id, void *arg);

//extern int GRT_DLL RTreeSearch(struct Node *, struct Rect *, SearchHitCallback, void *);
extern int GRT_DLL RTreeInsertRect(struct Rect *, int, struct Node **, int depth);
extern int GRT_DLL RTreeInsertRect1(struct Rect *, struct Node *, struct Node **, int depth);
extern int GRT_DLL RTreeDeleteRect(struct Rect *, int, struct Node **);
extern int GRT_DLL RTreeDeleteRect1(struct Rect *, struct Node *, struct Node **);
extern struct Node GRT_DLL *RTreeNewIndex(void);
extern struct Node GRT_DLL *RTreeNewNode(void);
extern void GRT_DLL RTreeInitNode(struct Node *);
extern void GRT_DLL RTreeFreeNode(struct Node *);
extern void GRT_DLL RTreeDestroyNode(struct Node *);
extern void GRT_DLL RTreePrintNode(struct Node *, int);
extern void GRT_DLL RTreeTabIn(int);
extern struct Rect GRT_DLL RTreeNodeCover(struct Node *);
extern void GRT_DLL RTreeInitRect(struct Rect *);
extern struct Rect GRT_DLL RTreeNullRect(void);
extern RectReal GRT_DLL RTreeRectArea(struct Rect *);
extern RectReal GRT_DLL RTreeRectSphericalVolume(struct Rect *R);
extern RectReal GRT_DLL RTreeRectVolume(struct Rect *R);
extern struct Rect GRT_DLL RTreeCombineRect(struct Rect *, struct Rect *);
extern int GRT_DLL RTreeOverlap(struct Rect *, struct Rect *);
extern void GRT_DLL RTreePrintRect(struct Rect *, int);
extern int GRT_DLL RTreeAddBranch(struct Branch *, struct Node *, struct Node **);
extern int GRT_DLL RTreePickBranch(struct Rect *, struct Node *);
extern void GRT_DLL RTreeDisconnectBranch(struct Node *, int);
extern void GRT_DLL RTreeSplitNode(struct Node *, struct Branch *, struct Node **);

extern int GRT_DLL RTreeSetNodeMax(int);
extern int GRT_DLL RTreeSetLeafMax(int);
extern int GRT_DLL RTreeGetNodeMax(void);
extern int GRT_DLL RTreeGetLeafMax(void);

#ifdef __cplusplus
}
#endif


#endif
