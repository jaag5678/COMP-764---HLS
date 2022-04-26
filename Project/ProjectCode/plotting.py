#To plot graphs for the project

import matplotlib.pyplot as plt
import csv

m0 = open('merge0.text', 'r')
m1 = open('merge1.text', 'r')
m2 = open('merge2.text', 'r')
m3 = open('merge3.text', 'r')
m4 = open('merge4.text', 'r')
m5 = open('merge5.text', 'r')


plots0 = csv.reader(m0, delimiter=' ')
plots1 = csv.reader(m1, delimiter=' ')
plots2 = csv.reader(m2, delimiter=' ')
plots3 = csv.reader(m3, delimiter=' ')
plots4 = csv.reader(m4, delimiter=' ')
plots5 = csv.reader(m5, delimiter=' ')

plots = []
plots.append(plots0)
plots.append(plots1)
plots.append(plots2)
plots.append(plots3)
plots.append(plots4)
plots.append(plots5)

x = []
y = []
z = []

#xlabel = [0, 1000, 2000, 3000]

for plot in plots:
    xt = []
    yt = [] 
    zt = []
    for row in plot:
        xt.append(int(row[0]))
        yt.append(int(row[2]))
        zt.append(int(row[4]))
    x.append(xt)
    y.append(yt)
    z.append(zt)





plt.xlabel("Program Number")
plt.ylabel("Clock cycles saved")


#plt.xticks(xlabel)
plt.scatter(x[0], y[0], marker='x', s=1, label="Merge1")
plt.scatter(x[0], y[1], marker='x', s=1, label="Merge2")
plt.scatter(x[0], y[2], marker='x', s=1, label="Merge3")
plt.scatter(x[0], y[3], marker='x', s=1, label="Merge4")
plt.scatter(x[0], y[4], marker='x', s=1, label="Merge5")
plt.scatter(x[0], y[5], marker='x', s=1, label="Merge6")
plt.ylim(ymin=-1)
plt.ylim(ymax=5)
plt.legend(loc='upper right')
plt.savefig('m01.png')

plt.close()


plt.xlabel("Program Number")
plt.ylabel("Adders Saved on Merging")
plt.scatter(x[0], z[0], marker='x', s=20, label="Merge1", alpha=0.5)
plt.scatter(x[0], z[1], marker='x', s=20, label="Merge2", alpha=0.5)
plt.scatter(x[0], z[2], marker='x', s=20, label="Merge3", alpha=0.5)
plt.scatter(x[0], z[3], marker='x', s=20, label="Merge4", alpha=0.5)
plt.scatter(x[0], z[4], marker='x', s=20, label="Merge5", alpha=0.5)
plt.scatter(x[0], z[5], marker='x', s=20, label="Merge6", alpha=0.5)

plt.savefig('m02.png')