#include<iostream>
#include<Python.h>

static PyObject* 

hello(PyObject* self, PyObject* arg) {

	std::cout << "hello python";

	float vectorArray[3] = { 1,2,3 };

	int tuple = PyArg_ParseTuple(arg, "i");
	return Py_BuildValue("i", tuple);
}
static char lofasz[] = "hello";

static PyObject*
vector(PyObject* self, PyObject* arg) {
	/**
	class Vector {

	public:
		Vector() {};
		Vector(int length) {}
		~Vector() {}
	};
	//dostuff

	float vectorArray[3] = { 1,2,3 }; 

	int tuple = PyArg_ParseTuple(arg, "i"); */
	return Py_BuildValue("i", 69);
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
	return PyModule_Create(&HelloModule);
}