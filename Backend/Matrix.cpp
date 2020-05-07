#include <Python.h>
#include "Matrix.h"
#include<arrayobject.h>
#include<iostream>

Matrix::Matrix(int row, int column, PyObject* data) {

	this->row = row;
	this->column = column;
	this->data = data;
}


float* Matrix::operator+(Matrix matrix) {
	return doMatrixSumOrSubtract(*this, matrix, 1);
};


float* Matrix::operator-(Matrix matrix) {
	return doMatrixSumOrSubtract(*this, matrix, 0);
};



float* Matrix::doMatrixSumOrSubtract(Matrix matrix1, Matrix matrix2, int subtraction) {
	if (matrix1.row != matrix2.row || matrix1.column != matrix2.column)
	{
		PyErr_SetString((PyObject*)& matrix1, "Size mismatch");
	}
	int thisSize = matrix1.row * matrix1.column;
	int matrixSize = matrix2.row * matrix2.column;
	float* resultData = new float[thisSize];
	PyArrayObject* thisArrayObject = (PyArrayObject*)matrix1.data;
	float* thisData = (float*)PyArray_DATA(thisArrayObject);
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix2.data;
	float* matrixData = (float*)PyArray_DATA(matrixArrayObject);

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

float* Matrix::operator*(float scalar) {
	return doMatrixMulOrDiv(*this, scalar, 0);
};

float* Matrix::operator/(float scalar) {
	return doMatrixMulOrDiv(*this, scalar, 1);
};


float* Matrix::doMatrixMulOrDiv(Matrix matrix, float scalar, int flipScalar) {
	int matrixSize = matrix.row * matrix.column;
	float* matrixData = new float[matrixSize];
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix.data;
	matrixData = (float*)PyArray_DATA(matrixArrayObject);
	float* resultData = new float[matrixSize];
	if (flipScalar == 1 && scalar != 0.0f)
	{
		scalar = 1 / scalar;
	}

	for (int i = 0; i < matrixSize; i++)
	{
		resultData[i] = matrixData[i] * scalar;
	}

	return resultData;
}
