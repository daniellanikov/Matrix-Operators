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


//initconstructor
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


static void Vector_dealloc(vector_VectorObject* self) {
	self->data->ob_type->tp_free(self->data); //explanation needed
	self->ob_base.ob_type->tp_free((PyObject*)self);
}


static PyObject* VectorSum(vector_VectorObject* vector1, vector_VectorObject* vector2) {

	if (vector1->size != vector2->size)
	{
		std::cout << "Vector legth mismatch" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	std::cout << "loszar1" << std::endl;
	PyArrayObject* arrayObject1;
	PyArrayObject* arrayObject2;
	arrayObject1 = (PyArrayObject*)vector1;

	std::cout << "loszar2" << std::endl;


	arrayObject2 = (PyArrayObject*)vector2;

	std::cout << "loszar3" << std::endl;


	float* sum = (float*)malloc(sizeof(float) * vector1->size);
	float* vectorData1 = (float*)PyArray_DATA(arrayObject1);
	float* vectorData2 = (float*)PyArray_DATA(arrayObject2);

	std::cout << "loszar4" << std::endl;

	for (int i = 0; i < vector1->size; i++)
	{
		sum[i] = vectorData1[i] + vectorData2[i];
	}

	PyObject* result;
	int* dims = (int*)malloc(sizeof(int));
	dims[0] = vector1->size;
	PyArrayObject* array = (PyArrayObject*)PyArray_SimpleNewFromData(1, dims, PyArray_FLOAT32, sum);

	std::cout << "loszar5" << std::endl;
	PyObject* arg = Py_BuildValue("O", array);

	std::cout << "loszar6" << std::endl;
	result = PyObject_CallObject((PyObject*)& vector_VectorType, arg);
	return result;
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

static PyNumberMethods vectorNumberMethods = { NULL };

PyMODINIT_FUNC
PyInit_VectorModule(void)
{
	PyObject* m;
	vectorNumberMethods.nb_add = (binaryfunc)VectorSum; //operator+ overload
	vector_VectorType.tp_new = Vector_new;
	vector_VectorType.tp_as_number = &vectorNumberMethods;
	vector_VectorType.tp_basicsize = sizeof(vector_VectorObject);
	vector_VectorType.tp_members = vector_members;
	vector_VectorType.tp_init = (initproc)vector_init;
	vector_VectorType.tp_dealloc = (destructor)Vector_dealloc;
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

