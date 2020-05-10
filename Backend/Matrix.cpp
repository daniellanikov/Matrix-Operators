#include <Python.h>
#include "Matrix.h"
#include <arrayobject.h>
#include <iostream>
#include "Strassen.h"

Matrix::Matrix(int row, int column, PyObject* data) {
	this->row = row;
	this->column = column;
	this->data = data;
}

void Matrix::setRow(int row) { this->row = row; }

int Matrix::getRow() { return this->row; }

void Matrix::setColumn(int column) { this->column = column; }

int Matrix::getColumn() { return this->column; }

void Matrix::setData(PyObject* data) { this->data = data; }

PyObject* Matrix::getData() { return this->data; }

float* Matrix::operator+(Matrix matrix) {
	return doMatrixSumOrSubtract(*this, matrix, true);
};

float* Matrix::operator-(Matrix matrix) {
	return doMatrixSumOrSubtract(*this, matrix, false);
};

float* Matrix::doMatrixSumOrSubtract(Matrix matrix1, Matrix matrix2,
	bool isSum) {
	if (matrix1.row != matrix2.row || matrix1.column != matrix2.column) {
		throw std::invalid_argument("Size mismatch");
	}
	int thisSize = matrix1.row * matrix1.column;
	int matrixSize = matrix2.row * matrix2.column;
	float* resultData = new float[thisSize];
	PyArrayObject* thisArrayObject = (PyArrayObject*)matrix1.data;
	float* thisData = (float*)PyArray_DATA(thisArrayObject);
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix2.data;
	float* matrixData = (float*)PyArray_DATA(matrixArrayObject);

	for (int i = 0; i < thisSize; i++) {
		if (isSum) {
			resultData[i] = thisData[i] + matrixData[i];
		}
		else {
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
	if (isDiv && scalar != 0.0f) {
		scalar = 1 / scalar;
	}

	for (int i = 0; i < matrixSize; i++) {
		resultData[i] = matrixData[i] * scalar;
	}

	return resultData;
}

float* Matrix::operator*(Matrix matrix) {
	return doStrassen(*this, matrix);
}

static float** toArray(float* data, int row, int column) {
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

static float* yarrAot(float** data, int row, int column) {
	 
	int columnNumber = 0;
	float* result = new float[row * column];
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			result[columnNumber] = data[i][j];
			columnNumber++;
		}
	}
	return result;
}

float* Matrix::doStrassen(Matrix matrix1, Matrix matrix2) {
		
	PyArrayObject* thisArrayObject = (PyArrayObject*)matrix1.getData();
	float* thisData = (float*)PyArray_DATA(thisArrayObject);
	float** strassenInput1 = toArray(thisData, matrix1.getRow(), matrix1.getColumn());

	PyArrayObject* thisArrayObject2 = (PyArrayObject*)matrix2.getData();
	float* thisData2 = (float*)PyArray_DATA(thisArrayObject2);
	float** strassenInput2 = toArray(thisData2, matrix2.getRow(), matrix2.getColumn());

	float** result = Strassen::Calculate(strassenInput1, strassenInput2, matrix1.getRow(), matrix1.getColumn(), matrix2.getColumn());
	return yarrAot(result, matrix1.getRow(), matrix2.getColumn());
}