#include<iostream>
#include<Python.h>
#include<arrayobject.h>

static PyObject* 

hello(PyObject* self, PyObject* arg) {

	std::cout << "hello python";
	int tuple = PyArg_ParseTuple(arg, "i");
	return Py_BuildValue("i", tuple);
}
static char helloName[] = "hello";

static PyObject*
vector(PyObject* self, PyObject* arg) {

	PyObject* tuple;
	PyArg_ParseTuple(arg, "O", &tuple);
	PyArrayObject* arrayObject;
	arrayObject = (PyArrayObject*)PyArray_FromAny(tuple, NULL, 1, 3, NPY_ARRAY_DEFAULT, NULL);
	return PyArray_Return(arrayObject);
}

static PyMethodDef module_methods[] = {
    { "hello", (PyCFunction)hello, METH_NOARGS, NULL },
	{ "vector", (PyCFunction)vector, METH_VARARGS},
	{NULL}
};

static struct PyModuleDef HelloModule = {
	PyModuleDef_HEAD_INIT,
	"HelloModule",
	"documentation is here",
	-1,
	module_methods

};

PyMODINIT_FUNC PyInit_HelloModule(void)
{
	import_array();
	return PyModule_Create(&HelloModule);
}