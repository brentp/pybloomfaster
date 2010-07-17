#!/usr/bin/env python
# -*- coding: utf-8 -*-

from setuptools import setup
from distutils.extension import Extension
import os, sys

if not os.path.exists('src/cybloomfaster.c'):
    print "run cython src/cybloomfaster.c"
    sys.exit()

setup(
    ext_modules = [Extension("cybloomfaster", ["src/cybloomfaster.c", "src/bloom.c", "src/suggestions.c"],
                            include_dirs=["src/"], extra_objects=['src/bloom.h'],
                             depends=["src/bloom.h", "src/jenkins.h"])
                  ],
    name = 'bloomfaster',
    version = '0.1.1',
    author = 'brentp',
    author_email = 'bpederse@gmail.com',
    description = 'Cython wrapper for BloomFaster',
    tests_require=['nose'],
    test_suite='nose.collector',

    long_description = open('README.rst').read(),
    url          = 'http://',
    download_url = 'http://',
    classifiers  = ['Development Status :: 3 - Alpha',
                    'Intended Audience :: Developers',
                    'License :: OSI Approved :: BSD License',
                    'Operating System :: OS Independent',
                    'Programming Language :: Python',
                    'Programming Language :: C',
                    'Topic :: Database :: Database Engines/Servers',
                   ],
    package_dir = {'bloomfaster': "bloomfaster"},
    packages = ['bloomfaster'],
)
