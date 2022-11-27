import matplotlib.pyplot as plt
import sys
import numpy as np

filename = sys.argv[1]

x = []
y = []
with open(filename) as f:
    lines = [line.strip() for line in f]
    for i in range(0, len(lines), 2):
        x.append(float(lines[i]))
        y.append(float(lines[i + 1]))


fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
plt.plot(x, y)
plt.axis('off')
ax2 = fig.add_axes(ax1.get_position())
ax2.patch.set_alpha(0)
ax1_xlim = ax1.get_xlim()
ax2.set_xlim([0.6, 1.0])
plt.savefig(f'{filename[:-3]}png')
# plt.show()