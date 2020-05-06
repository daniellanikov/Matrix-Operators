#include <Python.h>
#include<arrayobject.h>
#include "structmember.h"
#include<iostream>
#include <string>
#include <sstream>
#include <floatobject.h>

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


//initconstructor
static PyObject* vector_init(vector_VectorObject* self, PyObject* args, PyObject* kwds) {
	
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
static PyObject* Vector_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	vector_VectorObject* self;
	self = (vector_VectorObject*)type->tp_alloc(type, sizeof(int));
	return (PyObject*)self;
}


static void Vector_dealloc(vector_VectorObject* self) {
	self->data->ob_type->tp_free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyObject* toNumpy(PyObject* self) {
	PyObject* data = ((vector_VectorObject*)self)->data;
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}


static PyObject* VectorSum(vector_VectorObject* vector1, vector_VectorObject* vector2) {

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

static PyObject* VectorSubstract(vector_VectorObject* vector1, vector_VectorObject* vector2) {

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

static PyObject* doScalarMulVector(PyObject* left, PyObject* right, int flipScalar) {
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
	resultVector = PyObject_CallObject((PyObject*)& vector_VectorType, args);
	Py_INCREF(resultVector);
	Py_DECREF(rightArrayObject);
	return resultVector;
}


static PyObject* doVectorMulVector(PyObject* left, PyObject* right) {
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
	resultVector = PyObject_CallObject((PyObject*)& vector_VectorType, args);
	Py_INCREF(resultVector);
	Py_DECREF(rightArrayObject);
	Py_DECREF(leftArrayObject);
	return resultVector;
}

//TODO: change left arg to vector and right to scalar



static PyObject* VectorMul(PyObject* left, PyObject* right) {
	PyObject* returnValue = NULL;
	vector_VectorObject* vector = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 1 && rightISAFloat == 0)
	{
		returnValue = doScalarMulVector(left, right, 0);
	}
	else if(leftISAFloat == 0 && rightISAFloat == 1)
	{
		returnValue = doScalarMulVector(right, left, 0); 
	}
	else {
		returnValue = doVectorMulVector(left, right);
	}
	return returnValue;
}

static PyObject* VectorDivision(PyObject* left, PyObject* right) {
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


static PyObject* repr(PyObject* self) {
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



static PyMethodDef vector_methods[] = {
	{ "toNumpy", (PyCFunction)toNumpy, METH_VARARGS},
	{NULL}
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
	 NULL  /* docstring */},
	 {NULL}
};

static PyNumberMethods vectorNumberMethods = { NULL };





// ---------------------------------------------------------------------------------------------------------



typedef struct {
	PyObject_HEAD
		PyObject* data;
	int row;
	int column;
} MatrixObject;

static PyTypeObject MatrixType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"vector.Matrix",             /* tp_name */
	sizeof(MatrixObject)
};



static PyObject* matrix_init(MatrixObject* self, PyObject* args, PyObject* kwds) {

	PyObject* pyObject = NULL;
	PyArg_ParseTuple(args, "O", &pyObject);

	if (pyObject == NULL)
	{
		std::cout << "Argument parse failed" << std::endl; //TODO: change to python exception handling
		throw std::invalid_argument("received negative value");
	}
	int row = PyArray_SHAPE((PyArrayObject*)(pyObject))[0];
	int column = PyArray_SHAPE((PyArrayObject*)(pyObject))[1];
	self->data = pyObject;
	self->row = row;
	self->column = column;
	return Py_None;
}


//Instantiate
static PyObject* Matrix_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	MatrixObject* self;
	self = (MatrixObject*)type->tp_alloc(type, sizeof(int) * 2);
	return (PyObject*)self;
}

static void Matrix_dealloc(MatrixObject* self) {
	self->data->ob_type->tp_free(self->data);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}




static PyObject* doMatrixSum(MatrixObject* matrix1, MatrixObject* matrix2, int substraction) {

	PyArrayObject* arrayObject1;
	PyArrayObject* arrayObject2;
	arrayObject1 = (PyArrayObject*)matrix1->data;
	arrayObject2 = (PyArrayObject*)matrix2->data;
	int size = matrix1->row * matrix1->column;
	float* sum = (float*)malloc(sizeof(float) * size);
	float* matrixData1 = (float*)PyArray_DATA(arrayObject1);
	float* matrixData2 = (float*)PyArray_DATA(arrayObject2);

	for (int i = 0; i < size; i++)
	{
		if (substraction == 1)
		{
			sum[i] = matrixData1[i] - matrixData2[i];
		}
		else { sum[i] = matrixData1[i] + matrixData2[i]; }
	}
	PyObject* result;
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp*) * 2);
	dims[0] = matrix1->row;
	dims[1] = matrix2->column;
	PyArrayObject* array = NULL;
	array = (PyArrayObject*)PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, (void*)sum);
	PyObject* arg = Py_BuildValue("(O)", array);
	result = PyObject_CallObject((PyObject*)& MatrixType, arg);
	Py_INCREF(result);
	return result;
}

static PyObject* MatrixSum(MatrixObject* matrix1, MatrixObject* matrix2) {
	PyObject* result = doMatrixSum(matrix1, matrix2, 0);
	return result;
}

static PyObject* MatrixSubstraction(MatrixObject* matrix1, MatrixObject* matrix2) {
	PyObject* result = doMatrixSum(matrix1, matrix2, 1);
	return result;
}


static PyObject* doScalarMulMatrix(PyObject* left, PyObject* right, int flipScalar) {
	//init
	PyObject* resultMatrix;
	MatrixObject* matrix = NULL;
	float scalar = 0;

	matrix = (MatrixObject*)right;
	PyArg_Parse(left, "f", &scalar);
	if (flipScalar == 1) {

		scalar = 1 / scalar;
	}
	PyArrayObject* rightArrayObject = (PyArrayObject*)PyArray_ContiguousFromAny(matrix->data, PyArray_FLOAT32, 0, 2);
	float* rightData = (float*)PyArray_DATA(rightArrayObject);
	npy_intp* dims = (npy_intp*)malloc(sizeof(npy_intp) * 2);
	dims[0] = matrix->row;
	dims[1] = matrix->column;
	int size = dims[0] * dims[1];
	float* result = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++)
	{
		result[i] = scalar * rightData[i];
	}
	PyArrayObject* resultArrayObject = NULL;
	resultArrayObject = (PyArrayObject*)PyArray_SimpleNewFromData(2, dims, NPY_FLOAT32, (void*)result);
	PyObject* args = NULL;
	args = Py_BuildValue("(O)", resultArrayObject);
	resultMatrix = PyObject_CallObject((PyObject*)& MatrixType, args);
	Py_INCREF(resultMatrix);
	Py_DECREF(rightArrayObject);
	return resultMatrix;

}



static PyObject* MatrixMul(PyObject* left, PyObject* right) {

	PyObject* returnValue = NULL;
	PyObject* resultMatrix;
	MatrixObject* matrix = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 1 && rightISAFloat == 0)
	{
		returnValue = doScalarMulMatrix(left, right, 0);

	}
	else if (leftISAFloat == 0 && rightISAFloat == 1)
	{
		returnValue = doScalarMulMatrix(right, left, 0);
	}
	return returnValue;
}


static PyObject* MatrixDiv(PyObject* left, PyObject* right) {
	PyObject* returnValue = NULL;
	PyObject* resultMatrix;
	MatrixObject* matrix = NULL;
	float scalar = 0;
	int rightISAFloat = PyFloat_Check(right);
	int leftISAFloat = PyFloat_Check(left);
	if (leftISAFloat == 0 && rightISAFloat == 1)
	{
		returnValue = doScalarMulMatrix(right, left, 1);

	}
	else
	{
		std::cout << "Wrong order" << std::endl;
	}
	return returnValue;
}


PyObject* toNumpyMatrix(PyObject* self) {
	PyObject* data = ((MatrixObject*)self)->data;
	return PyArray_Return((PyArrayObject*)PyArray_ContiguousFromAny(data, PyArray_FLOAT32, 0, 2, NPY_ARRAY_DEFAULT, NULL));
}


static PyMethodDef matrix_methods[] = {
	{ "toNumpy", (PyCFunction)toNumpyMatrix, METH_VARARGS},
	{NULL}
};


static PyMemberDef matrix_members[] = {
	{"row",  /* name */
	 offsetof(MatrixObject, row),  /* offset */
	 T_INT,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},

	 {"column",  /* name */
	 offsetof(MatrixObject, column),  /* offset */
	 T_INT,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},

	 {"data",  /* name */
	 offsetof(MatrixObject, data),  /* offset */
	 T_OBJECT_EX,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},

	 {NULL}
};

static PyNumberMethods matrixNumberMethods = { NULL };






static PyModuleDef vectormodule = {
	PyModuleDef_HEAD_INIT,
	"VectorModule",
	"Example module that creates an extension type.",
	-1,
	NULL, NULL, NULL, NULL, NULL
};




PyMODINIT_FUNC
PyInit_VectorModule(void)
{
	import_array();
	PyObject* m;
	vectorNumberMethods.nb_add = (binaryfunc)VectorSum; //operator+ overload
	vectorNumberMethods.nb_subtract = (binaryfunc)VectorSubstract; //operator- overload
	vectorNumberMethods.nb_multiply = (binaryfunc)VectorMul;
	vectorNumberMethods.nb_true_divide= (binaryfunc)VectorDivision;
	matrixNumberMethods.nb_add = (binaryfunc)MatrixSum;
	matrixNumberMethods.nb_subtract = (binaryfunc)MatrixSubstraction;
	matrixNumberMethods.nb_multiply = (binaryfunc)MatrixMul;
	matrixNumberMethods.nb_true_divide = (binaryfunc)MatrixDiv;
	vector_VectorType.tp_new = Vector_new;
	vector_VectorType.tp_as_number = &vectorNumberMethods;
	vector_VectorType.tp_basicsize = sizeof(vector_VectorObject);
	vector_VectorType.tp_members = vector_members;
	vector_VectorType.tp_repr = repr;
	vector_VectorType.tp_methods = vector_methods;
	vector_VectorType.tp_init = (initproc)vector_init;
	vector_VectorType.tp_dealloc = (destructor)Vector_dealloc;
	MatrixType.tp_new = Matrix_new;
	MatrixType.tp_as_number = &matrixNumberMethods;
	MatrixType.tp_basicsize = sizeof(MatrixObject);
	MatrixType.tp_members = matrix_members;
	MatrixType.tp_methods = matrix_methods;
	MatrixType.tp_init = (initproc)matrix_init;
	MatrixType.tp_dealloc = (destructor)Matrix_dealloc;
	

	if (PyType_Ready(&vector_VectorType) < 0)
		return NULL;
	if (PyType_Ready(&MatrixType) < 0)
		return NULL;

	m = PyModule_Create(&vectormodule);
	if (m == NULL)
		return NULL;

	Py_INCREF(&vector_VectorType);
	if (PyModule_AddObject(m, "vector", (PyObject*)& vector_VectorType) != 0) {
		return NULL;
	};
	Py_INCREF(&MatrixType);
	if (PyModule_AddObject(m, "matrix", (PyObject*)& MatrixType) != 0) {
		return NULL;
	};
	return m;
}

