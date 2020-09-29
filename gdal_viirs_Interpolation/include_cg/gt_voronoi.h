/****************************************************************
*
* Project:  地理信息处理工具集
* Purpose:  voronoi图
* Author:   范俊甫, fanjf@lreis.ac.cn,2011-09-22
*
****************************************************************
* Copyright (c) All rights reserved.
* 版权所有   (c)  保留所有权利
****************************************************************/
#ifndef GT_VORONOI_H_INCLUDED
#define GT_VORONOI_H_INCLUDED

#include "gts_cg_port.h"

namespace gts{

/*自有的数据结构*/
#ifndef NULL
#define NULL 0
#endif

#define DELETED -2

typedef struct tagFreenode
    {
    struct tagFreenode * nextfree;
    } Freenode ;


typedef struct tagFreelist
    {
    Freenode * head;
    int nodesize;
    } Freelist ;

typedef struct tagPoint
    {
    double x ;
    double y ;
    } Point ;

/* structure used both for sites and for vertices */

typedef struct tagSite
    {
    Point coord ;
    int sitenbr ;
    int refcnt ;
    } Site ;


typedef struct tagEdge
    {
    double a, b, c ;
    Site * ep[2] ;
    Site * reg[2] ;
    int edgenbr ;
    } Edge ;

#define le 0
#define re 1

typedef struct tagHalfedge
    {
    struct tagHalfedge * ELleft ;
    struct tagHalfedge * ELright ;
    Edge * ELedge ;
    int ELrefcnt ;
    char ELpm ;
    Site * vertex ;
    double ystar ;
    struct tagHalfedge * PQnext ;
    } Halfedge ;

struct Point3
{
	double x,y,z;
	int count;
};

struct VertexLink
{
	Point coord;
	Point v[3];
	int count;
};

class GTS_DLL GTVoronoi
{
public:
	GTVoronoi(void);
	~GTVoronoi(void);

	bool generateVoronoi(std::vector<GTPoint *>& poiny_list, GTEnvelop& env, double minDist,bool bRemoveRepeatoints = false);

	bool getVoronoi(std::vector<GTLineString *>& line_list, bool bNeedBorder = true);

	bool generatePolygon(std::vector<GTPolygon *>& poly_list, bool bAll = true);

	bool getNextVertexPair(double& x1, double& y1, double& x2, double& y2);	

	void reset();
private:

	Site *nextone();
	void insertVertexAddress(long vertexNum, Site* address);
	void insertVertexLink(long vertexNum, long vertexLinkedTo);
	void generateVertexLinks();
	void cleanup();
	void initParam();

	int ELhashsize ;
	Site * bottomsite ;
	Freelist hfl ;
	Halfedge * ELleftend, * ELrightend, **ELhash ;

	std::vector<Freenode *> mem;

	double deltax, deltay ;
	int nsites, nedges, sqrt_nsites, nvertices ;
	Freelist sfl, efl ;

	int PQmin, PQcount, PQhashsize ;
	Halfedge * PQhash ;

	int sorted, triangulate, plot, debug, siteidx ;
	double xmin, xmax, ymin, ymax ;
	Site * sites ;

	int total_alloc ;

	double pxmin, pxmax, pymin, pymax, cradius;

	double		minDistanceBetweenSites;

	Site**		vertices;
	long		sizeOfVertices ;

	VertexLink* finalVertexLinks;
	long 		sizeOfFinalVertexLinks;
	long		currentVertexLink;

	Point3*		vertexLinks;
	long		sizeOfVertexLinks;

	Point*		finalVertices;	
	long		sizeOfFinalVertices ;	
	long 		currentVertex;

	double		borderMinX, borderMaxX, borderMinY, borderMaxY;

	/* voronoi.c */
	void voronoi() ;

	/* edgelist.c */
	void ELinitialize(void) ;
	Halfedge * HEcreate(Edge *, int) ;
	void ELinsert(Halfedge *, Halfedge *) ;
	Halfedge * ELgethash(int) ;
	Halfedge * ELleftbnd(Point *) ;
	void ELdelete(Halfedge *) ;
	Halfedge * ELright(Halfedge *) ;
	Halfedge * ELleft(Halfedge *) ;
	Site * leftreg(Halfedge *) ;
	Site * rightreg(Halfedge *) ;
	

	/* geometry.c */
	void geominit(void) ;
	Edge * bisect(Site *, Site *) ;
	Site * intersect(Halfedge *, Halfedge *) ;
	int right_of(Halfedge *, Point *) ;
	void endpoint(Edge *, int, Site *) ;
	double dist(Site *, Site *) ;
	void makevertex(Site *) ;
	void deref(Site *) ;
	void ref(Site *) ;

	/* heap.c */
	void PQinsert(Halfedge *, Site *, double) ;
	void PQdelete(Halfedge *) ;
	int PQbucket(Halfedge *) ;
	int PQempty(void) ;
	Point PQ_min(void) ;
	Halfedge * PQextractmin(void) ;
	void PQinitialize(void) ;	

	/* getopt.c */

	/* memory.c */
	void freeinit(Freelist *, int) ;
	char *getfree(Freelist *) ;
	void makefree(Freenode *, Freelist *) ;
	char *myalloc(unsigned) ;

	/* output.c */
	/*void openpl(void) ;
	void line(double, double, double, double) ;
	void circle(double, double, double) ;
	void range(double, double, double, double) ;*/
	/*void out_bisector(Edge *) ;*/
	void out_ep(Edge *) ;
	/*void out_vertex(Site *) ;
	void out_site(Site *) ;
	void out_triple(Site *, Site *, Site *) ;*/
	void plotinit(void) ;
	void clip_line(Edge *) ;
};

}
#endif
