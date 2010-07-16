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
    >>> e = Elf(expected_size)
    >>> e.add("ASDF")
    >>> "ASDF" in e
    True

.. _`Bloom::Faster`: http://search.cpan.org/~palvaro/Bloom-Faster-1.6/lib/Bloom/Faster.pm

