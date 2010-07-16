"""
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
    struct bloomstat stat;
    randoms random_nums;
} bloom;


/* public interface */
int bloom_init(bloom *bloom,BIGNUM size,BIGNUM capacity, float error_rate,
           int hashes,hash_t hash,int flags);
int bloom_check(bloom *bloom,char *str);
int bloom_add(bloom *bloom,char *str);
int bloom_test(bloom *bloom,char *str,int MODE);
void bloom_destroy(bloom *bloom);
int bloom_serialize(bloom *bloom, char *fname);
bloom * bloom_deserialize(char *fname);

"""
cimport stdlib
ctypedef unsigned long long BIGNUM

cdef extern from "bloom.h":

    #struct bloomstat:
    ctypedef struct bloomstat "struct bloomstat":
        int ideal_hashes
        int elements

    ctypedef struct bloom:
        bloomstat stat



    # the hash_function must take a string and return a bignum
    ctypedef BIGNUM (*hash_t)(char *) except -1

    int bloom_init(bloom * bloom, BIGNUM size, BIGNUM capacity, float error_rate,
                int hashes, hash_t hash, int_flags)

    int bloom_check(bloom * bloom, char *str)
    int bloom_add(bloom * bloom, char *str)
    int bloom_test(bloom * bloom, char *str, int MODE)
    void bloom_destroy(bloom * bloom)
    int bloom_serialize(bloom * bloom, char *fname)
    bloom * bloom_deserialize(char *fname)

    int get_suggestion(bloomstat *stats, BIGNUM n, double e)



cdef class Elf:
    cdef bloom* _bloom

    def __cinit__(self, BIGNUM n_elements, float error_rate=0.001):
        self._bloom = <bloom *>stdlib.malloc(sizeof(bloom))
        cdef bloomstat *stats
        stats = &self._bloom.stat
        get_suggestion(stats, n_elements, error_rate)

        bloom_init(self._bloom, self._bloom.stat.elements, n_elements, error_rate,
                   self._bloom.stat.ideal_hashes, NULL, 1)

    def add(self, astr):
        bloom_add(self._bloom, astr)

    def __contains__(self, astr):
        return bool(bloom_test(self._bloom, astr, 1))

    def __del__(self):
        bloom_destroy(self._bloom)
