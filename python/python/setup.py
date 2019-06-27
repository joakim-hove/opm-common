from setuptools import setup, find_packages






setup(
    name='Sunbeam',
    packages=[
                'sunbeam',
                'sunbeam.tools',
                'sunbeam.deck',
            ],
    license='Open Source',
    zip_safe=False,
    tests_require=['pytest'],
    tests_suite=['tests'],
    setup_requires=["pytest-runner", 'setuptools_scm'],
)
