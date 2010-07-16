===============================
pybloomfaster: fast bloomfilter
===============================


:Author: Brent Pedersen (brentp)
:Email: bpederse@gmail.com
:License: MIT

.. contents ::

Implementation
==============

Simple cython wrapper to the code from `Bloom::Faster`_ perl module.
It's called 'Elf' because 'ELF' is an early flowering mutant in Arabidopsis.
So Bloom Fast => Early Flowering => Elf.

Usage
=====
::
  
    >>> from pybloomfaster import Elf

    >>> expected_size = 1000
    >>> error_rate = 1e-10
    >>> e = Elf(expected_size, error_rate=error_rate)
    >>> e.add("ASDF")
    >>> "ASDF" in e
    True

    >>> e.save("some.elf")
    >>> f = Elf.load("some.elf")

    >>> "ASDF" in f
    True
    >>> "AA" in f
    False

.. _`Bloom::Faster`: http://search.cpan.org/~palvaro/Bloom-Faster-1.6/lib/Bloom/Faster.pm

