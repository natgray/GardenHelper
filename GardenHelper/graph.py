#Displays garden Helper data as graph
#Author: Jay Lowe

import matplotlib.pyplot as plt
import pandas as pd

# read file
df = pd.read_csv("337table.csv")

# modify columns
time = df.iloc[:,(0)]
for i in range (df.shape[0]-1 ):
	time[i] = time[i].split("-",1)[1]

moisture = df.iloc[:,(1)]
light = df.iloc[:,(2)]
temp = df.iloc[:,(3)]

# plot as a line graph
df.plot.line()
plt.legend(loc='upper right')
plt.show()