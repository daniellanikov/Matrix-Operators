#ifndef Matrix_h
#define Matrix_h


class MatrixObject {

public:

	PyObject_HEAD
	PyObject* data;
	int row;
	int column;
	MatrixObject(int, int, PyObject*);
	MatrixObject operator+ (MatrixObject);
	
};

#endif // !Matrix_h
