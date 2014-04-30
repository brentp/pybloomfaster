from libc cimport stdlib
ctypedef unsigned long long BIGNUM

cdef extern from "bloom.h":

    ctypedef struct bloomstat "struct bloomstat":
        int ideal_hashes
        int elements

    ctypedef struct bloom:
        bloomstat stat


    # the hash_function must take a string and return a bignum
    ctypedef BIGNUM (*hash_t)(char *) except -1

    int bloom_init(bloom * bloom, BIGNUM size, BIGNUM capacity, float error_rate,
                int hashes, hash_t hash, int flags)

    int bloom_check(bloom * bloom, char *str)
    int bloom_add(bloom * bloom, char *str)
    int bloom_test(bloom * bloom, char *str, int MODE)
    void bloom_destroy(bloom * bloom)
    int bloom_serialize(bloom * bloom, char *fname)
    bloom * bloom_deserialize(char *fname)

    int get_suggestion(bloomstat *stats, BIGNUM n, double e)


cdef class Elf:
    """
    >>> from bloomfaster import Elf
    >>> e = Elf(1000000, 0.0001)
    >>> e.add("CCC")
    >>> e.add("GGG")
    >>> e.add("FFF")
    >>> "AAA" in e
    False

    >>> "GGG" in e
    True

    >>> e.addmany(["ASDF", "RRR"])
    >>> "RRR" in e
    True

    """
    cdef bloom* _bloom

    def __init__(self, BIGNUM n_elements, float error_rate=0.001):
        self._bloom = <bloom *>stdlib.malloc(sizeof(bloom))
        cdef bloomstat *stats
        stats = &self._bloom.stat
        get_suggestion(stats, n_elements, error_rate)

        bloom_init(self._bloom, self._bloom.stat.elements, n_elements, error_rate,
                   self._bloom.stat.ideal_hashes, NULL, 1)

    def add(self, astr):
        """
        add a single item to the bloom filter
        """
        bloom_add(self._bloom, astr)

    def addmany(self, iterable):
        """
        add many items to the bloom filter without incurring a function call
        for each item
        """
        for astr in iterable:
            bloom_add(self._bloom, astr)

    update = addmany

    def __contains__(self, astr):
        return bool(bloom_test(self._bloom, astr, 1))

    def __del__(self):
        bloom_destroy(self._bloom)


    def save(self, filename):
        """
        save the bloom filter to a file
        """
        bloom_serialize(self._bloom, filename)

    @classmethod
    def load(cls, filename):
        """
        load a previously saved bloomfilter
        """
        return _load(filename)


cdef extern from "pnew.h":
    cdef Elf NEW_ELF "PY_NEW" (object t)

cdef Elf _load(char *filename):
    cdef Elf instance = NEW_ELF(Elf)
    instance._bloom = bloom_deserialize(filename)
    return instance
