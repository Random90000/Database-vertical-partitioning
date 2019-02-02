from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
import numpy as np

ax = plt.subplot(111)
plt.xlabel('threshold', fontsize=12)
plt.ylabel('seconds', fontsize=12)
ax.set_yscale('log')
ax.set_title('bab11', fontsize=14)
ax.plot([0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.9, 0.95,],
        [3.4,  3.4,  3.7,  3.6,  3.2,  3.1,  3.2,  3.2,  3.4, 4.0], color = 'black')
ax.axhline(125.3, color = 'black')
ax.text(0.7,0.65,'not clusterized', horizontalalignment='center', verticalalignment='center', transform=ax.transAxes)
ax.plot([0.8],[1000],color='white')
plt.show()