#include <Python.h>
#include<arrayobject.h>
#include "structmember.h"
#include<iostream>
#include <string>
#include <sstream>
#include <floatobject.h>
#include "PyMatrix.h"
#include "Matrix.h"
#include <pyerrors.h>

PyTypeObject PyMatrix::matrixType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"vector.Matrix",             /* tp_name */
	sizeof(Matrix)
};


PyObject* PyMatrix::matrixInit(Matrix* self, PyObject* args, PyObject* kwds) {

	PyObject* pyObject = NULL;
	PyArg_ParseTuple(args, "O", &pyObject);

	if (pyObject == NULL)
	{
		PyErr_Print();
	}
	int row = PyArray_SHAPE((PyArrayObject*)(pyObject))[0];
	int column = PyArray_SHAPE((PyArrayObject*)(pyObject))[1];
	self->data = pyObject;
	self->row = row;
	self->column = column;
	return Py_None;
}


//Instantiate
PyObject* PyMatrix::matrixNew(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	Matrix* self;
	self = (Matrix*)type->tp_alloc(type, sizeof(int) * 2);
	return (PyObject*)self;
}

void PyMatrix::matrixDealloc(Matrix* self) {
	self->data->ob_type->tp_free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyObject* PyMatrix::wrapMatrix(float* sum, int row, int column) {
	int dims[2] = { row, column };
	PyObject* sumArrayObject = PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, sum);
	PyObject* arg = Py_BuildValue("(O)", sumArrayObject);
	PyObject* sumMatrixObject = PyObject_CallObject((PyObject*)& PyMatrix::matrixType, arg);
	return sumMatrixObject;
}

PyObject* PyMatrix::matrixSum(Matrix* matrix1, Matrix* matrix2) {
	try
	{
		import_array();
		float* sum = *matrix1 + *matrix2;
		int dims[2] = { matrix1->row, matrix2->column };
		return wrapMatrix(sum, dims[0], dims[1]);
	}
	catch (const std::exception&)
	{
		PyErr_SetString((PyObject*)& matrix1, "Size mismatch");
		return (PyObject*)NULL;
	}
}

PyObject* PyMatrix::matrixSubstraction(Matrix* matrix1, Matrix* matrix2) {
	try
	{
		import_array();
		float* sum = *matrix1 - *matrix2;
		int dims[2] = { matrix1->row, matrix2->column };
		return wrapMatrix(sum, dims[0], dims[1]);
	}
	catch (const std::exception&)
	{
		PyErr_SetString(PyExc_BaseException, "Argument is not a vector");
		return (PyObject*)NULL;
	}
}

Matrix* PyMatrix::checkArgs(PyObject* left, PyObject* right, float &scalar) {
	Matrix* matrix;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 1 && rightISAFloat == 0)
	{
		matrix = (Matrix*)right;
		PyArg_Parse(left, "f", &scalar);
	}
	else if (leftISAFloat == 0 && rightISAFloat == 1)
	{
		matrix = (Matrix*)left;
		PyArg_Parse(right, "f", &scalar);
	}
	else
	{
		PyErr_SetString(PyExc_BaseException, "Argument is not a vector");
		return NULL;
	}
	return matrix;
}

PyObject* PyMatrix::matrixMul(PyObject* left, PyObject* right) {
	float* resultData;
	PyObject* resultMatrix;
	float scalar = 0;
	Matrix* matrix = checkArgs(left, right, scalar);
	resultData = *matrix * scalar;
	int dims[2] = { matrix->row, matrix->column };
	return wrapMatrix(resultData, dims[0], dims[1]);
}


PyObject* PyMatrix::matrixDiv(PyObject* left, PyObject* right) {
	float* resultData;
	float scalar = 0;
	PyObject* resultMatrix;
	Matrix* matrix = checkArgs(left, right, scalar);
	resultData = *matrix / scalar;
	int dims[2] = { matrix->row, matrix->column };
	return wrapMatrix(resultData, dims[0], dims[1]);
}


PyObject* PyMatrix::matrixMulMatrix(PyObject* left, PyObject* right) {
	std::cout << "lofasz1" << std::endl;
	float* resultData;
	PyObject* resultMatrix;
	Matrix* matrix;
	matrix = (Matrix*)left;
	Matrix* matrix2;
	matrix2 = (Matrix*)right;
	resultData = (*matrix) * (*matrix2);
	int dims[2] = { matrix->row, matrix2->column };
	return wrapMatrix(resultData, dims[0], dims[1]);
}


PyObject* PyMatrix::toNumpyMatrix(PyObject* self) {
	import_array();
	PyObject* data = ((Matrix*)self)->data;
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}


