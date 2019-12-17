import matplotlib.pyplot as plt

data = [[0] * 9 for i in range(9)]

while True:
    try:
        x=int(input())
        y=int(input())
        data[x][y]+=1
    except:
        break

fig=plt.figure()
ax=fig.add_subplot(111)
ax.set_xticks(range(9))
ax.set_yticks(range(9))
plt.colorbar(ax.imshow(data,cmap=plt.cm.hot_r))
plt.title("Hot table")
plt.show()