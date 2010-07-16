#!/usr/bin/env python
# -*- coding: utf-8 -*-

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import os, sys

if not os.path.exists('src/cybloomfaster.c'):
    print "run cython src/cybloomfaster.c"
    sys.exit()

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension("cybloomfaster", ["src/cybloomfaster.c", "src/bloom.c", "src/suggestions.c"],
                            include_dirs=["src/"], extra_objects=['src/bloom.h'],
                             depends=["src/bloom.h", "src/jenkins.h"])
                  ],
    name = 'pybloomfaster',
    version = '0.1',
    author = 'brentp',
    author_email = 'bpederse@gmail.com',
    description = 'Cython wrapper for BloomFaster',
    #tests_require=['nose'],
    #test_suite='nose.collector',

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
    package_dir = {'pybloomfaster': "pybloomfaster"},
    packages = ['pybloomfaster'],
)
