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
	return doMatrixSumOrSubtract(*this, matrix, true);
};


float* Matrix::operator-(Matrix matrix) {
	return doMatrixSumOrSubtract(*this, matrix, false);
};

float* Matrix::doMatrixSumOrSubtract(Matrix matrix1, Matrix matrix2, bool isSum) {
	if (matrix1.row != matrix2.row || matrix1.column != matrix2.column)
	{
		throw std::invalid_argument("Size mismatch");
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
		if (isSum)
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
	return doMatrixMulOrDiv(*this, scalar, false);
};

float* Matrix::operator/(float scalar) {
	return doMatrixMulOrDiv(*this, scalar, true);
};


float* Matrix::doMatrixMulOrDiv(Matrix matrix, float scalar, bool isDiv) {
	int matrixSize = matrix.row * matrix.column;
	float* matrixData = new float[matrixSize];
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix.data;
	matrixData = (float*)PyArray_DATA(matrixArrayObject);
	float* resultData = new float[matrixSize];
	if (isDiv && scalar != 0.0f)
	{
		scalar = 1 / scalar;
	}

	for (int i = 0; i < matrixSize; i++)
	{
		resultData[i] = matrixData[i] * scalar;
	}

	return resultData;
}

float* Matrix::operator*(Matrix matrix) {
	return doMatrixMulMatrix(*this, matrix);
}

float* Matrix::doMatrixMulMatrix(Matrix matrix1, Matrix matrix2) {
	int newSize = matrix1.row * matrix2.column;
	float* resultData = new float[newSize];
	PyArrayObject* thisArrayObject = (PyArrayObject*)matrix1.data;
	float* thisData = (float*)PyArray_DATA(thisArrayObject);
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix2.data;
	float* matrixData = (float*)PyArray_DATA(matrixArrayObject);

	for (int i = 0; i < matrix1.row; i++) {
		for (int j = 0; j < matrix2.column; j++) {
			resultData[i * matrix2.column + j] = 0;
			for (int u = 0; u < matrix1.column; u++) {
				resultData[i * matrix2.column + j] += thisData[i * matrix1.column + u] * matrixData[u * matrix2.column + j];
			}
		}
	}

	return resultData;
}
