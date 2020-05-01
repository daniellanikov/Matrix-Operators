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

	PyArrayObject* object;
	int size = getArrayAndSize(args, object);

	self->data = (float*)malloc(sizeof(float) * size);;
	self->size = size;
	for (int i = 0; i < self->size; i++)
	{
		self->data[i] = object->data[i];
	}
	Py_DECREF(object);
	return Py_None;
}

//Instantiate
static PyObject* Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	Vector* self;

	PyArrayObject* arrayObject;
	int size = getArrayAndSize(args, arrayObject);

	self = (Vector*)type->tp_alloc(type, size + sizeof(int));
	Py_DECREF(arrayObject);
	return (PyObject*)self;
}

static void Vector_dealloc(Vector* self) {
	free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}


static int getArrayAndSize(PyObject* args, PyArrayObject* object) {

	PyObject* pyObject;
	PyArg_ParseTuple(args, "O!", &PyArray_Type, &pyObject);
	if (pyObject == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	object = (PyArrayObject*)PyArray_ContiguousFromAny(pyObject, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
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
	vectorNumberMethods.nb_add = (binaryfunc)VectorSum; //operator+ overload
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