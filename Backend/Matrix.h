#ifndef Matrix_h
#define Matrix_h

class Matrix {
public:
	PyObject_HEAD

private: PyObject* data;
	int row;
	int column;

public:
	Matrix(int, int, PyObject*);
	float* operator+(Matrix);
	float* operator-(Matrix);
	float* operator*(Matrix);
	float* operator*(float);
	float* operator/(float);

	void setRow(int);
	int getRow();
	void setColumn(int);
	int getColumn();
	void setData(PyObject*);
	PyObject* getData();

private:
	float* doStrassen(Matrix, Matrix);
	float* doMatrixSumOrSubtract(Matrix, Matrix, bool);
	float* doMatrixMulOrDiv(Matrix, float, bool);
	float* doMatrixMulMatrix(Matrix, Matrix);
};

#endif  // !Matrix_h
