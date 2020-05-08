#include <Python.h>
#include <arrayobject.h>
#include "structmember.h"
#include <iostream>
#include <string>
#include <sstream>
#include <floatobject.h>
#include "PyVector.h"
#include "Matrix.h"
#include "PyMatrix.h"

PyTypeObject PyVector::vectorType = {
	PyVarObject_HEAD_INIT(NULL, 0) "vector.Vector", /* tp_name */
	sizeof(Matrix) };

// initconstructor
PyObject* PyVector::vectorInit(Matrix* self, PyObject* args, PyObject* kwds) {
	PyObject* pyObject = NULL;
	PyArg_ParseTuple(args, "O", &pyObject);
	if (pyObject == NULL) {
		PyErr_SetString(PyExc_BaseException, "Parse failed");
		return (PyObject*)NULL;
	}
	int* dims = PyArray_SHAPE((PyArrayObject*)(pyObject));
	if (dims[0] != 1 && dims[1] != 1) {
		PyErr_SetString(PyExc_BaseException, "Argument is not a vector");
		return (PyObject*)NULL;
	}
	self->setData(pyObject);
	self->setRow(dims[0]);
	self->setColumn(dims[1]);
	return Py_None;
}

// Instantiate
PyObject* PyVector::vectorNew(PyTypeObject* type, PyObject* args,
	PyObject* kwds) {
	Matrix* self;
	self = (Matrix*)type->tp_alloc(type, sizeof(int) * 2);
	return (PyObject*)self;
}

void PyVector::vectorDealloc(Matrix* self) {
	self->getData()->ob_type->tp_free(self->getData());
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyObject* PyVector::toNumpy(PyObject* self) {
	import_array();
	PyObject* data = ((Matrix*)self)->getData();
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(
		data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}

PyObject* PyVector::vectorSum(Matrix* vector1, Matrix* vector2) {
	return PyMatrix::matrixSum(vector1, vector2);
}

PyObject* PyVector::vectorSubstract(Matrix* vector1, Matrix* vector2) {
	return PyMatrix::matrixSubstraction(vector1, vector2);
}

PyObject* PyVector::vectorMul(PyObject* left, PyObject* right) {
	return PyMatrix::matrixMul(left, right);
}

PyObject* PyVector::vectorDivision(PyObject* left, PyObject* right) {
	return PyMatrix::matrixDiv(left, right);
}
