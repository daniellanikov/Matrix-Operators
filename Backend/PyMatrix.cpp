#include <Python.h>
#include<arrayobject.h>
#include "structmember.h"
#include<iostream>
#include <string>
#include <sstream>
#include <floatobject.h>
#include "PyMatrix.h"
#include "Matrix.h"

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

PyObject* wrapMatrix(float* sum, int* dims) {
	PyObject* sumArrayObject = PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, sum);
	PyObject* arg = Py_BuildValue("(O)", sumArrayObject);
	PyObject* sumMatrixObject = PyObject_CallObject((PyObject*)& PyMatrix::matrixType, arg);
	return sumMatrixObject;
}

PyObject* PyMatrix::matrixSum(Matrix* matrix1, Matrix* matrix2) {
	import_array();
	float* sum = *matrix1 + *matrix2;
	int* dims = new int[2];
	dims[0] = matrix1->row;
	dims[1] = matrix2->column;
	return wrapMatrix(sum, dims);
}

PyObject* PyMatrix::matrixSubstraction(Matrix* matrix1, Matrix* matrix2) {
	import_array();
	float* sum = *matrix1 - *matrix2;
	int* dims = new int[2];
	dims[0] = matrix1->row;
	dims[1] = matrix2->column;
	return wrapMatrix(sum, dims);
}


PyObject* doScalarMulMatrix(PyObject* left, PyObject* right, int flipScalar) {
	import_array();

	//init
	PyObject* resultMatrix;
	Matrix* matrix = NULL;
	float scalar = 0;

	matrix = (Matrix*)right;
	PyArg_Parse(left, "f", &scalar);
	if (flipScalar == 1) {

		scalar = 1 / scalar;
	}
	PyArrayObject* rightArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(matrix->data, PyArray_FLOAT32, 0, 2);
	float* rightData = (float*)PyArray_DATA(rightArrayObject);
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp) * 2);
	dims[0] = matrix->row;
	dims[1] = matrix->column;
	int size = dims[0] * dims[1];
	float* result = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++)
	{
		result[i] = scalar * rightData[i];
	}
	PyArrayObject* resultArrayObject = NULL;
	resultArrayObject = (PyArrayObject*)PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, (void*)result);
	PyObject* args = NULL;
	args = Py_BuildValue("(O)", resultArrayObject);
	resultMatrix = PyObject_CallObject((PyObject*)& PyMatrix::matrixType, args);
	Py_INCREF(resultMatrix);
	Py_DECREF(rightArrayObject);
	return resultMatrix;

}

float** toArray(float* data, int row, int column) {
	float** result = new float* [row];
	int columnNumber = 0;
	for (int i = 0; i < row; i++)
	{
		float* resultRow = new float[column];
		for (int j = 0; j < column; j++)
		{
			resultRow[j] = data[columnNumber];
			columnNumber++;
		}
		result[i] = resultRow;
	}
	return result;
}


//TODO: check args
PyObject* MatrixMulMatrix(PyObject* left, PyObject* right) {

	import_array();

	Matrix* leftObject1 = NULL;
	Matrix* rightObject2 = NULL;
	leftObject1 = (Matrix*)left;
	rightObject2 = (Matrix*)right;
	PyArrayObject* arrayObject1 = (PyArrayObject*)PyArray_ContiguousFromAny(leftObject1->data, PyArray_FLOAT32, 0, 2);
	PyArrayObject* arrayObject2 = (PyArrayObject*)PyArray_ContiguousFromAny(rightObject2->data, PyArray_FLOAT32, 0, 2);
	float* data1 = (float*)PyArray_DATA(arrayObject1);
	float* data2 = (float*)PyArray_DATA(arrayObject2);

	npy_intp* dims1 = new npy_intp[2];
	dims1[0] = leftObject1->row;
	dims1[1] = leftObject1->column;
	int size1 = dims1[0] * dims1[1];

	npy_intp* dims2 = new npy_intp[2];
	dims2[0] = rightObject2->row;
	dims2[1] = rightObject2->column;
	int size2 = dims2[0] * dims2[1];

	float** array1 = toArray(data1, dims1[0], dims1[1]);
	float** array2 = toArray(data2, dims2[0], dims2[1]);

	int resultSize = dims1[0] * dims2[1];
	float** result = new float* [dims1[0]];


	for (int i = 0; i < dims1[0]; i++)
	{
		for (int j = 0; j < dims2[1]; j++)
		{

			for (int k = 0; k < dims1[1]; k++)
			{

			}
		}
	}

}


Matrix* checkArgs(PyObject* left, PyObject* right, float scalar) {
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
	return matrix;
}

PyObject* PyMatrix::matrixMul(PyObject* left, PyObject* right) {
	float* resultData;
	PyObject* resultMatrix;
	float scalar = 0;
	Matrix* matrix = checkArgs(left, right, scalar);
	resultData = *matrix * scalar;
	int* dims = new int[2];
	dims[0] = matrix->row;
	dims[1] = matrix->column;
	return wrapMatrix(resultData, dims);
}


PyObject* PyMatrix::matrixDiv(PyObject* left, PyObject* right) {
	float* resultData;
	float scalar = 0;
	PyObject* resultMatrix;
	Matrix* matrix = checkArgs(left, right, scalar);
	resultData = *matrix / scalar;
	int* dims = new int[2];
	dims[0] = matrix->row;
	dims[1] = matrix->column;
	return wrapMatrix(resultData, dims);
}


PyObject* PyMatrix::toNumpyMatrix(PyObject* self) {
	import_array();
	PyObject* data = ((Matrix*)self)->data;
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}


