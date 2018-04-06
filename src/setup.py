from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import os

extensions = [
    Extension(
        name="cyhdc",
        sources=["cyhdc.pyx"],
        extra_compile_args=['-O2', '-std=c++11', '-lgfortran', '-lstdc++', '-lrt', '-fPIC', '-static'],
        language='c++',
        # TODO fix paths after headers are installed
        include_dirs=[
            os.path.join('..', 'include'),
            os.path.join('..', '..', 'thirdparty'),
            os.path.join('..', '..', 'thirdparty', 'jsoncpp'),
            os.path.join('..', '..', 'thirdparty', 'Pluma-1.1', 'include'),
        ],
        libraries=['hdc'],
        library_dirs=[os.path.join('..', 'lib')],
        runtime_library_dirs=[os.path.abspath(os.path.join('..', 'lib'))], )
]

options = dict(
    name="pyhdc",
    py_modules=["pyhdc"],
    ext_modules=cythonize(extensions),
    license='MIT',
    description='Hierarchical Dynamic Containers',
    author='David Fridrich, Jakub Urban',
    author_email='fridrich@ipp.cas.cz',
    url='https://bitbucket.org/compass-tokamak/hdc',
    install_requires=[
        'six',
        'numpy',
        'pytest',
    ],
    setup_requires=['pytest-runner'],
    tests_require=['pytest'],
)

setup(**options)
