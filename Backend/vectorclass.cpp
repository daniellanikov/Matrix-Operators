#include <Python.h>
#include<arrayobject.h>
#include "structmember.h"
#include<iostream>
#include <string>
#include <sstream>
#include <floatobject.h>
#include "vectorclass.h"

PyTypeObject Vector::vector_VectorType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"vector.Vector",             /* tp_name */
	sizeof(vector_VectorObject)
};



//initconstructor
PyObject* Vector::vector_init(vector_VectorObject* self, PyObject* args, PyObject* kwds) {
	PyObject* pyObject = NULL;
	PyArg_ParseTuple(args, "O", &pyObject);

	if (pyObject == NULL)
	{
		std::cout << "Argument parse failed" << std::endl; //TODO: change to python exception handling
		throw std::invalid_argument("received negative value");
	}
	int size = PyArray_SHAPE((PyArrayObject*)(pyObject))[0];
	self->data = pyObject;
	self->size = size;
	return Py_None;
}

//Instantiate
PyObject* Vector::Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	vector_VectorObject* self;
	self = (vector_VectorObject*)type->tp_alloc(type, sizeof(int));
	return (PyObject*)self;
}


void Vector::Vector_dealloc(vector_VectorObject* self) {
	self->data->ob_type->tp_free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyObject* Vector::toNumpy(PyObject* self) {
	import_array();
	PyObject* data = ((vector_VectorObject*)self)->data;
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}


PyObject* Vector::VectorSum(vector_VectorObject* vector1, vector_VectorObject* vector2) {
	import_array();
	if (vector1->size != vector2->size)
	{
		std::cout << "Vector legth mismatch" << std::endl;
		throw std::invalid_argument("received negative value");
	}
	
	PyArrayObject* arrayObject1;
	PyArrayObject* arrayObject2;
	arrayObject1 = (PyArrayObject*)vector1->data;
	arrayObject2 = (PyArrayObject*)vector2->data;
	float* sum = (float*)malloc(sizeof(float) * vector1->size);
	float* vectorData1 = (float*)PyArray_DATA(arrayObject1);
	float* vectorData2 = (float*)PyArray_DATA(arrayObject2);
	
	for (int i = 0; i < vector1->size; i++)
	{
		sum[i] = vectorData1[i] + vectorData2[i];
	}
	PyObject* result;
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp*));
	dims[0] = vector1->size;
	int ndim = PyArray_NDIM(vector1->data);
	PyArrayObject* array = NULL;
	array = (PyArrayObject*)PyArray_SimpleNewFromData(1, dims, NPY_FLOAT32, (void*)sum);
	
	PyObject* arg = Py_BuildValue("(O)", array);
	result = PyObject_CallObject((PyObject*)& vector_VectorType, arg);
	Py_INCREF(result);
	return result;
}

PyObject* Vector::VectorSubstract(vector_VectorObject* vector1, vector_VectorObject* vector2) {

	if (vector1->size != vector2->size)
	{
		std::cout << "Vector legth mismatch" << std::endl;
		throw std::invalid_argument("received negative value");
	}

	PyArrayObject* arrayObject1;
	PyArrayObject* arrayObject2;
	arrayObject1 = (PyArrayObject*)vector1->data;
	arrayObject2 = (PyArrayObject*)vector2->data;
	float* sum = (float*)malloc(sizeof(float) * vector1->size);
	float* vectorData1 = (float*)PyArray_DATA(arrayObject1);
	float* vectorData2 = (float*)PyArray_DATA(arrayObject2);

	for (int i = 0; i < vector1->size; i++)
	{
		sum[i] = vectorData1[i] - vectorData2[i];
	}
	PyObject* result;
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp));
	dims[0] = vector1->size;
	int ndim = PyArray_NDIM(vector1->data);
	PyArrayObject* array = NULL;
	array = (PyArrayObject*)PyArray_SimpleNewFromData(1, dims, NPY_FLOAT32, (void*)sum);
	PyObject* arg = Py_BuildValue("(O)", array);
	result = PyObject_CallObject((PyObject*)& vector_VectorType, arg);
	Py_INCREF(result);
	return result;
}

PyObject* doScalarMulVector(PyObject* left, PyObject* right, int flipScalar) {
	//init
	PyObject* resultVector;
	vector_VectorObject* vector = NULL;
	float scalar = 0;

	vector = (vector_VectorObject*)right;
	PyArg_Parse(left, "f", &scalar);
	if (flipScalar == 1) {

		scalar = 1 / scalar;
	}
	PyArrayObject* rightArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(vector->data, PyArray_FLOAT32, 0, 1);
	float* rightData = (float*)PyArray_DATA(rightArrayObject);
	float* result = (float*)malloc(sizeof(float) * vector->size);
	for (int i = 0; i < vector->size; i++)
	{
		result[i] = scalar * rightData[i];
	}
	PyArrayObject* resultArrayObject = NULL;
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp));
	dims[0] = vector->size;
	resultArrayObject = (PyArrayObject*)PyArray_SimpleNewFromData(1, dims, NPY_FLOAT32, (void*)result);
	PyObject* args = NULL;
	args = Py_BuildValue("(O)", resultArrayObject);
	resultVector = PyObject_CallObject((PyObject*)& Vector::vector_VectorType, args);
	Py_INCREF(resultVector);
	Py_DECREF(rightArrayObject);
	return resultVector;
}


PyObject* doVectorMulVector(PyObject* left, PyObject* right) {
	//init
	PyObject* resultVector;
	vector_VectorObject* vector1 = NULL;
	vector_VectorObject* vector2 = NULL;
	float scalar = 0;

	vector1 = (vector_VectorObject*)right;
	vector2 = (vector_VectorObject*)left;
	PyArrayObject* rightArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(vector1->data, PyArray_FLOAT32, 0, 1);
	PyArrayObject* leftArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(vector2->data, PyArray_FLOAT32, 0, 1);
	float* rightData = (float*)PyArray_DATA(rightArrayObject);
	float* letfData = (float*)PyArray_DATA(leftArrayObject);
	float* result = (float*)malloc(sizeof(float) * vector1->size);
	for (int i = 0; i < vector1->size; i++)
	{
		result[i] = letfData[i] * rightData[i];
	}
	PyArrayObject* resultArrayObject = NULL;
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp));
	dims[0] = vector1->size;
	int ndim = PyArray_NDIM(vector1->data);
	resultArrayObject = (PyArrayObject*)PyArray_SimpleNewFromData(1, dims, NPY_FLOAT32, (void*)result);
	PyObject* args = NULL;
	args = Py_BuildValue("(O)", resultArrayObject);
	resultVector = PyObject_CallObject((PyObject*)& Vector::vector_VectorType, args);
	Py_INCREF(resultVector);
	Py_DECREF(rightArrayObject);
	Py_DECREF(leftArrayObject);
	return resultVector;
}


PyObject* Vector::VectorMul(PyObject* left, PyObject* right) {
	PyObject* returnValue = NULL;
	vector_VectorObject* vector = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 1 && rightISAFloat == 0)
	{
		returnValue = doScalarMulVector(left, right, 0);
	}
	else if (leftISAFloat == 0 && rightISAFloat == 1)
	{
		returnValue = doScalarMulVector(right, left, 0);
	}
	else {
		returnValue = doVectorMulVector(left, right);
	}
	return returnValue;
}

PyObject* Vector::VectorDivision(PyObject* left, PyObject* right) {
	PyObject* returnValue = NULL;
	PyObject* resultVector;
	vector_VectorObject* vector = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 1 && rightISAFloat == 0)
	{
		std::cout << "Wrong order" << std::endl;
	}
	else
	{
		returnValue = doScalarMulVector(right, left, 1);
	}
	return returnValue;
}


PyObject* repr(PyObject* self) {
	std::string lofasz;
	std::stringstream valami;

	float* data = (float*)PyArray_DATA(((vector_VectorObject*)self)->data);
	int size = ((vector_VectorObject*)self)->size;
	valami << "<";
	for (int i = 0; i < size; i++)
	{
		valami << " " << (int)data[i];
	}
	valami << " >";
	lofasz = valami.str();
	PyObject* result = Py_BuildValue("s", &lofasz);
	return result;
}
