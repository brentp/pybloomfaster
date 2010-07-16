#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "bloom.h"
//#include "hashes.h"
#include "jenkins.h"

int bloom_init(bloom *bloom,BIGNUM size,BIGNUM capacity, float error_rate,
	       int hashes,hash_t hash,int flags)
{
	if (size < 1) {
		fprintf(stderr,"overflow1\n");
		return -1;
	} else {
		/* this may waste a little time, but we need to ensure
		 * that our array has a prime number of elements, even
		 * if we have been requested to do otherwise */
		bloom->stat.elements = find_close_prime(size);
	}
	if (hashes < 1) {
		fprintf(stderr,"hashes was %d,size %llu\n",hashes,size);
		return -1;
	} else {
		bloom->stat.ideal_hashes = hashes;
	}

	if (hash == NULL) {
		bloom->hash = (hash_t)HASH_FUNC;
	} else {
		bloom->hash = hash;
	}
	bloom->inserts = 0;
	bloom->stat.capacity = capacity;
	bloom->stat.e = error_rate;

	if ((flags & BVERBOSE) == BVERBOSE) {
		fprintf(stderr,"bloom_init(%llu,%d) => (%llu,%d)\n",(BIGCAST)size,hashes,(BIGCAST)bloom->stat.elements,bloom->stat.ideal_hashes);
		//verbose = 1;
	} else {
		//verbose = 0;
	}
	if ((size > TOPLIMIT)) {
		fprintf(stderr,"overflow2\n");
		return -2;
	}

	/* allocate our array of bytes.  where m is the size of our desired 
	 * bit vector, we allocate m/8 + 1 bytes. */
	if ((bloom->vector = (char *)malloc(sizeof(char) * ((int)(bloom->stat.elements / 8) + 1))) == NULL) {
		perror("malloc");
		return -1;
	}
	/* generate a collection of random integers, to use later
	 * when salting our keys before hashing them */
	sketchy_randoms(&bloom->random_nums,hashes);

	return 0;
}

void bloom_destroy(bloom *bloom)
{
	free(bloom->vector);
	free(bloom->random_nums.num);
}

int bloom_check(bloom *bloom,char *str)
{
	return bloom_test(bloom,str,READ_ONLY);
}

int bloom_add(bloom *bloom,char *str)
{
	int ret;
	if ((ret = bloom_test(bloom,str,SET)) == 0) {
		bloom->inserts++;	
	}
	return ret;
}

int bloom_test (bloom *bloom,char *str,int mode)
{
	int i,hit;
	BIGNUM ret;

	/* as many times as our ideal hash count dictates, salt our key
	 * and hash it into the bit vector */	
	hit = 1;
	for (i=0; i < bloom->stat.ideal_hashes; i++) {
		ret = bloom_hash(bloom,str,i);
		if (!test(bloom->vector,ret)) {
			hit = 0;
			if (mode == SET) {
				set(bloom->vector,ret);
			} else {
				/* if we are merely testing, we are done. */
				return hit;
			}
		}
	}
	return hit;
}

BIGNUM bloom_hash(bloom *bloom,char *str, int i)
{
	char *newstr;
	char salt[100];
	BIGNUM ret = 0;
	//BIGNUM hash;

	if (i > 0) {
		sprintf(salt,"%d",bloom->random_nums.num[i]);
		if ((newstr = (char *)malloc(sizeof(char) * (strlen(str) + strlen(salt)  + 1))) == NULL) {
			perror("malloc");
			errno = ERR_MALLOC;
			return -1;
		}
		sprintf(newstr,"%s%s",str,salt);
	} else {
		newstr = strdup(str);
	} 

	/* as long as our hash function returns a positive # */	
	ret = (BIGNUM)abs(bloom->hash(newstr)) % (BIGNUM)bloom->stat.elements;
	
	free(newstr);

	return ret;
}

int set(char *big,BIGNUM index)
{
	deref dr;
	
	finder(index,&dr);
	big[dr.index] += dr.spot;
	
	return 0;
}

int test (char *big, BIGNUM index)
{
	deref dr;
	char bucket;
	
	finder(index,&dr);
	bucket = big[dr.index];
	if ((bucket & dr.spot) == dr.spot) {
		return 1;
	} else {
		return 0;
	}
}

int finder (BIGNUM index,deref *dr) 
{

	dr->index = (BIGNUM)(index / 8);
	dr->spot = pow(2,(index % 8));	
	return 0;
}



int sketchy_randoms(randoms *rands,int cnt)
{
	int i;
	srand(CONS);
	if ((rands->num = (int *)malloc(sizeof(int) * (cnt+1))) == NULL) {
		perror("malloc");
		errno = ERR_MALLOC;
		return -1;
	}
	for (i=0; i < cnt;i++) {
		rands->num[i] = 1 + (int)((float)SALTRAND * rand() / (RAND_MAX + 1.0));
	}
	rands->cnt = cnt;
	return 0;
}

BIGNUM report_capacity(bloom *bloom)
{
        return bloom->stat.capacity;
}

char * to_bitstr(bloom *bm)
{
	int i,j;
	char *ptr;
	BIGNUM steps = (bm->stat.elements / 8) + 1;
	char *ret = (char *)malloc(sizeof(char *) * (bm->stat.elements + 2));

	strcpy(ret,"");
	for (i=0,ptr = bm->vector; i < steps; i++,ptr++) {
		for (j=0;j < 8; j++) {
			if ((*ptr >> j) == 1) {
				strcat(ret,"1");		
			} else {
				strcat(ret,"0");		
			}
		}
	}
	return ret;
}

int bloom_serialize(bloom *b,char *fname) 
{
	FILE *fp;
	int r;
	int elems = b->stat.elements / 8 + 1;

	if ((fp = fopen(fname,"w")) ==  NULL) {
		perror("fopen");
		return -1;
	}
	if ((fwrite(b,sizeof(bloom),1,fp)) != 1) {
		perror("fwrite");
		return -1;
	}
	if ((fwrite(b->random_nums.num,sizeof(int) * b->random_nums.cnt,1,fp)) != 1) {
		perror("fwrite");
		return -1;
	}

	if ((r = fwrite(b->vector,sizeof(char)*elems,1,fp)) != 1) {
		//perror("fwrite");
		//fprintf(stderr,"ret=%d vs %d\n",r,elems);
		return -1;
	}

	fclose(fp);
	return 0;
}

bloom * bloom_deserialize(char *fname)
{
	FILE *fp;
	int elems;
	bloom *ret = malloc(sizeof(bloom));
	
	if (ret == NULL) {
		perror("malloc");
		return NULL;
	}
	if ((fp = fopen(fname,"r")) == NULL) {
		perror("fopen");
		return NULL;
	}
	if (fread(ret,sizeof(bloom),1,fp) != 1) {
		perror("fread");
		return NULL;
	}
	//printf("RETelem=%d\n",ret->stat.elements);

	/* function pointers aren't so easy to serialize!  deserialization 
	   (at least for now) will use the default hash function. c api
	   users may reset this, obviously, after calling this function.

	*/

	//printf("hashesr %d\n",ret->ideal_hashes);

	ret->hash = (hash_t)HASH_FUNC;

	//printf("randsr %d\n",ret->random_nums.cnt);

	if ((ret->random_nums.num = (int *)malloc(sizeof(int) * ret->random_nums.cnt)) == NULL) {
		perror("malloc");
		return NULL;
	}

	if ((fread(ret->random_nums.num,sizeof(int)*ret->random_nums.cnt,1,fp)) != 1) {
		perror("fwrite");
		return NULL;
	}

	//printf("randsr %d\n",ret->random_nums.cnt);

	//printf("RETelem=%d\n",ret->stat.elements);
	elems = ret->stat.elements / 8 + 1;

	//printf("elemsr = %d\n",elems);
	if ((ret->vector = (char *)malloc(sizeof(char) * elems)) == NULL) {
		perror("malloc");
		return NULL;
	}

	if (fread(ret->vector,sizeof(char)*elems,1,fp) != 1) {
		perror("fread");
		return NULL;
	}
	fclose(fp);
	return ret;
}
