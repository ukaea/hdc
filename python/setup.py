import os
import sys

from setuptools import setup
from setuptools.command.test import test as TestCommand
from setuptools.extension import Extension


try:
    import numpy
except ImportError:
    raise ImportError('numpy must be installed before installing this package')


class PyTest(TestCommand):
    user_options = [('pytest-args=', 'a', "Arguments to pass to pytest")]

    def initialize_options(self):
        TestCommand.initialize_options(self)
        self.pytest_args = ''

    def run_tests(self):
        import shlex
        # import here, cause outside the eggs aren't loaded
        import pytest
        errno = pytest.main(shlex.split(self.pytest_args))
        sys.exit(errno)


extensions = [
    Extension(
        name="pyhdc.cyhdc",
        sources=["pyhdc/cyhdc.pyx"],
        extra_compile_args=['-O2', '-std=c++14', '-lgfortran', '-lstdc++', '-lrt', '-fPIC', '-static',],
        language='c++',
        # TODO fix paths after headers are installed
        include_dirs=[
            os.path.join("${CMAKE_SOURCE_DIR}", 'include'),
            os.path.join("${CMAKE_SOURCE_DIR}", 'thirdparty'),
            os.path.join("${CMAKE_SOURCE_DIR}", 'thirdparty', 'jsoncpp'),
            os.path.join("${CMAKE_SOURCE_DIR}", 'thirdparty', 'Pluma-1.1', 'include'),
            numpy.get_include(),
        ],
        libraries=['hdc'],
        library_dirs=[os.path.abspath(os.path.join("..","${CMAKE_INSTALL_PREFIX}", 'lib'))],
        runtime_library_dirs=[os.path.abspath(os.path.join("..","${CMAKE_INSTALL_PREFIX}", 'lib'))],
        cython_directives={"embedsignature": True},)
]

options = dict(
    name="pyhdc",
    packages=["pyhdc"],
    ext_modules=extensions,
    license='MIT',
    description='Hierarchical Dynamic Containers',
    author='David Fridrich, Jakub Urban',
    author_email='fridrich@ipp.cas.cz',
    url='https://bitbucket.org/compass-tokamak/hdc',
    setup_requires=['cython', 'setuptools', 'numpy'],
    install_requires=[
        'Click',
        'future',
        'jinja2',
        'numpy',
        'six',
    ],
    tests_require=['pytest','h5py','hypothesis'],
    entry_points='''
        [console_scripts]
        hdc-binder=pyhdc.binder:cli
    ''',
    package_data={'pyhdc': ['templates/*.j2']},
    cmdclass={'test': PyTest},
    version='0.21.0',
)

setup(**options)
