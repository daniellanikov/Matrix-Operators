#ifndef vectorclass_h
#define vectorclass_h

typedef struct {
	PyObject_HEAD
		PyObject* data;
	int size;
} VectorObject;

class PyVector {
public:
	static PyObject* vectorSum(VectorObject*, VectorObject*);
	static PyObject* vectorSubstract(VectorObject* vector1, VectorObject* vector2);
	static PyObject* vectorMul(PyObject* left, PyObject* right);
	static PyObject* vectorDivision(PyObject* left, PyObject* right);
	static PyObject* vectorNew(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static PyObject* vectorInit(VectorObject* self, PyObject* args, PyObject* kwds);
	static void vectorDealloc(VectorObject* self);
	static PyObject* toNumpy(PyObject* self);
	static PyTypeObject vectorType;


};

#endif // !vectorclass_h
