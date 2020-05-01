#!python
from distutils.core import setup, Extension

VectorModule = Extension('VectorModule',
                    sources = ['vector.cpp'])

setup (name = 'VectorModule',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [VectorModule])