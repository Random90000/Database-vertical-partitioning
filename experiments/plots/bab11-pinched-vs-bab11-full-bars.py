from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

fig, (ax1, ax2) = plt.subplots(2, 1, sharex = True)

ax = fig.gca()

df = pd.DataFrame({
    'threshold' : [0 ,0.50 , 0.55 , 0.60 , 0.65 , 0.70 , 0.75 , 0.80 , 0.85 , 0.9  , 0.95 , 1.00],
    'full'      : [0, 4.720, 4.270, 4.895, 3.741, 3.765, 3.701, 3.553, 4.093, 3.970, 4.105, 0.00],
    'pinched'   : [0, 3.609, 3.657, 3.468, 3.446, 3.626, 3.596, 3.562, 3.922, 3.916, 4.043, 0.00]
})

df.plot(kind = 'bar', x = 'threshold', y = ['pinched', 'full'], ax = ax, fontsize = 10, rot = 0, color = ['#0d00f2', '#ff0000'])

ax1.set_title('bab11 on pinched table vs bab11 on full table', fontsize = 14)
ax1.set_ylabel('seconds', fontsize = 12)
ax2.set_xlabel('threshold', fontsize = 12)

ax1.set_yscale('log')
ax1.set_ylim(5,1000)

ax1.axhline(6.8920, color = 'black')
ax1.text(0,7.5000,'pinched not clusterized',rotation=0) #sdss_pinched
ax1.axhline(125.344, color = 'black')
ax1.text(0,135.000,'full not clusterized',rotation=0) #sdss_full

ax1.spines['bottom'].set_visible(False)
ax1.xaxis.tick_top()
ax1.tick_params(labeltop = 'off') 

ax2.set_ylim(3.2,5.0) 
ax2.spines['top'].set_visible(False)
ax2.xaxis.tick_bottom()

d = .01  
kwargs = dict(transform = ax1.transAxes, color = 'k', clip_on = False)
ax1.plot((-d, +d), (-d, +d), **kwargs)   
ax1.plot((1 - d, 1 + d), (-d, +d), **kwargs)
kwargs.update(transform = ax2.transAxes)
ax2.plot((-d, +d), (1 - d, 1 + d), **kwargs)
ax2.plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs) 

#bars = np.arange(12)

#fig, (ax, ax2) = plt.subplots(2, 1, sharex=True)

#ax.set_title('bab11', fontsize=14)
#plt.xlabel('threshold', fontsize=12)
#ax.yaxis.set_label_coords(-0.09,-0.01)
#ax.set_ylabel('seconds', fontsize=12)
#ax.axhline(6.8920, color = 'black')
#ax.text(0,6.8926,'sdss pinched not clusterized',rotation=0) #sdss_pinched
#ax.axhline(125.344, color = 'black')
#ax.text(0,125.344,'sdss full not clusterized',rotation=0) #sdss_full
#ax.set_ylim(75,85) 
#ax2.set_ylim(0,6)
#ax.spines['bottom'].set_visible(False)
#ax2.spines['top'].set_visible(False)
#ax.xaxis.tick_top()
#ax.tick_params(labeltop='off')  
#ax2.xaxis.tick_bottom()
#d = .01  
#kwargs = dict(transform=ax.transAxes, color='k', clip_on=False)
#ax.plot((-d, +d), (-d, +d), **kwargs)        # top-left diagonal
#ax.plot((1 - d, 1 + d), (-d, +d), **kwargs)  # top-right diagonal
#kwargs.update(transform=ax2.transAxes)  # switch to the bottom axes
#ax2.plot((-d, +d), (1 - d, 1 + d), **kwargs)  # bottom-left diagonal
#ax2.plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs)  # bottom-right diagon

#ax2.bar(bars,       [0, 4.720, 4.270, 4.895, 3.741, 3.765, 3.701, 3.553, 4.093, 3.970, 4.105, 0],color='white',edgecolor ='black') #sdss_full
#ax2.set_xticks(bars,[0 ,0.50 , 0.55 , 0.60 , 0.65 , 0.70 , 0.75 , 0.80 , 0.85 , 0.9  , 0.95 , 0])

#ax.bar(bars,       [0, 3.609, 3.657, 3.468, 3.446, 3.626, 3.596, 3.562, 3.922, 3.916, 4.043, 0],color='white',edgecolor ='red') #sdss_pinched
#ax.set_xticks(bars,[0 ,0.50 , 0.55 , 0.60 , 0.65 , 0.70 , 0.75 , 0.80 , 0.85 , 0.9  , 0.95 , 0])

#ax2.set_ylim(3,6)
plt.show()