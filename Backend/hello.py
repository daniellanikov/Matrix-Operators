#!python
from distutils.core import setup, Extension
import numpy as np
import ctypes

arrVector = np.array([1, 2, 3, 4])
def vector():
    return Extension

HelloModule = vector()('HelloModule',
                    sources = ['hello.cpp'])

setup (name = 'HelloModule',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [HelloModule])

arr = np.array([[1, 2, 3, 4], [5, 6, 7, 8]])


print(arr.shape)