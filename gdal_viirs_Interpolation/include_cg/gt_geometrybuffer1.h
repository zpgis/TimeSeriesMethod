/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  Buffer处理类(参考GRASS)
* Author:   范俊甫, fanjf@lreis.ac.cn 05-11-2011
*
****************************************************************
* Copyright (c)  All rights reserved.
* 版权所有    (c)  保留所有权利
****************************************************************/
#ifndef GTSGEOMETRYBUFFER1_H_INCLUDED
#define GTSGEOMETRYBUFFER1_H_INCLUDED

#include "gts_cg_port.h"
#include "gt_datasource.h"
#include "gt_geometrysimplicify.h"

namespace gts{

struct GTPGEdge{
	int  v1;            /* first vertex */
	int  v2;            /* second vertex */
	char visited_left;
	char visited_right;
	char winding_left;  /* winding numbers */
	char winding_right;
};

struct GTPGVertex{
	double            x; 
	double            y;
	int               ecount;     /* number of neighbours */
	int               eallocated; /* size of the array bellow */
	struct GTPGEdge **edges;      /* array of pointers */
	double           *angles;     /* precalculated angles with Ox */
};

struct GTPlanarGraph{
	int                vcount;
	struct GTPGVertex *v;
	int                ecount;
	int                eallocated;
	struct GTPGEdge   *e;
};

class GTS_DLL GTGeometryBuffer1
{
public:	
	GTGeometryBuffer1(void);
	~GTGeometryBuffer1(void);

private:
	/*自有数据类型*/
	struct line_pnts
	{
		GTRawPoint *points;
		int         n_points;
	};

	struct intersection_point
	{
		double x;
		double y;
		int    group;/* IPs with very similar dist will be in the same group */
	};

	struct seg_intersection
	{
		int    with;	/* second segment */
		int    ip;	    /* index of the IP */
		double dist;	/* distance from first point of first segment to intersection point (IP) */
	};

	struct seg_intersection_list
	{
		int                      count;
		int                      allocated;
		struct seg_intersection *a;
	};

	struct seg_intersections
	{
		int                           ipcount;
		int                           ipallocated;
		struct intersection_point    *ip;
		int                           ilcount;
		struct seg_intersection_list *il;
		int                           group_count;
	};

	/*Graph的函数*/
	GTPlanarGraph* pg_create_struct (int n, int e);
	void           pg_destroy_struct(GTPlanarGraph *pg);
	int            pg_existsedge    (GTPlanarGraph *pg, int v1, int v2);
	void           pg_addedge       (GTPlanarGraph *pg, int v1, int v2);
	void           pg_addedge1      (GTPGVertex *v, GTPGEdge *e);
	GTPlanarGraph* pg_create        (GTLineString *Points);

	
	/*相交结构的操作函数*/
	struct seg_intersections *create_si_struct(int segments_count);
	void                      destroy_si_struct(struct seg_intersections *si);
	void                      add_ipoint1(struct seg_intersection_list *il, int with, double dist,int ip);
	void                      add_ipoint (GTLineString *Points, int first_seg, int second_seg, double x, double y, struct seg_intersections *si);
	void                      sort_intersection_list(struct seg_intersection_list *il);
	static int                compare(const void *a, const void *b);
	double                    get_epsilon(GTLineString *Points);
	struct seg_intersections *find_all_intersections(GTLineString *Points);

	/*Buffer基础操作函数*/
	void norm_vector        (double x1,  double y1, double x2,   double y2,   double *x,     double *y);
	void rotate_vector      (double x,   double y,  double cosa, double sina, double *nx,    double *ny);
	void elliptic_transform (double x,   double y,  double da,   double db,   double dalpha, double *nx, double *ny);
	void elliptic_tangent   (double x,   double y,  double da,   double db,   double dalpha, double *px, double *py);
	void line_coefficients  (double x1,  double y1, double x2,   double y2,   double *a,     double *b,  double *c);
	int  line_intersection  (double a1,  double b1, double c1,   double a2,   double b2,     double c2,  double *x, double *y);
	double angular_tolerance(double tol, double da, double db);
	
	/*
	void parallel_line(
		struct line_pnts *Points,
		double da, double db, double dalpha, 
		int side, int round, int caps, int looped,
		double tol, struct line_pnts *nPoints);
	*/
public:
	void convolution_line(
	    GTLineString *Points, 
		double da, double db, double dalpha, 
		int side, int round, int caps,
		double tol, GTLineString *nPoints);

	void extract_contour(
	    GTPlanarGraph *pg, 
		GTPGEdge      *first,
		int side, int winding, int stop_at_line_end, 
		GTLineString    *nPoints);

	void extract_outer_contour(
		GTPlanarGraph *pg, 
		int                  side,
	    GTLineString    *nPoints);

	int extract_inner_contour(
		GTPlanarGraph *pg, 
		int                 *winding,
		GTLineString    *nPoints);

	int point_in_buf(
		GTLineString *Points, 
		double px, double py, 
		double da, double db, double dalpha);

	int get_polygon_orientation(
		const double *x, 
		const double *y, 
		int           n);

	void buffer_lines(
		GTLineString  *area_outer, 
		std::vector<GTLineString >& area_isles,
		int isles_count, int side, double da, double db,
		double dalpha, int round, int caps, double tol,
		GTLineString *oPoints,
		std::vector<GTLineString>&  iPoints,
		int                        *inner_count);
// 	    std::vector<GTLineString *>& oPoints, 
// 		std::vector<GTPolygon *>&    iPoints,
// 		int            *inner_count);

	void Vect_line_buffer2(
		GTLineString   *Points, 
		double da, double db, double dalpha, 
		int round, int caps, double tol,
		GTLineString   *oPoints,
		std::vector<GTLineString>& iPoints, 
		int            *inner_count);


// 	void Vect_area_buffer2(
// 		struct Map_info *Map, 
// 		int area, double da, double db,
// 		double dalpha, int round, int caps, double tol,
// 		struct line_pnts **oPoints,
// 		struct line_pnts ***iPoints, int *inner_count);

	void Vect_point_buffer2(
		double px, double py, double da, double db,
		double dalpha, int round, double tol,
		GTLineString **oPoints);

	void Vect_line_parallel2(
		GTLineString *InPoints, double da, double db,
		double dalpha, int side, int round, double tol,
		GTLineString *OutPoints);

	void swap(double *a, double *b);

	int segment_intersection_2d(
		double ax1, double ay1, double ax2, double ay2,
		double bx1, double by1, double bx2, double by2,
		double *x1, double *y1, double *x2, double *y2);

	double dig_distance2_point_to_line(double x, double y, double z,	/* point */
		double x1, double y1, double z1,	/* line segment */
		double x2, double y2, double z2, int with_z,	/* use z coordinate, (3D calculation) */
		double *px, double *py, double *pz,	/* point on segment */
		double *pdist,	/* distance of point on segment from the first point of segment */
		int *status);

	int get_polygon_orientation(GTLineString *Points);

};

}
#endif
