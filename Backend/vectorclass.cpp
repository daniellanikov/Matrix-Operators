#include<iostream>
#include<Python.h>
#include<arrayobject.h>


typedef struct {
	PyObject_HEAD;
	float* data;
	int size;
}Vector;

//initconstructor
static PyObject* Vector_init(Vector* self, PyObject* args) {

	PyObject* object;
	PyArg_ParseTuple(args, "O!", &PyArray_Type, &object);
	if (object == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	PyArrayObject* arrayobject;
	arrayobject = (PyArrayObject*)PyArray_ContiguousFromAny(object, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
	if (arrayobject == NULL) {
		std::cout << "Arrayobject cast failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	int dims = PyArray_NDIM(arrayobject);
	if (dims != 1) {
		std::cout << "Argument is not a vector" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	float* objectData = (float*)PyArray_DATA(arrayobject);
	npy_intp* ndShape1 = PyArray_SHAPE(arrayobject);

	self->data = (float*)malloc(sizeof(float) * ndShape1[0]);;
	self->size = ndShape1[0];
	for (int i = 0; i < self->size; i++)
	{
		self->data[i] = objectData[i];
	}

	Py_DECREF(arrayobject);
	return Py_None;
}


static PyObject* Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	Vector* self;

	PyObject* object;
	PyArg_ParseTuple(args, "O!", &PyArray_Type, &object);
	if (object == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		throw std::invalid_argument("received negative value");

	}
	PyArrayObject* arrayobject;
	arrayobject = (PyArrayObject*)PyArray_ContiguousFromAny(object, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
	if (arrayobject == NULL) {
		std::cout << "Arrayobject cast failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	int dims = PyArray_NDIM(arrayobject);
	if (dims != 1) {
		std::cout << "Argument is not a vector" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	npy_intp* ndShape = PyArray_SHAPE(arrayobject);

	self = (Vector*)type->tp_alloc(type, ndShape[0] + sizeof(int));
	return (PyObject*)self;
}

static void
Vector_dealloc(Vector* self) {
	free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}


static PyObject* VectorSum(Vector* vector1, Vector* vector2) {

	if (vector1->size != vector2->size)
	{
		std::cout << "Vector legth mismatch" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	float* sum = (float*)malloc(sizeof(float) * vector1->size);
	for (int i = 0; i < vector1->size; i++)
	{
		sum[i] = vector1->data[i] + vector2->data[i];
	}

	PyObject* result;
	int* dims = (int*)malloc(sizeof(int));
	dims[0] = vector1->size;
	PyArrayObject* array = (PyArrayObject*)PyArray_SimpleNewFromData(1, dims, PyArray_FLOAT32, sum);
	PyObject* arg = Py_BuildValue("O", array);
	result = PyObject_CallObject((PyObject*)& vector_VectorType, arg);
	return result;
}

static PyNumberMethods vectorNumberMethods = { NULL };

PyMODINIT_FUNC PyInit_VectorModule(void)
{
	import_array();
	vectorNumberMethods.nb_add = (binaryfunc)VectorSum;
	vector_VectorType.tp_as_number = &vectorNumberMethods;
	vector_VectorType.tp_name = "vector.Vector";
	vector_VectorType.tp_basicsize = sizeof(Vector);
	vector_VectorType.tp_dealloc = (destructor)Vector_dealloc;
	vector_VectorType.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
	vector_VectorType.tp_doc = "Vector objects";
	vector_VectorType.tp_methods = Vector_methods;
	vector_VectorType.tp_members = Vector_members;
	vector_VectorType.tp_init = (initproc)Vector_init;
	vector_VectorType.tp_new = Vector_new;
	return PyModule_Create(&VectorModule);
}



static PyTypeObject vector_VectorType = { PyObject_HEAD_INIT(NULL) };