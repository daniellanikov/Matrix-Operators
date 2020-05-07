#ifndef PyMatrix_h
#define PyMatrix_h
#include "Matrix.h"



class PyMatrix {

public: 

	static PyObject* matrixSum(MatrixObject* matrix1, MatrixObject* matrix2);
	static PyObject* matrixSubstraction(MatrixObject* matrix1, MatrixObject* matrix2);
	static PyObject* matrixMul(PyObject* left, PyObject* right);
	static PyObject* matrixDiv(PyObject* left, PyObject* right);
	static PyObject* matrixNew(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static PyObject* matrixInit(MatrixObject* self, PyObject* args, PyObject* kwds);
	static void matrixDealloc(MatrixObject* self);
	static PyObject* toNumpyMatrix(PyObject* self);
	static PyTypeObject matrixType;
};

#endif // !PyMatrix_h



