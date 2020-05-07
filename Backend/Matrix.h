
class MatrixObject {

public:

	PyObject_HEAD
	PyObject* data;
	int row;
	int column;

	MatrixObject(int, int, PyObject*);
	MatrixObject operator+ (MatrixObject);
	
};