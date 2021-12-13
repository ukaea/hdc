# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# In order to make this work, we have to build pyhdc first. To do so, please run
# cd build/python
# python setup.py build_ext --inplace
import os
import sys
os.environ['LD_LIBRARY_PATH'] = os.path.join(os.getcwd(),"../build/python/pyhdc")
sys.path.insert(0, os.path.abspath('../build/python'))

# -- Project information -----------------------------------------------------

project = 'HDC'
copyright = '2020, David Fridrich, Jakub Urban'
author = 'David Fridrich, Jakub Urban'


# -- General configuration ---------------------------------------------------

# set index as master_doc
master_doc = 'index'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "m2r", # Markdown support - mdinclude
    "sphinxfortran.fortran_domain",
    "sphinxfortran.fortran_autodoc",
    "sphinx.ext.graphviz",
    "breathe",
    "exhale"
]

source_suffix = ['.rst', '.md']

breathe_projects = {"hdc": "../../docs/xml"}
breathe_default_project = "hdc"

## Options for Sphinx-Fortran
# List of possible extensions in the case of a directory listing
fortran_ext = ['f90', 'F90']
# fortran plugin settings
fortran_src = ["../../src"]
# Indentation string or length (default 4). If it is an integer,
# indicates the number of spaces.
fortran_indent = 4

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./hdc_api",
    "rootFileName":          "hdc_libraty_root.rst",
    "rootFileTitle":         "HDC Library API (C/C++)",
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
    #"exhaleExecutesDoxygen": True,
    #"exhaleDoxygenStdin":    "INPUT = ../include"
}
primary_domain = None
highlight_language = 'default'


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
