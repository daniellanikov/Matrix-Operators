#ifndef Matrix_h
#define Matrix_h

typedef struct {
	PyObject_HEAD
		PyObject* data;
	int row;
	int column;
} MatrixObject;


class Matrix {

public: 

	static PyObject* MatrixSum(MatrixObject* matrix1, MatrixObject* matrix2);
	static PyObject* MatrixSubstraction(MatrixObject* matrix1, MatrixObject* matrix2);
	static PyObject* MatrixMul(PyObject* left, PyObject* right);
	static PyObject* MatrixDiv(PyObject* left, PyObject* right);
	static PyObject* Matrix_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static PyObject* matrix_init(MatrixObject* self, PyObject* args, PyObject* kwds);
	static void Matrix_dealloc(MatrixObject* self);
	static PyObject* toNumpyMatrix(PyObject* self);
	static PyTypeObject MatrixType;
};

#endif // !Matrix_h



