#include<iostream>
#include<Python.h>

static PyObject* hello(PyObject* self, PyObject* arg) {

	std::cout << "hello python";

	Py_RETURN_NONE;
}
static char lofasz[] = "hello";

static PyMethodDef module_methods[] = {
    { "hello", (PyCFunction)hello, METH_NOARGS, NULL },
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
	return PyModule_Create(&HelloModule);
}