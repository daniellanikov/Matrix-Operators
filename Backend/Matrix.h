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


PyObject* MatrixSum(MatrixObject* matrix1, MatrixObject* matrix2);
PyObject* MatrixSubstraction(MatrixObject* matrix1, MatrixObject* matrix2);
PyObject* MatrixMul(PyObject* left, PyObject* right);
PyObject* MatrixDiv(PyObject* left, PyObject* right);
PyObject* Matrix_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
PyObject* matrix_init(MatrixObject* self, PyObject* args, PyObject* kwds);
void Matrix_dealloc(MatrixObject* self);
PyObject* toNumpyMatrix(PyObject* self);


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