import numpy as np
M = 0
N = 0
a = []
b = []
c = []
matname = ""
with open('/Users/john/Programs/parallel_matmul/output/mat.txt', 'r') as f:
    for line in f:
        currentline = line.split(",")
        if (currentline[0] == "A" or currentline[0] == "B" 
            or currentline[0] == "C"):
            matname = currentline[0]
            continue
        else:
            if(matname == "A"):
                a.append([np.double(num) for num in currentline])
            elif (matname == "B"):
                b.append([np.double(num) for num in currentline])
            elif (matname == "C"):
                c.append([np.double(num) for num in currentline])

A = np.array(a)
B = np.array(b)
C = np.array(c)
print("Matrix Shapes")
print("A ", A.shape)
print("B ", B.shape)
print("C ", C.shape)
Ccomp = np.around(A@B, 10)
#print(C - Ccomp)
print("Frobenius norm: ", np.linalg.norm(C - Ccomp, ord='fro'))#/np.sqrt(C.shape[0]*C.shape[1]))
print("\n")

       

       


