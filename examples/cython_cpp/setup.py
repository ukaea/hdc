import os
import os.path
import sys

from setuptools import setup
from setuptools.extension import Extension


extensions = [
    Extension(
        name="cyhdcdumper",
        sources=["cyhdcdumper.pyx"],
        extra_compile_args=['-std=c++11'],
        language='c++',
        include_dirs=[
            os.path.join("/home/david/projects/hdc", 'include'),
            os.path.join("/home/david/projects/hdc", 'thirdparty'),
            os.path.join("/home/david/projects/hdc", 'thirdparty', 'jsoncpp'),
            os.path.join("/home/david/projects/hdc", 'thirdparty', 'Pluma-1.1', 'include'),
            os.path.join("/home/david/projects/hdc", 'examples', 'cython_cpp'),
        ],
        libraries=["hdc",
                   "hdcdumper"
        ],
        library_dirs=[
            os.path.abspath(os.path.join("/home/david/projects/hdc/install", 'lib')),
            os.path.abspath(os.path.join("/home/david/projects/hdc/examples/cython_cpp"))
        ],
        runtime_library_dirs=[
            os.path.abspath(os.path.join("/home/david/projects/hdc/install", 'lib')),
            os.path.abspath(os.path.join("/home/david/projects/hdc/examples/cython_cpp")),
        ],
    )
]

options = dict(
    name="cyhdcdumper",
    packages=[],
    ext_modules=extensions,
    setup_requires=['cython', 'setuptools'],
    install_requires=[
    ],
    version='0.1',
)

setup(**options)


