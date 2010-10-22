bloomfaster: fast bloomfilter
=============================


:Author: Brent Pedersen (brentp)
:Email: bpederse@gmail.com
:License: MIT

Implementation
==============

Simple cython wrapper to the C-code from `Bloom::Faster`_ perl module.
It's called 'Elf' because 'ELF' is an early flowering mutant in Arabidopsis.
So Bloom Fast => Early Flowering => Elf.

It is tune-able for the number of false positives.

Usage
=====
::
  
    >>> from bloomfaster import Elf

    >>> expected_size = 1000
    >>> e = Elf(expected_size, error_rate=1e-10)
    >>> e.add("ASDF")
    >>> "ASDF" in e
    True

    >>> e.update(("A", "B", "C", "D", "E", "F"))
    >>> "B" in e
    True

serialization::

    >>> e.save("some.elf")
    >>> f = Elf.load("some.elf")

    >>> "ASDF" in f
    True
    >>> "AA" in f
    False


Tests
=====
::
    
    $ python setup.py test

or with nose::

    $ nosetests --with-doctest --doctest-extension="pyx" --doctest-extension="rst"

.. _`Bloom::Faster`: http://search.cpan.org/~palvaro/Bloom-Faster-1.6/lib/Bloom/Faster.pm

