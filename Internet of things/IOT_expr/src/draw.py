import matplotlib.pyplot as plt

x=[10000,20000,30000,40000,50000,60000,70000,80000,90000,100000]
y=[3188.85,6316.9,9381.56,10952.2,11365.1,11091.9,10776,10625.6,10465.5,10553.6]
z=[10000,20000,29669,34446,32125,29586,27708,26690,25970,26180]
plt.figure()
plt.plot(x,z)
plt.xlabel("Incoming packet(s)")
# plt.ylabel("Average delay(ms)")
plt.ylabel("Finished packet(s)")
# plt.title("Incoming packet - Average delay Table")
plt.title("Incoming packet - Finished packet Table")
plt.show()