import VectorModule
import numpy as np


arrVector = np.array([[3.111, 4.111, 4, 4, 4]], dtype=np.float32)
arrVector1 = np.array([[1, 1, 1, 1, 1]], dtype=np.float32)
arrVector3 = np.array([[2, 2, 2, 2, 2]], dtype=np.float32)
arrVector4 = np.array([[6, 6, 6, 6, 6]], dtype=np.float32)
arrVector5 = np.array([[1], [3]], dtype=np.float32)

arrMatrix = np.array([[1,2],[1,2]], dtype=np.float32)
arrMatrix2 = np.array([[2,4],[2,4]], dtype=np.float32)
arrMatrix3 = np.array([[6,6,6],[6,6,6],[6,6,6]], dtype=np.float32)

matrix = VectorModule.matrix(arrMatrix)
matrix2 = VectorModule.matrix(arrMatrix2)
matrix3 = VectorModule.matrix(arrMatrix3)

vector1 = VectorModule.vector(arrVector)
vector2 = VectorModule.vector(arrVector3)
vector3 = VectorModule.vector(arrVector1)
vector4 = VectorModule.vector(arrVector4)
vector5 = VectorModule.vector(arrVector5)

print("(elementwise): V-V, V+V")
#V-V
difference = vector1 - vector2
print(difference.toNumpy())

#V+V
sumVector = vector1 + vector2
print(sumVector.toNumpy())


print("(elementwise): M-M, M+M")
#M-M
dif = matrix - matrix2
print(dif.toNumpy())

#M+M
sum = matrix + matrix2
print(sum.toNumpy())


print("(elementwise): V*S, M*S, V/S, M/S")
#s*V*s
product = 2.0 * vector1 * 3.0
print(product.toNumpy())

#s*M*s
matrixProduct = 2.0 * matrix * 2.0
print(matrixProduct.toNumpy())

#V/s
vectorDivision = vector4 / 2.0
print(vectorDivision.toNumpy())

#M/s
matrixDivision = matrix3 / 2.0
print(matrixDivision.toNumpy())


print("(NOT elementwise, regular matrix and vector multiplications): M*V, M*M")
#M*V
matrixMulVector = matrix * vector5
print(matrixMulVector.toNumpy())

#M*M
matrixMul = matrix * matrix2
print(matrixMul.toNumpy())

