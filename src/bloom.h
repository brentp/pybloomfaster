
#ifndef _BLOOM
#define _BLOOM


#include <limits.h>

/* leopold, a c implementation of bloom filters 
 *
 * this technique is very useful dealing with large streams of data in
 * limited memory.  
 *
 * by Peter Alvaro, 2006
 */


/* if your arch/os support the "long long" datatype, you can handle
 * VERY large vectors.  it's worth setting BIGNUM to long long and
 * seeing what happens.  
 */

#define BIGNUM unsigned long long
#define BIGNUM_STR "unsigned long"
#define BIGCAST long long
#define TOPLIMIT LONG_MAX

#if !HAVE_SQRTL
#define sqrtl(val) ((long double)sqrt((double)val))
#endif

#define SALTRAND 99999999
#define CONS 1234567

#define HASH_FUNC hash5

typedef struct
{
	BIGNUM index;
	char spot;

} deref;

typedef BIGNUM (*hash_t)(char *str);

typedef struct 
{
	int *num;
	int cnt;
} randoms;

struct bloomstat
{
	BIGNUM elements; /* size of array */
	int ideal_hashes; /* num hash functions */
	BIGNUM capacity; /* number of elements */
	float e; /* max error rate */
} ;

typedef struct
{
	char *vector;
	hash_t hash;
 	BIGNUM inserts;
  //	int ideal_hashes;
        struct bloomstat stat;
	randoms random_nums;
} bloom;

//int verbose;


/* these are modes to test_all() */
#define READ_ONLY 0
#define SET 1
#define BVERBOSE 2

/* errors */

#define ERR_MALLOC 1
#define ERR_OVERFLOW 2
#define ERR_UNKNOWN 3

#define hashsize(n) ((BIGNUM)1<<(n))
#define hashmask(n) (hashsize(n) - 1)


BIGNUM mkprime(BIGNUM startval);

/* public interface */
int bloom_init(bloom *bloom,BIGNUM size,BIGNUM capacity, float error_rate,
	       int hashes,hash_t hash,int flags);
int bloom_check(bloom *bloom,char *str);
int bloom_add(bloom *bloom,char *str);
int bloom_test(bloom *bloom,char *str,int MODE);
void bloom_destroy(bloom *bloom);
int bloom_serialize(bloom *bloom, char *fname);
bloom * bloom_deserialize(char *fname);

/* private interface */
int sketchy_randoms(randoms *rands,int cnt);
int finder (BIGNUM index,deref *dr);
int set(char *big,BIGNUM index);
int test (char *big, BIGNUM index);
BIGNUM bloom_hash(bloom *bloom,char *str, int i);
int bloom_hash_old(bloom *bloom,char *str, int i);

BIGNUM find_close_prime(BIGNUM m);
int get_suggestion(struct bloomstat *stats, BIGNUM n,double e);
int is_prime(BIGNUM m);
void get_rec (struct bloomstat *stat);
BIGNUM report_capacity(bloom *bloom);

#endif
