from setuptools import setup, find_packages

from setuptools import Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools

import glob
import os

ext_modules = [
    Extension(
        'libsunbeam',
        glob.glob(os.path.join('src_sunbeam', '*.cpp')),
        libraries=['opmcommon', 'boost_filesystem', 'boost_regex', 'ecl'],
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
    tests_suite=['tests'],
    setup_requires=["pytest-runner", 'setuptools_scm'],
)
