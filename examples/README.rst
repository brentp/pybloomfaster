Filter to Unique FASTQ
======================

It's common to filter repeated reads from a fastq or fasta file. With
recent high-throughput sequencing output reaching ... hi throughput,
it's no longer feasible to hold all reads in an in-memory datastructure
to determine uniqueness. 

`fastq-unique.py` is an example of using a bloom-filter as a pre-filter
for a more memory intensive python set. It filters to unique sequences
and prints information on the number of false positives in the bloom filter.

::

    $ time python fastq-unique.py < /usr/local/src/methylcode/data/dme/en.wt.1.fastq > unique.fastq
    36754782 records in file
    checking 2525155 potential duplicates in a python set
    4699 false-positive duplicates in the bloom filter

    real    11m31.077s
    user    11m1.937s
    sys 0m23.093s

