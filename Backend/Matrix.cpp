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
	std::cout << "matrix1: " << this->row << std::endl;
	int thisSize = this->row * this->column;
	int matrixSize = matrix.row * matrix.column;
	float* thisData = new float[thisSize];
	float* matrixData = new float[matrixSize];
	float* resultData = new float[thisSize];
	std::cout << "lofasz1 " << std::endl;
	PyArrayObject* thisArrayObject = (PyArrayObject*)this->data;

	thisData = (float*)PyArray_DATA(thisArrayObject);
	std::cout << "lofasz2 " << std::endl;
	PyArrayObject* matrixArrayObject = (PyArrayObject*)matrix.data;
	matrixData = (float*)PyArray_DATA(matrixArrayObject);

	for (int i = 0; i < thisSize; i++)
	{
		resultData[i] = thisData[i] + matrixData[i];
	}
	
	return resultData;
};
