#!/usr/bin/env python3
"""
Setup script for building DisjointForest Python bindings
"""

from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "disjoint_forest",
        ["disjoint_forest_bindings.cc"],
        include_dirs=[
            pybind11.get_include(),
            "..",  # For the disjoint_forest.h header
        ],
        library_dirs=[".."],  # For the libdisjoint_forest.a
        libraries=["disjoint_forest"],
        language="c++",
        cxx_std=17,
    ),
]

setup(
    name="disjoint_forest",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
) 