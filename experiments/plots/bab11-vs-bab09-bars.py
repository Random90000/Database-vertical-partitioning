from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

fig, (ax1, ax2) = plt.subplots(2, 1, sharex = True)

ax = fig.gca()

df = pd.DataFrame({ #0.6 near
    'threshold'       : [0 ,0.50 , 0.55 , 0.60 , 0.65 , 0.70 , 0.75 , 0.80 , 0.85 , 0.9  , 0.95 , 1.00],
    'bab11'           : [0, 4.720, 4.270, 4.896, 3.741, 3.765, 3.701, 3.553, 4.093, 3.970, 4.105, 0.00],
    'bab09_nearest'   : [0, 6.037, 5.092, 5.003, 4.993, 4.035, 4.345, 3.862, 4.540, 4.230, 5.579, 0.00],
    'bab09_separate'  : [0, 5.630, 4.866, 4.252, 3.983, 3.915, 3.598, 3.500, 4.206, 5.069, 5.330, 0.00],
    'bab09_replicate' : [0, 3.472, 3.358, 3.788, 3.720, 3.479, 4.889, 4.709, 4.727, 4.550, 4.575, 0.00]
})

df.plot(kind = 'bar', x = 'threshold', y = ['bab11', 'bab09_nearest', 'bab09_separate','bab09_replicate'],
        ax = ax, fontsize = 10, rot = 0, color = ['#0d00f2','#73008c','#a60059', '#ff0000'])

ax1.set_title('bab11 vs bab09', fontsize = 14)
ax1.set_ylabel('seconds', fontsize = 12)
ax2.set_xlabel('threshold', fontsize = 12)

ax1.set_yscale('log')
ax1.set_ylim(5,1000)

ax1.axhline(125.344, color = 'black')
ax1.text(0,135.000,'not clusterized',rotation=0) 

ax1.spines['bottom'].set_visible(False)
ax1.xaxis.tick_top()
ax1.tick_params(labeltop = 'off') 

ax2.set_ylim(3.2,6.2) 
ax2.spines['top'].set_visible(False)
ax2.xaxis.tick_bottom()

d = .01  
kwargs = dict(transform = ax1.transAxes, color = 'k', clip_on = False)
ax1.plot((-d, +d), (-d, +d), **kwargs)   
ax1.plot((1 - d, 1 + d), (-d, +d), **kwargs)
kwargs.update(transform = ax2.transAxes)
ax2.plot((-d, +d), (1 - d, 1 + d), **kwargs)
ax2.plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs) 

plt.show()