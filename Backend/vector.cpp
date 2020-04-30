#include<iostream>
#include<Python.h>
#include<arrayobject.h>

static PyObject*
vector(PyObject* self, PyObject* arg) {

	PyObject* vector1;
	PyObject* vector2;
	PyArg_ParseTuple(arg, "O!O!", &PyArray_Type, &vector1, &PyArray_Type, &vector2);
	if (vector1 == NULL || vector2 == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		return 0;
	}
	PyArrayObject* arrayVector1;
	PyArrayObject* arrayVector2;
	PyArrayObject* arrayVectorSum;
	arrayVector1 = (PyArrayObject*)PyArray_ContiguousFromAny(vector1, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
	arrayVector2 = (PyArrayObject*)PyArray_ContiguousFromAny(vector2, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
	if (arrayVector1 == NULL || arrayVector2 == NULL) {
		std::cout << "Arrayobject cast failed" << std::endl;
		return 0;
	}
	float sum;
	float* data = (float*)PyArray_DATA(arrayVector1);
	npy_intp* ndShape = PyArray_SHAPE(arrayVector1);
	float* data2 = (float*)PyArray_DATA(arrayVector2);
	int size = ndShape[0];
	float* vectorSum = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++)
	{
		sum = data[i] + data2[i];
		vectorSum[i] = sum;
	}
	npy_intp sizeSum[] = { size };
	arrayVectorSum = (PyArrayObject*)PyArray_SimpleNewFromData(1, sizeSum, NPY_FLOAT32, (void*)vectorSum);
	Py_DECREF(arrayVector1);
	Py_DECREF(arrayVector2);
	return PyArray_Return(arrayVectorSum);
}

static PyMethodDef module_methods[] = {
	{ "vector", (PyCFunction)vector, METH_VARARGS},
	{NULL}
};

static struct PyModuleDef VectorModule = {
	PyModuleDef_HEAD_INIT,
	"VectorModule",
	"documentation is here",
	-1,
	module_methods
};

PyMODINIT_FUNC PyInit_VectorModule(void)
{
	import_array();
	return PyModule_Create(&VectorModule);
}