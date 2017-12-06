# TODO look at http://scikit-build.readthedocs.io/en/latest/
# or https://github.com/thewtex/cython-cmake-example

# TODO - IMPORTANT
# this only work after copying this and all cython files to the build directory

import sys
from distutils.core import setup, Extension
from Cython.Build import cythonize

compile_args = ['-g', '-std=c++11']

if sys.platform == 'darwin':
    compile_args.append('-mmacosx-version-min=10.7')

basics_module = Extension(
    'cyhdc',
    sources=['cyhdc.pyx'],
    extra_compile_args=compile_args,
    include_dirs=['../include', '../thirdparty/Pluma-1.1/include'],
    libraries=['hdc'],
    library_dirs=['./lib'],
    language='c++')

setup(name='cyhdc', packages=['cyhdc'], ext_modules=cythonize(basics_module))
