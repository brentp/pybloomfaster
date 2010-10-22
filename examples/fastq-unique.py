"""
Reads a FastQ file from stdin and writes a file with unique records to sdout
usage:
    %s < in.fastq > out.unique.fastq
"""
from bloomfaster import Elf
import collections
import sys
__doc__ %= sys.argv[0]
if len(sys.argv) > 1:
    print sys.argv
    print __doc__
    sys.exit()

records = sum(1 for _ in sys.stdin) / 4
print >>sys.stderr, records, "records in file"

# say 1 out of 1000 is false positive.
bloom = Elf(records, error_rate=1e-3)
sys.stdin.seek(0)
readline = sys.stdin.readline

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
sys.stdin.seek(0)
checks = frozenset(checks)
print >>sys.stderr, "checking %s potential duplicates in a python set" \
                                            % len(checks)
putative_false_positives = collections.defaultdict(int)
while True:
    header = readline().rstrip()
    if not header: break
    seq = readline().rstrip()
    plus = readline().rstrip()
    qual = readline().rstrip()
    # it appeared only once, so just print it.
    if not seq in checks:
        print "\n".join((header, seq, plus, qual))
        continue
    # it appeared in the bloom-filter > 1x, so track and make sure not
    # to print any others with same sequence.
    putative_false_positives[seq] += 1
    if putative_false_positives[seq] > 1:
        continue
    print "\n".join((header, seq, plus, qual))

false_positives = sum(1 for count in putative_false_positives.values() \
                                                        if count == 1)
print >>sys.stderr,  false_positives, "false-positive duplicates in the bloom filter"
