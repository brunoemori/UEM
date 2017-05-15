import matplotlib.pyplot as plt

plt.plot([2, 4], [2, 4], "r")
plt.plot([2, 4], [1.83, 3.53], "g--")
plt.ylabel("Speedup")
plt.xlabel("Number of threads")
#plt.axis([2, 4, 2, 4])
plt.show()
