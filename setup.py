#!/usr/bin/env python
# -*- coding: utf-8 -*-

from distutils.core import setup, Extension
from os.path import exists
import os
import sys

bloom_sources = ["src/cybloomfaster.c",
                 "src/bloom.c",
                 "src/suggestions.c"]
if not all(map(exists, bloom_sources)):
    sys.stderr.write("Please install Cython to continue.")
    sys.exit(1)

bloom_module = Extension('cybloomfaster', sources=bloom_sources,
                         include_dirs=['src/'], 
                         depends=['src/bloom.h', 'src/jenkins.h'])
setup(
    ext_modules=[bloom_module],
    name='bloomfaster',
    version='0.1.1',
    author='brentp',
    author_email='bpederse@gmail.com',
    description='Cython wrapper for BloomFaster',
    tests_require=['nose'],
    test_suite='nose.collector',

    long_description=open('README.rst').read(),
    url='http://',
    download_url='http://',
    classifiers=['Development Status :: 3 - Alpha',
                 'Intended Audience :: Developers',
                 'License :: OSI Approved :: BSD License',
                 'Operating System :: OS Independent',
                 'Programming Language :: Python',
                 'Programming Language :: C',
                 'Topic :: Database :: Database Engines/Servers',
                 ],
    package_dir={'bloomfaster': "bloomfaster"},
    packages=['bloomfaster'],
)
