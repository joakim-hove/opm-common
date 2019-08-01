from setuptools import setup, find_packages

from setuptools import Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools

import glob
import os
import ecl

ext_modules = [
    Extension(
        'libsunbeam',
        glob.glob(os.path.join('src', '*.cpp')),
        include_dirs=[
            'src',
            '/home/stf/ert/install/include',
            '/home/stf/opm/opm-common',
            '/home/stf/opm/opm-common/build/include',
            'pybind11/include'
        ],
        library_dirs=['/home/stf/opm/opm-common/build/lib'],
        libraries=['opmcommon', 'boost_filesystem', 'boost_regex'],
        language='c++',
        undef_macros=["NDEBUG"],
    ),
]



setup(
    name='Sunbeam',
    package_dir = {'': 'python'},
    packages=[
                'sunbeam',
                'sunbeam.tools',
                'sunbeam.deck',
            ],
    ext_modules=ext_modules,
    license='Open Source',
    zip_safe=False,
    #tests_suite=['python/tests'],
    setup_requires=["pytest-runner", 'setuptools_scm'],
)
