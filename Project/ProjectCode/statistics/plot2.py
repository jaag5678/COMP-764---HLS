#Plot bar graphs for each merge form 

import matplotlib.pyplot as plt
import csv
import numpy as np

mc0 = open('merge_clk_saved0.txt', 'r')
mc1 = open('merge_clk_saved1.txt', 'r')
mc2 = open('merge_clk_saved2.txt', 'r')
mc3 = open('merge_clk_saved3.txt', 'r')
mc4 = open('merge_clk_saved4.txt', 'r')
mc5 = open('merge_clk_saved5.txt', 'r')

plots0 = csv.reader(mc0, delimiter=' ')
plots1 = csv.reader(mc1, delimiter=' ')
plots2 = csv.reader(mc2, delimiter=' ')
plots3 = csv.reader(mc3, delimiter=' ')
plots4 = csv.reader(mc4, delimiter=' ')
plots5 = csv.reader(mc5, delimiter=' ')

plots = []
plots.append(plots0)
plots.append(plots1)
plots.append(plots2)
plots.append(plots3)
plots.append(plots4)
plots.append(plots5)

x = []
y = []

#xlabel = [0, 1000, 2000, 3000]

for plot in plots:
    xt = []
    yt = [] 
    for row in plot:
        xt.append(int(row[0]))
        yt.append(int(row[1]))
    x.append(xt)
    y.append(yt)

xt = np.arange(len(x[0]))

plt.xlabel("Number of clock cycles saved")

plt.ylabel("Number of programs")


plt.bar(xt-0.3, y[0], width=0.1, label='Merge0')
plt.bar(xt-0.2, y[1], width=0.1, label='Merge1')
plt.bar(xt-0.1, y[2], width=0.1, label='Merge2')
plt.bar(xt-0.0, y[3], width=0.1, label='Merge3')
plt.bar(xt+0.1, y[4], width=0.1, label='Merge4')
plt.bar(xt+0.2, y[5], width=0.1, label='Merge5')

plt.legend()

plt.savefig("per_merge_clck_saved.pdf")

plt.close()

ma0 = open('merge_add_saved0.txt', 'r')
ma1 = open('merge_add_saved1.txt', 'r')
ma2 = open('merge_add_saved2.txt', 'r')
ma3 = open('merge_add_saved3.txt', 'r')
ma4 = open('merge_add_saved4.txt', 'r')
ma5 = open('merge_add_saved5.txt', 'r')

plots0 = csv.reader(ma0, delimiter=' ')
plots1 = csv.reader(ma1, delimiter=' ')
plots2 = csv.reader(ma2, delimiter=' ')
plots3 = csv.reader(ma3, delimiter=' ')
plots4 = csv.reader(ma4, delimiter=' ')
plots5 = csv.reader(ma5, delimiter=' ')

plots = []
plots.append(plots0)
plots.append(plots1)
plots.append(plots2)
plots.append(plots3)
plots.append(plots4)
plots.append(plots5)

x = []
y = []

#xlabel = [0, 1000, 2000, 3000]

for plot in plots:
    xt = []
    yt = [] 
    for row in plot:
        xt.append(int(row[0]))
        yt.append(int(row[1]))
    x.append(xt)
    y.append(yt)

xt = np.arange(len(x[0]))

plt.xlabel("Number of clock cycles saved")

plt.ylabel("Number of programs")


plt.bar(xt-0.3, y[0], width=0.1, label='Merge0')
plt.bar(xt-0.2, y[1], width=0.1, label='Merge1')
plt.bar(xt-0.1, y[2], width=0.1, label='Merge2')
plt.bar(xt-0.0, y[3], width=0.1, label='Merge3')
plt.bar(xt+0.1, y[4], width=0.1, label='Merge4')
plt.bar(xt+0.2, y[5], width=0.1, label='Merge5')


plt.legend()

plt.savefig("per_merge_add_saved.pdf")
