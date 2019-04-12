from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

ax = plt.figure().gca()

plt.yscale('log')
plt.ylim(0.001,100000)

df = pd.DataFrame({ 
    'threshold'      : [ 0.50  , 0.55  , 0.60   , 0.65  , 0.70   , 0.75   , 0.80   , 0.85   , 0.9    , 0.95   ],
    'A11'           : [ 0.06 , 0.061 , 0.057  , 0.060 , 0.107  , 0.108  , 0.136  , 0.142  , 0.120  , 0.119   ],
    'A09_nearest'   : [ 0.677 , 2.294 , 2.310  , 2.293 , 16.889 , 16.873 , 17.032 , 38.677 , 42.080 , 135.665],
    'A09_separate'  : [ 0.674 , 2.339 , 2.966 , 2.322 , 17.250 , 16.888 , 17.119 , 37.636 , 41.746 , 138.354 ],
    'A09_replicate' : [ 0.727 , 2.445 , 2.433  , 2.428 , 17.490 , 17.081 , 17.028 , 37.281 , 41.423 , 135.480]
})

dfplot = df.plot(kind = 'bar', x = 'threshold', y = ['A11', 'A09_nearest', 'A09_separate','A09_replicate'],
        ax = ax, fontsize = 10, rot = 0, color = ['#0d00f2','#73008c','#a60059', '#ff0000'])

plt.plot([-2,4.24],[17.492,17.492], color = 'black')
plt.text(0,20,'17.49',rotation=0)
plt.plot([-2,9.24],[146,146], color = 'black')
plt.text(0,160,'138.35',rotation=0)
plt.title('clustering speed', fontsize = 14)
plt.ylabel('seconds', fontsize = 12)
plt.xlabel('threshold', fontsize = 12)
plt.show()
