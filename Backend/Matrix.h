#ifndef Matrix_h
#define Matrix_h


class Matrix {

public:

	PyObject_HEAD
	PyObject* data;
	int row;
	int column;
	Matrix(int, int, PyObject*);
	float* operator+ (Matrix);
	float* operator-(Matrix);
	float* operator*(Matrix);
	float* operator*(float);
	float* operator/(float);
	

private:

	float* doMatrixSumOrSubtract(Matrix, Matrix, bool);
	float* doMatrixMulOrDiv(Matrix, float, bool);
	float* doMatrixMulMatrix(Matrix, Matrix);
};

#endif // !Matrix_h
