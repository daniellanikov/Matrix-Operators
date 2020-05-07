#include <Python.h>
#include<arrayobject.h> // import array miatt
#include "structmember.h"
#include "PyMatrix.h"
#include "PyVector.h"
#include "Matrix.h"




PyMethodDef vectorMethods[] = {
	{ "toNumpy", (PyCFunction)PyVector::toNumpy, METH_VARARGS},
	{NULL}
};
struct PyModuleDef vectorModule = {
	PyModuleDef_HEAD_INIT,
	"VectorModule",
	"documentation is here",
	-1,
	vectorMethods
};

PyNumberMethods vectorNumberMethods = { NULL };

static PyMethodDef matrixMethods[] = {
	{ "toNumpy", (PyCFunction)PyMatrix::toNumpyMatrix, METH_VARARGS},
	{NULL}
};


static PyMemberDef matrixMembers[] = {
	{"row",  /* name */
	 offsetof(Matrix, row),  /* offset */
	 T_INT,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},

	 {"column",  /* name */
	 offsetof(Matrix, column),  /* offset */
	 T_INT,  /* type */
	 READONLY,  /* flags */
	 NULL  /* docstring */},

	 {"data",  /* name */
	 offsetof(Matrix, data),  /* offset */
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
	vectorNumberMethods.nb_add = (binaryfunc)PyVector::vectorSum; //operator+ overload

	vectorNumberMethods.nb_subtract = (binaryfunc)PyVector::vectorSubstract; //operator- overload
	vectorNumberMethods.nb_multiply = (binaryfunc)PyVector::vectorMul;
	vectorNumberMethods.nb_true_divide = (binaryfunc)PyVector::vectorDivision;
	matrixNumberMethods.nb_add = (binaryfunc)PyMatrix::matrixSum;
	matrixNumberMethods.nb_subtract = (binaryfunc)PyMatrix::matrixSubstraction;
	matrixNumberMethods.nb_multiply = (binaryfunc)PyMatrix::matrixMul;
	matrixNumberMethods.nb_true_divide = (binaryfunc)PyMatrix::matrixDiv;
	PyVector::vectorType.tp_new = PyVector::vectorNew;
	PyVector::vectorType.tp_as_number = &vectorNumberMethods;
	PyVector::vectorType.tp_basicsize = sizeof(Matrix);
	PyVector::vectorType.tp_members = matrixMembers;
	PyVector::vectorType.tp_methods = vectorMethods;
	PyVector::vectorType.tp_init = (initproc)PyVector::vectorInit;
	PyVector::vectorType.tp_dealloc = (destructor)PyVector::vectorDealloc;
	PyMatrix::matrixType.tp_new = PyMatrix::matrixNew;
	PyMatrix::matrixType.tp_as_number = &matrixNumberMethods;
	PyMatrix::matrixType.tp_basicsize = sizeof(Matrix);
	PyMatrix::matrixType.tp_members = matrixMembers;
	PyMatrix::matrixType.tp_methods = matrixMethods;
	PyMatrix::matrixType.tp_init = (initproc)PyMatrix::matrixInit;
	PyMatrix::matrixType.tp_dealloc = (destructor)PyMatrix::matrixDealloc;


	if (PyType_Ready(&PyVector::vectorType) < 0)
		return NULL;
	if (PyType_Ready(&PyMatrix::matrixType) < 0)
		return NULL;
	m = PyModule_Create(&vectorModule);
	if (m == NULL)
		return NULL;
	Py_INCREF(&PyVector::vectorType);
	if (PyModule_AddObject(m, "vector", (PyObject*)& PyVector::vectorType) != 0) {
		return NULL;
	};
	Py_INCREF(&PyMatrix::matrixType);
	if (PyModule_AddObject(m, "matrix", (PyObject*)& PyMatrix::matrixType) != 0) {
		return NULL;
	};
	return m;
}

