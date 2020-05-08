#ifndef PyMatrix_h
#define PyMatrix_h
#include "Matrix.h"



class PyMatrix {

public: 

	static PyObject* matrixSum(Matrix* matrix1, Matrix* matrix2);
	static PyObject* matrixSubstraction(Matrix* matrix1, Matrix* matrix2);
	static PyObject* matrixMul(PyObject* left, PyObject* right);
	static PyObject* matrixDiv(PyObject* left, PyObject* right);
	static PyObject* matrixNew(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static PyObject* matrixInit(Matrix* self, PyObject* args, PyObject* kwds);
	static PyObject* wrapMatrix(float* sum, int row, int column);
	static Matrix* checkArgs(PyObject* left, PyObject* right, float &scalar);
	static void matrixDealloc(Matrix* self);
	static PyObject* toNumpyMatrix(PyObject* self);
	static PyTypeObject matrixType;
};

#endif // !PyMatrix_h



