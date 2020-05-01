import VectorModule
import numpy as np

arrVector = np.array([1, 2, 3, 4, 5], dtype=np.float32)
arrVector1 = np.array([1, 2, 3, 4, 5], dtype=np.float32)

print(VectorModule.vector(arrVector, arrVector1))

