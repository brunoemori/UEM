import matplotlib.pyplot as plt

plt.plot([2, 4], [2, 4])
plt.plot([2, 4], [1.99, 3.717])
plt.ylabel("Speedup")
plt.xlabel("Threads")
plt.axis([2, 4, 2, 4])
plt.show()
