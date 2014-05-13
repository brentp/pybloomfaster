"""
Reads a FastQ file and writes a file with unique records to sdout
usage:
    %s  in.fastq > out.unique.fastq
"""
from __future__ import print_function

import sys
import gzip
from bloomfaster import Elf
import collections
__doc__ %= sys.argv[0]

if len(sys.argv) != 2:
    print(sys.argv)
    print(__doc__)
    sys.exit()

xopen = lambda f: gzip.open(f) if f.endswith(".gz") else open(f) 

records = sum(1 for _ in xopen(sys.argv[1])) / 4
print(records, "records in file", file=sys.stderr)

# say 1 out of 1000 is false positive.
bloom = Elf(records, error_rate=1e-3)
fh = xopen(sys.argv[1])
readline = fh.readline

checks = []
header = readline().rstrip()
while header:
    seq = readline().rstrip()
    readline(); readline()
    if seq in bloom:
        checks.append(seq)
    bloom.add(seq)
    header = readline().rstrip()

# now checks contains anything that could be a duplicate according to
# the bloomfilter. for some, they were false positives.
# for actual duplicated, just choose the first, but can also sort by quality.
fh.seek(0)
checks = frozenset(checks)
print("checking %s potential duplicates in a python set" \
                                            % len(checks), file=sys.stderr)
putative_false_positives = collections.defaultdict(int)
while True:
    header = readline().rstrip()
    if not header: break
    seq = readline().rstrip()
    plus = readline().rstrip()
    qual = readline().rstrip()
    # it appeared only once, so just print it.
    if not seq in checks:
        print("\n".join((header, seq, plus, qual)))
        continue
    # it appeared in the bloom-filter > 1x, so track and make sure not
    # to print any others with same sequence.
    putative_false_positives[seq] += 1
    if putative_false_positives[seq] > 1:
        continue
    print("\n".join((header, seq, plus, qual)))

false_positives = sum(1 for count in list(putative_false_positives.values()) \
                                                        if count == 1)
print(false_positives, "false-positive duplicates in the bloom filter", file=sys.stderr)
