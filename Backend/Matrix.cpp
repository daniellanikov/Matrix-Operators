#include <Python.h>
#include "Matrix.h"
#include<arrayobject.h>
#include<iostream>

MatrixObject::MatrixObject(int row, int column, PyObject* data) {

	this->row = row;
	this->column = column;
	this->data = data;
}


float* MatrixObject::operator+(MatrixObject matrix) {
	return doMatrixSum(*this, matrix, 1);
};


float* MatrixObject::operator-(MatrixObject matrix) {
	return doMatrixSum(*this, matrix, 0);
};



float* MatrixObject::doMatrixSum(MatrixObject matrix1, MatrixObject matrix2, int subtraction) {
	int thisSize = matrix1.row * matrix1.column;
	int matrixSize = matrix2.row * matrix2.column;
	float* thisData = new float[thisSize];
	float* matrixData = new float[matrixSize];
	float* resultData = new float[thisSize];
	PyArrayObject* thisArrayObject = (PyArrayObject*)matrix1.data;

	thisData = (float*)PyArray_DATA(thisArrayObject);
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix2.data;
	matrixData = (float*)PyArray_DATA(matrixArrayObject);

	for (int i = 0; i < thisSize; i++)
	{
		if (subtraction == 1)
		{
			resultData[i] = thisData[i] + matrixData[i];
		}
		else
		{
			resultData[i] = thisData[i] - matrixData[i];
		}
	}

	return resultData;
}
