import numpy as np
import matplotlib.pyplot as plt

dict = {}
arr = np.zeros(5)
tmplst = []
tmpstr = ""
tmpcore = ""
sum = 0
with open('/Users/john/Programs/parallel_matmul/output/timings.txt', 'r') as f:
    for line in f:
        currentline = line.split(" ")
        if (currentline[0][0:4] == "SIZE"):
            sum = 0
            tmpstr = np.int32(currentline[0][5:])
            dict[tmpstr] = []
            continue
        elif currentline[0][0:4] == "core":
            if sum != 0:
                dict[tmpstr].append((tmpcore, sum/5))
            tmpcore = currentline[0][5:-1]
            sum = 0
            continue
        elif currentline[0][0:4] == "Time":
            sum += np.double(currentline[2])
            continue
        elif currentline[0][0:3] == "---":
            dict[tmpstr].append((tmpcore, sum/5))
            continue

core_cnt = 6
x = np.zeros(core_cnt)
y = np.zeros(core_cnt)
plt.figure(figsize=(10,10))
for k in dict:
    for i in range(core_cnt):
        x[i] = dict[k][i][0]
        y[i] = dict[k][i][1]
    plt.semilogy(x, y, "-o", label = "size = {}".format(k))
plt.title("Time vs No. of cores for different matrix sizes", fontsize = 20)
plt.xlabel("No. of cores", fontsize = 15)
plt.ylabel("Time (s)", fontsize = 15)
plt.legend()
plt.grid()
plt.savefig("../output/plot.png")