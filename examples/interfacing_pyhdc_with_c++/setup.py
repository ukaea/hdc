import os
import os.path
import sys

from setuptools import setup
from setuptools.extension import Extension

# HDC root is "../.."
hdc_root = os.path.abspath(os.path.join("..",".."))
# Let us assume that installation prefix is ${hdc_root}/install. You can change this if necessary:
hdc_install_prefix = os.path.abspath(os.path.join(hdc_root,"install"))

extensions = [
    Extension(
        name="cyhdcdumper",
        sources=["cyhdcdumper.pyx"],
        extra_compile_args=['-std=c++11'],
        language='c++',
        include_dirs=[
            os.path.join(hdc_root, 'include'),
            os.path.join(hdc_root, 'thirdparty'),
            os.path.join(hdc_root, 'thirdparty', 'jsoncpp'),
            os.path.join(hdc_root, 'thirdparty', 'Pluma-1.1', 'include'),
            os.path.join(hdc_root, 'examples', 'cython_cpp'),
        ],
        libraries=["hdc",
                   "hdcdumper"
        ],
        library_dirs=[
            os.path.abspath(os.path.join(hdc_install_prefix, 'lib')),
            os.path.abspath(os.path.curdir),
        ],
        runtime_library_dirs=[
            os.path.abspath(os.path.join(hdc_install_prefix, 'lib')),
            os.path.abspath(os.path.curdir),
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


