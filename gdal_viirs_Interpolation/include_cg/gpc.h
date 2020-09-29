/*
===========================================================================

Project:   Generic Polygon Clipper

A new algorithm for calculating the difference, intersection,
exclusive-or or union of arbitrary polygon sets.

File:      gpc.h
Author:    Alan Murta (email: gpc@cs.man.ac.uk)
Version:   2.32
Date:      17th December 2004

Copyright: (C) Advanced Interfaces Group,
University of Manchester.

This software is free for non-commercial use. It may be copied,
modified, and redistributed provided that this copyright notice
is preserved on all copies. The intellectual property rights of
the algorithms used reside with the University of Manchester
Advanced Interfaces Group.

You may not use this software, in whole or in part, in support
of any commercial product without the express consent of the
author.

There is no warranty or other guarantee of fitness of this
software for any purpose. It is provided solely "as is".

===========================================================================
*/

#ifndef __gpc_h
#define __gpc_h

#include <stdio.h>


/*
===========================================================================
Constants
===========================================================================
*/

/* Increase GPC_EPSILON to encourage merging of near coincident edges    */

#define GPC_EPSILON (DBL_EPSILON)

#define GPC_VERSION "2.32"

#ifdef _MSC_VER
#   pragma warning(disable: 4996)
#   define _SCL_SECURE_NO_WARNING
#   define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef GPC_DLL
#if defined(__WIN32__) || defined(_WIN32) || defined(_WINDOWS)//for window c++ compiler
#   define GPC_DLL __declspec(dllexport)
#else              //for gcc
#   define GPC_DLL __attribute__ ((visibility("default")))
#endif
#endif

/*
===========================================================================
Constants
===========================================================================
*/

#ifndef TRUE
#define FALSE              0
#define TRUE               1
#endif

#define LEFT               0
#define RIGHT              1

#define ABOVE              0
#define BELOW              1

#define CLIP               0
#define SUBJ               1

#define INVERT_TRISTRIPS   FALSE


/*
===========================================================================
Macros 
===========================================================================
*/

#define EQ(a, b)           (fabs((a) - (b)) <= GPC_EPSILON)

#define PREV_INDEX(i, n)   ((i - 1 + n) % n)
#define NEXT_INDEX(i, n)   ((i + 1    ) % n)

#define OPTIMAL(v, i, n)   ((v[PREV_INDEX(i, n)].y != v[i].y) || \
	(v[NEXT_INDEX(i, n)].y != v[i].y))

#define FWD_MIN(v, i, n)   ((v[PREV_INDEX(i, n)].vertex.y >= v[i].vertex.y) \
	&& (v[NEXT_INDEX(i, n)].vertex.y > v[i].vertex.y))

#define NOT_FMAX(v, i, n)   (v[NEXT_INDEX(i, n)].vertex.y > v[i].vertex.y)

#define REV_MIN(v, i, n)   ((v[PREV_INDEX(i, n)].vertex.y > v[i].vertex.y) \
	&& (v[NEXT_INDEX(i, n)].vertex.y >= v[i].vertex.y))

#define NOT_RMAX(v, i, n)   (v[PREV_INDEX(i, n)].vertex.y > v[i].vertex.y)

#define VERTEX(e,p,s,x,y)  {add_vertex(&((e)->outp[(p)]->v[(s)]), x, y); \
	(e)->outp[(p)]->active++;}

#define P_EDGE(d,e,p,i,j)  {(d)= (e); \
	do {(d)= (d)->prev;} while (!(d)->outp[(p)]); \
	(i)= (d)->bot.x + (d)->dx * ((j)-(d)->bot.y);}

#define N_EDGE(d,e,p,i,j)  {(d)= (e); \
	do {(d)= (d)->next;} while (!(d)->outp[(p)]); \
	(i)= (d)->bot.x + (d)->dx * ((j)-(d)->bot.y);}

#define MALLOC(p, b, s, t) {if ((b) > 0) { \
	p= (t*)malloc(b); if (!(p)) { \
	fprintf(stderr, "gpc malloc failure: %s\n", s); \
	exit(0);}} else p= NULL;}

#define FREE(p)            {if (p) {free(p); (p)= NULL;}}


/*
===========================================================================
Public Data Types
===========================================================================
*/

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum                        /* Set operation type                */
{
	GPC_DIFF,                         /* Difference                        */
	GPC_INT,                          /* Intersection                      */
	GPC_XOR,                          /* Exclusive or                      */
	GPC_UNION                         /* Union                             */
} gpc_op;

typedef struct                      /* Polygon vertex structure          */
{
	double              x;            /* Vertex x component                */
	double              y;            /* vertex y component                */
} gpc_vertex;

typedef struct                      /* Vertex list structure             */
{
	int                 num_vertices; /* Number of vertices in list        */
	gpc_vertex         *vertex;       /* Vertex array pointer              */
} gpc_vertex_list;

typedef struct                      /* Polygon set structure             */
{
	int                 num_contours; /* Number of contours in polygon     */
	int                *hole;         /* Hole / external contour flags     */
	gpc_vertex_list    *contour;      /* Contour array pointer             */
} gpc_polygon;

typedef struct                      /* Tristrip set structure            */
{
	int                 num_strips;   /* Number of tristrips               */
	gpc_vertex_list    *strip;        /* Tristrip array pointer            */
} gpc_tristrip;


/*
===========================================================================
Public Function Prototypes
===========================================================================
*/
void GPC_DLL gpc_add_contour         (gpc_polygon     *polygon,
							  gpc_vertex_list *contour,
							  int              hole);

void GPC_DLL gpc_polygon_clip        (gpc_op           set_operation,
							  gpc_polygon     *subject_polygon,
							  gpc_polygon     *clip_polygon,
							  gpc_polygon     *result_polygon);

void GPC_DLL gpc_tristrip_clip       (gpc_op           set_operation,
							  gpc_polygon     *subject_polygon,
							  gpc_polygon     *clip_polygon,
							  gpc_tristrip    *result_tristrip);

void GPC_DLL gpc_polygon_to_tristrip (gpc_polygon     *polygon,
							  gpc_tristrip    *tristrip);

void GPC_DLL gpc_free_polygon        (gpc_polygon     *polygon);

void GPC_DLL gpc_free_tristrip       (gpc_tristrip    *tristrip);

#ifdef __cplusplus
    }
#endif

#endif

/*
===========================================================================
End of file: gpc.h
===========================================================================
*/
