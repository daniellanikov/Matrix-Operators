#!python
from distutils.core import setup, Extension

HelloModule = Extension('HelloModule',
                    sources = ['hello.cpp'])

setup (name = 'HelloModule',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [HelloModule])
