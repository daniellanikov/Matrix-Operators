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



PyMODINIT_FUNC
PyInit_VectorModule(void)
{
	import_array();
	PyObject* m;
	vectorNumberMethods.nb_add = (binaryfunc)Vector::VectorSum; //operator+ overload
	
	vectorNumberMethods.nb_subtract = (binaryfunc)Vector::VectorSubstract; //operator- overload
	vectorNumberMethods.nb_multiply = (binaryfunc)Vector::VectorMul;
	vectorNumberMethods.nb_true_divide= (binaryfunc)Vector::VectorDivision;
	matrixNumberMethods.nb_add = (binaryfunc)MatrixSum;
	matrixNumberMethods.nb_subtract = (binaryfunc)MatrixSubstraction;
	matrixNumberMethods.nb_multiply = (binaryfunc)MatrixMul;
	matrixNumberMethods.nb_true_divide = (binaryfunc)MatrixDiv;
	vector_VectorType.tp_new = Vector::Vector_new;
	vector_VectorType.tp_as_number = &vectorNumberMethods;
	vector_VectorType.tp_basicsize = sizeof(vector_VectorObject);
	vector_VectorType.tp_members = vector_members;
	vector_VectorType.tp_methods = vector_methods;
	vector_VectorType.tp_init = (initproc)Vector::vector_init;
	vector_VectorType.tp_dealloc = (destructor)Vector::Vector_dealloc;
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
	m = PyModule_Create(&vectorModule);
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

