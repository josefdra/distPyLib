from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "distPyLib._parallel_sum",
        [
            "src/cc/python_bindings.cc",
            "src/cc/parallel_sum.cc",
        ],
        include_dirs=[
            "src/cc",
        ],
        libraries=["mpi"],
        language='c++'
    ),
]

setup(
    name="distPyLib",
    version="1.0.0",
    author="Josef Dräxl",
    description="A Python MPI library with C++ backend",
    long_description="",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    python_requires=">=3.8",
    install_requires=[
        "numpy",
    ]
)
