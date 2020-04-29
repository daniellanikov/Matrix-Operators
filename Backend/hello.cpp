#include<iostream>
#include<Python.h>
#include<arrayobject.h>

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

	PyObject* tuple;
	//PyArg_ParseTuple(arg, "O", &tuple);

	//std::cout << tuple->ob_refcnt;
	PyArrayObject* arrayObject;
	arrayObject = (PyArrayObject*)PyArray_FromAny(arg, NULL, 1, 3, NPY_ARRAY_DEFAULT, NULL);
	std::cout << arrayObject;
	//float data[3] = { 1,2,3 };
	//int dims[1] = { 3 };
	//tuple = PyArray_FromDimsAndData(1, dims, PyArray_FLOAT32, data*);
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
	return PyModule_Create(&HelloModule);
}