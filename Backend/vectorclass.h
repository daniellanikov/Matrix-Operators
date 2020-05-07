typedef struct {
	PyObject_HEAD
		PyObject* data;
	int size;
} vector_VectorObject;

static PyTypeObject vector_VectorType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"vector.Vector",             /* tp_name */
	sizeof(vector_VectorObject)
}; 

PyObject* VectorSum(vector_VectorObject* vector1, vector_VectorObject* vector2);
PyObject* VectorSubstract(vector_VectorObject* vector1, vector_VectorObject* vector2);
PyObject* VectorMul(PyObject* left, PyObject* right);
PyObject* VectorDivision(PyObject* left, PyObject* right);
PyObject* Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
PyObject* vector_init(vector_VectorObject* self, PyObject* args, PyObject* kwds);
void Vector_dealloc(vector_VectorObject* self);
PyObject* toNumpy(PyObject* self);

static PyMethodDef vector_methods[] = {
	{ "toNumpy", (PyCFunction)toNumpy, METH_VARARGS},
	{NULL}
};


static PyMemberDef vector_members[] = {
	{"size",  /* name */
	 offsetof(vector_VectorObject, size),  /* offset */
	 T_INT,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},

	 {"data",  /* name */
	 offsetof(vector_VectorObject, data),  /* offset */
	 T_OBJECT_EX,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},
	 {NULL}
};


static PyNumberMethods vectorNumberMethods = { NULL };


static struct PyModuleDef vectorModule = {
	PyModuleDef_HEAD_INIT,
	"VectorModule",
	"documentation is here",
	-1,
	vector_methods
};
