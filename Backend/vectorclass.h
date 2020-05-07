#ifndef vectorclass_h
#define vectorclass_h

typedef struct {
	PyObject_HEAD
		PyObject* data;
	int size;
} vector_VectorObject;

class Vector {
public:
	static PyObject* VectorSum(vector_VectorObject*, vector_VectorObject*);
	static PyObject* VectorSubstract(vector_VectorObject* vector1, vector_VectorObject* vector2);
	static PyObject* VectorMul(PyObject* left, PyObject* right);
	static PyObject* VectorDivision(PyObject* left, PyObject* right);
	static PyObject* Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static PyObject* vector_init(vector_VectorObject* self, PyObject* args, PyObject* kwds);
	static void Vector_dealloc(vector_VectorObject* self);
	static PyObject* toNumpy(PyObject* self);
	static PyTypeObject vector_VectorType;


};

#endif // !vectorclass_h
