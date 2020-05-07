#include <Python.h>
#include "Matrix.h"
#include<arrayobject.h>


MatrixObject::MatrixObject(int row, int column, PyObject* data) {

	this->row = row;
	this->column = column;
	this->data = data;
}

MatrixObject MatrixObject::operator+(MatrixObject matrix) {

	//import_array();
	
	int thisSize = this->row * this->column;
	int matrixSize = matrix.row * matrix.column;
	float* thisData = new float[thisSize];
	float* matrixData = new float[matrixSize];
	float* resultData = new float[thisSize];

	PyArrayObject* thisArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(this->data, PyArray_FLOAT32, 0, 2);
	thisData = (float*)PyArray_DATA(thisArrayObject);

	PyArrayObject* matrixArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(matrix.data, PyArray_FLOAT32, 0, 2);
	matrixData = (float*)PyArray_DATA(matrixArrayObject);

	for (int i = 0; i < thisSize; i++)
	{
		resultData[i] = thisData[i] + matrixData[i];
	}
	int* dims = new int[2];
	dims[0] = this->row;
	dims[1] = this->column;
	PyObject* resultObject = PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, resultData);
	MatrixObject result = MatrixObject(dims[0], dims[1], resultObject);
	return result;
};
