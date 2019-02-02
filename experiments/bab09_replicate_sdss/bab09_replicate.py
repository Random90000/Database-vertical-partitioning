from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
import numpy as np


bars = np.arange(9)

fig, (ax, ax2) = plt.subplots(2, 1, sharex=True)

ax.set_title('bab09 replicate', fontsize=14)
plt.xlabel('threshold', fontsize=12)
ax.yaxis.set_label_coords(-0.1,-0.01)
ax.set_ylabel('seconds', fontsize=12)
ax.axhline(125.3, color = 'black')
ax.text(0,125.4,'not clusterized',rotation=0)
ax.set_ylim(124,127) 
ax2.set_ylim(0,6)
ax.spines['bottom'].set_visible(False)
ax2.spines['top'].set_visible(False)
ax.xaxis.tick_top()
ax.tick_params(labeltop='off')  
ax2.xaxis.tick_bottom()
d = .01  
kwargs = dict(transform=ax.transAxes, color='k', clip_on=False)
ax.plot((-d, +d), (-d, +d), **kwargs)        # top-left diagonal
ax.plot((1 - d, 1 + d), (-d, +d), **kwargs)  # top-right diagonal
kwargs.update(transform=ax2.transAxes)  # switch to the bottom axes
ax2.plot((-d, +d), (1 - d, 1 + d), **kwargs)  # bottom-left diagonal
ax2.plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs)  # bottom-right diagon
ax2.bar(bars,[0,3.5, 3.4, 3.8, 3.7, 3.5, 4.9, 4.7,0],color='white',edgecolor ='black')
plt.xticks(bars,[0,0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80,0.9])
ax2.set_ylim(3,5)
plt.show()

