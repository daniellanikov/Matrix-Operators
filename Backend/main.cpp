#include <Python.h>
#include<arrayobject.h> // import array miatt
#include "structmember.h"

#include "Matrix.h"



#include "vectorclass.h"




PyMethodDef vector_methods[] = {
	{ "toNumpy", (PyCFunction)Vector::toNumpy, METH_VARARGS},
	{NULL}
};
struct PyModuleDef vectorModule = {
	PyModuleDef_HEAD_INIT,
	"VectorModule",
	"documentation is here",
	-1,
	vector_methods
};



PyMemberDef vector_members[] = {
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


PyNumberMethods vectorNumberMethods = { NULL };

static PyMethodDef matrix_methods[] = {
	{ "toNumpy", (PyCFunction)Matrix::toNumpyMatrix, METH_VARARGS},
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

PyMODINIT_FUNC
PyInit_VectorModule(void)
{
	import_array();
	PyObject* m;
	vectorNumberMethods.nb_add = (binaryfunc)Vector::VectorSum; //operator+ overload
	
	vectorNumberMethods.nb_subtract = (binaryfunc)Vector::VectorSubstract; //operator- overload
	vectorNumberMethods.nb_multiply = (binaryfunc)Vector::VectorMul;
	vectorNumberMethods.nb_true_divide= (binaryfunc)Vector::VectorDivision;
	matrixNumberMethods.nb_add = (binaryfunc)Matrix::MatrixSum;
	matrixNumberMethods.nb_subtract = (binaryfunc)Matrix::MatrixSubstraction;
	matrixNumberMethods.nb_multiply = (binaryfunc)Matrix::MatrixMul;
	matrixNumberMethods.nb_true_divide = (binaryfunc)Matrix::MatrixDiv;
	Vector::vector_VectorType.tp_new = Vector::Vector_new;
	Vector::vector_VectorType.tp_as_number = &vectorNumberMethods;
	Vector::vector_VectorType.tp_basicsize = sizeof(vector_VectorObject);
	Vector::vector_VectorType.tp_members = vector_members;
	Vector::vector_VectorType.tp_methods = vector_methods;
	Vector::vector_VectorType.tp_init = (initproc)Vector::vector_init;
	Vector::vector_VectorType.tp_dealloc = (destructor)Vector::Vector_dealloc;
	Matrix::MatrixType.tp_new = Matrix::Matrix_new;
	Matrix::MatrixType.tp_as_number = &matrixNumberMethods;
	Matrix::MatrixType.tp_basicsize = sizeof(MatrixObject);
	Matrix::MatrixType.tp_members = matrix_members;
	Matrix::MatrixType.tp_methods = matrix_methods;
	Matrix::MatrixType.tp_init = (initproc)Matrix::matrix_init;
	Matrix::MatrixType.tp_dealloc = (destructor)Matrix::Matrix_dealloc;
	

	if (PyType_Ready(&Vector::vector_VectorType) < 0)
		return NULL;
	if (PyType_Ready(&Matrix::MatrixType) < 0)
		return NULL;
	m = PyModule_Create(&vectorModule);
	if (m == NULL)
		return NULL;
	Py_INCREF(&Vector::vector_VectorType);
	if (PyModule_AddObject(m, "vector", (PyObject*)& Vector::vector_VectorType) != 0) {
		return NULL;
	};
	Py_INCREF(&Matrix::MatrixType);
	if (PyModule_AddObject(m, "matrix", (PyObject*)& Matrix::MatrixType) != 0) {
		return NULL;
	};
	return m;
}

