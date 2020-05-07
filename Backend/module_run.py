import VectorModule
import numpy as np


arrVector = np.array([3.111, 4.111, 4, 4, 4], dtype=np.float32)
arrVector1 = np.array([1, 1, 1, 1, 1], dtype=np.float32)
arrVector3 = np.array([2, 2, 2, 2, 2], dtype=np.float32)
arrVector4 = np.array([6, 6, 6, 6, 6], dtype=np.float32)
arrMatrix = np.array([[1,1,1],[2,2,2],[3,3,3]], dtype=np.float32)
arrMatrix2 = np.array([[6,6,6],[6,6,6],[6,6,6]], dtype=np.float32)
print("lofasz1")

matrix = VectorModule.matrix(arrMatrix)
print("lofasz2")
matrix2 = VectorModule.matrix(arrMatrix2)
vector1 = VectorModule.vector(arrVector)
print("lofasz3")
vector2 = VectorModule.vector(arrVector3)
vector3 = VectorModule.vector(arrVector1)
vector4 = VectorModule.vector(arrVector4)
sumVector = vector1 + vector2


print("lofasz4")
sum = matrix - matrix2
product = 2.0 * vector1 * 3.0
vectorProduct = vector1 * vector2
vectorDivision = vector4 / 2
sumVector = vector1 + vector2
sum = matrix - matrix2
difference = vector1 - vector2
matrixProduct = 2.0 * matrix * 2.0
matrixDivision = matrix2 / 2.0

print(matrixDivision.toNumpy())
print(sumVector.toNumpy())
print(vectorProduct.toNumpy())
print(vectorDivision.toNumpy())
print(sum.toNumpy())
print(difference.toNumpy())


