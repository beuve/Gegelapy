import os
import re
import sys
import platform
import subprocess
import shutil
from pathlib import Path
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        # Don't invoke the normal build_ext for this extension
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        # Check CMake is available
        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build this package")
        
        # Build each extension
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = [
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}',
            f'-DPYTHON_EXECUTABLE={sys.executable}'
        ]
        
        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        # On multi-core systems, allow parallel build
        if platform.system() == "Windows":
            build_args += ['--', f'/m']
        else:
            build_args += ['--', f'-j{os.cpu_count()}']

        build_temp = os.path.join(self.build_temp, ext.name)
        os.makedirs(build_temp, exist_ok=True)

        # Configure
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=build_temp)
        # Build
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=build_temp)

        ext_path = os.path.abspath(self.get_ext_fullpath(ext.name))
        ext_dir = os.path.dirname(ext_path)
        package_dir = os.path.join(os.path.dirname(ext_path), 'gegelapy')

        # If it exists from CMake build, copy it entirely
        gegelapy_folder = os.path.join(ext.sourcedir, 'gegelapy')
        if os.path.exists(gegelapy_folder):
            shutil.rmtree(package_dir, ignore_errors=True)
            shutil.copytree(gegelapy_folder, package_dir)

setup(
    name="gegelapy",
    version='0.1',
    description='Genetic programming library',
    author='Nicolas Beuve, Karol Desnos',
    url='https://github.com/beuve/Gegelapy',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    ext_modules=[CMakeExtension('gegelapy', sourcedir=str(Path(__file__).parent))],
    cmdclass={'build_ext': CMakeBuild},
)
