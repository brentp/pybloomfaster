#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"

/* dmitriy ryaboy */

int get_suggestion(struct bloomstat *stats, BIGNUM n,double e)
{
        stats->capacity = n;
	//fprintf(stderr,"n is %ld %ld\n",(long)stats->n,(long)n);
        stats->e = e;
        get_rec(stats);
        return 0;
}

BIGNUM find_close_prime(BIGNUM m) {
  if ((m % 2) == 0)
    m+=1;

 while (!is_prime(m)) {
    m +=2;
  }
  return m;
}

void get_rec ( struct bloomstat *stat) {
  /* assuming perfect number of cells, k directly depends on e */
  //printf("start get rec with %ld %f\n",(long)stat->n,stat->e);
  stat->ideal_hashes = (int) log(stat->e)/log(0.5);
  stat->elements = find_close_prime((BIGNUM) 13*stat->capacity*(BIGNUM)stat->ideal_hashes/(BIGNUM)9);
  /*
  recalculate k with the actual m, not the ideal 
  wouldn't need to if it wasn't prime, but that causes problems
  for hash algs
  */
  stat->ideal_hashes = 9*stat->elements/(13*stat->capacity);
  //printf("m is %ld (from %ld),k is %d\n",(long)stat->elements,
  //	 (long)stat->capacity,stat->ideal_hashes);

}

int is_prime(BIGNUM m) {
  BIGNUM a =  (BIGNUM) sqrtl(( long double) m);
  BIGNUM currval=3;
  if (m % 2 == 0 && m!=2)
    return 0;
  while (m % currval != 0 && currval < a) {
    if (m % (currval + 2) == 0) return 0;
    if (m % (currval + 4) == 0) return 0;
    currval +=8;
  }
  return (int) m % currval;
}



