#include<iostream>
#include<Python.h>
#include<arrayobject.h>

class Vector {

	float* data;
	int size;
	Vector(float* data, int size) {

		this->data = (float*)malloc(sizeof(float)* size);
		this->size = size;
		for (int i = 0; i < size; i++)
		{
			this->data[i] = data[i];
		}
	};

	Vector(PyArrayObject* arrayVector) {
	
		float* data = (float*)PyArray_DATA(arrayVector);
		npy_intp* ndShape1 = PyArray_SHAPE(arrayVector);
		this->data = (float*)malloc(sizeof(float) * ndShape1[0]);
		this->size = ndShape1[0];
		for (int i = 0;  i < size;  i++)
		{
			this->data[i] = data[i];
		}
	}

	Vector operator+(Vector& vector) {
		if (this->size != vector.size)
		{
			std::cout << "Vector legth mismatch" << std::endl;
			return NULL;
		}
		float* sum = (float*)malloc(sizeof(float) * vector.size);
		for (int i = 0; i < vector.size; i++)
		{
			sum[i] = this->data[i] + vector.data[i];
		}
		Vector vendetta = Vector(sum, vector.size);
		free(sum);
		return vendetta;
	};

	PyArrayObject* toNumpy() {
		npy_intp vectorSize[] = { this->size };
		return (PyArrayObject*)PyArray_SimpleNewFromData(1, vectorSize, NPY_FLOAT32, (void*)this->data);
	}

	~Vector() {
		free(data);
	};
};




static PyObject*
vector(PyObject* self, PyObject* arg) {

	PyObject* vector1;
	PyObject* vector2;
	PyArg_ParseTuple(arg, "O!O!", &PyArray_Type, &vector1, &PyArray_Type, &vector2);
	if (vector1 == NULL || vector2 == NULL)
	{
		std::cout << "Argument parse failed" << std::endl;
		return NULL;
	}
	PyArrayObject* arrayVector1;
	PyArrayObject* arrayVector2;
	PyArrayObject* arrayVectorSum;
	arrayVector1 = (PyArrayObject*)PyArray_ContiguousFromAny(vector1, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
	arrayVector2 = (PyArrayObject*)PyArray_ContiguousFromAny(vector2, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL);
	if (arrayVector1 == NULL || arrayVector2 == NULL) {
		std::cout << "Arrayobject cast failed" << std::endl;
		return NULL;
	}
	int dims1 = PyArray_NDIM(arrayVector1);
	int dims2 = PyArray_NDIM(arrayVector2);
	if (dims1 != 1 || dims2 != 1) {
		std::cout << "Argument is not a vector" << std::endl;
		return NULL;
	}
	float sum;
	float* data = (float*)PyArray_DATA(arrayVector1);
	npy_intp* ndShape1 = PyArray_SHAPE(arrayVector1);
	float* data2 = (float*)PyArray_DATA(arrayVector2);
	npy_intp* ndShape2 = PyArray_SHAPE(arrayVector2);
	if (ndShape1[0] != ndShape2[0]) {
		std::cout << "Vector lenght mismatch" << std::endl;
		return NULL;
	}

	int size = ndShape1[0];
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