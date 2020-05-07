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
	sizeof(MatrixObject)
};


PyObject* PyMatrix::matrixInit(MatrixObject* self, PyObject* args, PyObject* kwds) {

	PyObject* pyObject = NULL;
	PyArg_ParseTuple(args, "O", &pyObject);

	if (pyObject == NULL)
	{
		std::cout << "Argument parse failed" << std::endl; //TODO: change to python exception handling
		throw std::invalid_argument("received negative value");
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
	MatrixObject* self;
	self = (MatrixObject*)type->tp_alloc(type, sizeof(int) * 2);
	return (PyObject*)self;
}

void PyMatrix::matrixDealloc(MatrixObject* self) {
	self->data->ob_type->tp_free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}


PyObject* doMatrixSum(MatrixObject* matrix1, MatrixObject* matrix2, int substraction) {

	import_array();

	PyArrayObject* arrayObject1;
	PyArrayObject* arrayObject2;
	arrayObject1 = (PyArrayObject*)matrix1->data;
	arrayObject2 = (PyArrayObject*)matrix2->data;
	int size = matrix1->row * matrix1->column;
	float* sum = (float*)malloc(sizeof(float) * size);
	float* matrixData1 = (float*)PyArray_DATA(arrayObject1);
	float* matrixData2 = (float*)PyArray_DATA(arrayObject2);

	for (int i = 0; i < size; i++)
	{
		if (substraction == 1)
		{
			sum[i] = matrixData1[i] - matrixData2[i];
		}
		else { sum[i] = matrixData1[i] + matrixData2[i]; }
	}
	PyObject* result;
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp*) * 2);
	dims[0] = matrix1->row;
	dims[1] = matrix2->column;
	PyArrayObject* array = NULL;
	array = (PyArrayObject*)PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, (void*)sum);
	PyObject* arg = Py_BuildValue("(O)", array);
	result = PyObject_CallObject((PyObject*)& PyMatrix::matrixType, arg);
	Py_INCREF(result);
	return result;
}

PyObject* PyMatrix::matrixSum(MatrixObject* matrix1, MatrixObject* matrix2) {
	
	MatrixObject sum = *matrix1 + *matrix2;
	return sum.data;
}

PyObject* PyMatrix::matrixSubstraction(MatrixObject* matrix1, MatrixObject* matrix2) {
	PyObject* result = doMatrixSum(matrix1, matrix2, 1);
	return result;
}


PyObject* doScalarMulMatrix(PyObject* left, PyObject* right, int flipScalar) {
	import_array();

	//init
	PyObject* resultMatrix;
	MatrixObject* matrix = NULL;
	float scalar = 0;

	matrix = (MatrixObject*)right;
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

	MatrixObject* leftObject1 = NULL;
	MatrixObject* rightObject2 = NULL;
	leftObject1 = (MatrixObject*)left;
	rightObject2 = (MatrixObject*)right;
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




PyObject* PyMatrix::matrixMul(PyObject* left, PyObject* right) {

	PyObject* returnValue = NULL;
	PyObject* resultMatrix;
	MatrixObject* matrix = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 1 && rightISAFloat == 0)
	{
		returnValue = doScalarMulMatrix(left, right, 0);

	}
	else if (leftISAFloat == 0 && rightISAFloat == 1)
	{
		returnValue = doScalarMulMatrix(right, left, 0);
	}
	return returnValue;
}


PyObject* PyMatrix::matrixDiv(PyObject* left, PyObject* right) {
	PyObject* returnValue = NULL;
	PyObject* resultMatrix;
	MatrixObject* matrix = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 0 && rightISAFloat == 1)
	{
		returnValue = doScalarMulMatrix(right, left, 1);

	}
	else
	{
		std::cout << "Wrong order" << std::endl;
	}
	return returnValue;
}


PyObject* PyMatrix::toNumpyMatrix(PyObject* self) {
	import_array();
	PyObject* data = ((MatrixObject*)self)->data;
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}


