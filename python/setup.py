from setuptools import setup, find_packages






setup(
    name='Sunbeam',
    package_dir = {'': 'python'},
    packages=[
                'sunbeam',
                'sunbeam.tools',
                'sunbeam.deck',
            ],
    license='Open Source',
    zip_safe=False,
    tests_suite=['tests'],
    setup_requires=["pytest-runner", 'setuptools_scm'],
)
