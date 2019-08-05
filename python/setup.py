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
        #glob.glob(os.path.join('src_sunbeam', '*.cpp')),
        [
                'src_sunbeam/connection.cpp',
                'src_sunbeam/deck.cpp',
                'src_sunbeam/deck_keyword.cpp',
                'src_sunbeam/eclipse_3d_properties.cpp',
                'src_sunbeam/eclipse_config.cpp',
                'src_sunbeam/eclipse_grid.cpp',
                'src_sunbeam/eclipse_state.cpp',
                'src_sunbeam/group.cpp',
                'src_sunbeam/group_tree.cpp',
                'src_sunbeam/parser.cpp',
                'src_sunbeam/schedule.cpp',
                'src_sunbeam/sunbeam_state.cpp',
                'src_sunbeam/table_manager.cpp',
                'src_sunbeam/well.cpp',
                'src_sunbeam/sunbeam.cpp'
        ],
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
