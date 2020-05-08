#ifndef PyVector_h
#define PyVector_h
#include "Matrix.h"

class PyVector {
public:
	static PyObject* vectorSum(Matrix*, Matrix*);
	static PyObject* vectorSubstract(Matrix* vector1, Matrix* vector2);
	static PyObject* vectorMul(PyObject* left, PyObject* right);
	static PyObject* vectorDivision(PyObject* left, PyObject* right);
	static PyObject* vectorNew(PyTypeObject* type, PyObject* args,
		PyObject* kwds);
	static PyObject* vectorInit(Matrix* self, PyObject* args, PyObject* kwds);
	static void vectorDealloc(Matrix* self);
	static PyObject* toNumpy(PyObject* self);
	static PyTypeObject vectorType;
};

#endif  // !PyVector_h
