#include "triangulate.h"
#include <cstring>
#include <cmath>
#include <ctime>

#ifdef __STDC__
extern double log2(double);
#else
extern double log2();
#endif

static int choose_idx;
static int permute[SEGSIZE];


/* Generate a random permutation of the segments 1..n */

int generate_random_ordering(int n)
{
  register int i;
  int m, st[SEGSIZE], *p;

  choose_idx = 1;
  srand((unsigned)time(NULL));

  for (i = 0; i <= n; i++)
    st[i] = i;

  p = st;
  for (i = 1; i <= n; i++, p++)
    {
      m = rand() % (n + 1 - i) + 1;
      permute[i] = p[m];
      if (m != 1)
  p[m] = p[1];
    }
  return 0;
}


/* Return the next segment in the generated random ordering of all the */
/* segments in S */

int choose_segment()
{
  int i;

#ifdef DEBUG
  fprintf(stderr, "choose_segment: %d\n", permute[choose_idx]);
#endif
  return permute[choose_idx++];
}

int math_logstar_n(int n)
{
  register int i;
  double v;

  for (i = 0, v = (double) n; v >= 1; i++)
    v = log2(v);

  return (i - 1);
}

int math_N(int n, int h)
{
  register int i;
  double v;

  for (i = 0, v = (int) n; i < h; i++)
    v = log2(v);

  return (int) ceil((double) 1.0*n/v);
}

static int initialise(int n)
{
  register int i;

  for (i = 1; i <= n; i++)
    seg[i].is_inserted = FALSE;
  generate_random_ordering(n);
  return 0;
}

int triangulate_polygon(int npoints, double (*vertices)[2],int (*triangles)[3])
{
  int i=1,j,first,last,n,nmonpoly;
  memset((void *)seg, 0, sizeof(seg));
  first = i;
  last = first + npoints - 1;
  for (j = 0; j < npoints; j++, i++)
	{
	  seg[i].v0.x = vertices[i][0];
	  seg[i].v0.y = vertices[i][1];
    if (i == last)
	    {
	      seg[i].next = first;
	      seg[i].prev = i-1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  else if (i == first)
	    {
	      seg[i].next = i+1;
	      seg[i].prev = last;
	      seg[last].v1 = seg[i].v0;
	    }
	  else
	    {
	      seg[i].prev = i-1;
	      seg[i].next = i+1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  seg[i].is_inserted = FALSE;
	}
  n = i-1;
  initialise(n);
  construct_trapezoids(n);
  nmonpoly = monotonate_trapezoids(n);
  triangulate_monotone_polygons(n, nmonpoly, triangles);
  return 0;
}

int is_point_inside_polygon(double vertex[2])
{
  point_t v;
  int trnum, rseg;
  trap_t *t;

  v.x = vertex[0];
  v.y = vertex[1];

  trnum = locate_endpoint(&v, &v, 1);
  t = &tr[trnum];

  if (t->state == ST_INVALID)
    return FALSE;

  if ((t->lseg <= 0) || (t->rseg <= 0))
    return FALSE;
  rseg = t->rseg;
  return _greater_than_equal_to(&seg[rseg].v1, &seg[rseg].v0);
}
