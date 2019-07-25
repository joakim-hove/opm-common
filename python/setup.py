from setuptools import setup, find_packages

from setuptools import Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools

import glob
import os


"""
ext_modules = [
    Extension(
        'libsunbeam',
        [
         'src/eclipse_state.cpp',
         'src/deck_keyword.cpp',
         'src/deck.cpp',
         'src/well.cpp',
         'src/sunbeam_state.cpp',
         'src/schedule.cpp',
         'src/connection.cpp',
         'src/eclipse_config.cpp',
         'src/table_manager.cpp',
         'src/eclipse_grid.cpp',
         'src/group.cpp',
         'src/group_tree.cpp',
         'src/eclipse_3d_properties.cpp',
         'src/parser.cpp',
         'src/sunbeam.cpp',
        ],
        include_dirs=[
            'src',
            '/home/stf/ert/install/include',
            '/home/stf/opm/opm-common',
            '/home/stf/opm/opm-common/build/include',
            'pybind11/include'
        ],
        library_dirs=['/usr/X11R6/lib'],
        language='c++'
    ),
]
"""

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
        libraries=['opmcommon'],
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
