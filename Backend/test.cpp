#include <Python.h>
#include<arrayobject.h>
#include "structmember.h"
#include<iostream>

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


static int getArrayAndSize(PyObject* args, PyArrayObject* object) {

	PyObject* pyObject;
	PyArg_ParseTuple(args, "O!", &PyArray_Type, &pyObject);
	if (pyObject == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	object = (PyArrayObject*)PyArray_ContiguousFromAny(pyObject, PyArray_FLOAT32, 0, 2);
	if (object == NULL) {
		std::cout << "Arrayobject cast failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	int dims = PyArray_NDIM(object);
	if (dims != 1) {
		std::cout << "Argument is not a vector" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	npy_intp* ndShape = PyArray_SHAPE(object);
	Py_DECREF(pyObject);
	return ndShape[0];
}

static PyObject* vector_init(vector_VectorObject* self, PyObject* args, PyObject* kwds) {
	
	PyArrayObject* object = NULL;
	PyObject* pyObject = NULL;
	PyArg_ParseTuple(args, "O", &pyObject);
	if (pyObject == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	int size = PyArray_SHAPE((PyArrayObject*)(pyObject))[0];
	self->data = args;
	self->size = size;
	Py_DECREF(pyObject);
	return Py_None;
}

//Instantiate
static PyObject* Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	vector_VectorObject* self;
	self = (vector_VectorObject*)type->tp_alloc(type, sizeof(int));
	return (PyObject*)self;
}


static PyModuleDef vectormodule = {
	PyModuleDef_HEAD_INIT,
	"VectorModule",
	"Example module that creates an extension type.",
	-1,
	NULL, NULL, NULL, NULL, NULL
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
	 NULL  /* docstring */}
};


PyMODINIT_FUNC
PyInit_VectorModule(void)
{
	PyObject* m;

	vector_VectorType.tp_new = Vector_new;
	vector_VectorType.tp_basicsize = sizeof(vector_VectorObject);
	vector_VectorType.tp_members = vector_members;
	vector_VectorType.tp_init = (initproc)vector_init;
	if (PyType_Ready(&vector_VectorType) < 0)
		return NULL;

	m = PyModule_Create(&vectormodule);
	if (m == NULL)
		return NULL;

	Py_INCREF(&vector_VectorType);
	if (PyModule_AddObject(m, "vector", (PyObject*)& vector_VectorType) != 0) {
		return NULL;
	};
	return m;
}

