
#include "triangulate.h"
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
