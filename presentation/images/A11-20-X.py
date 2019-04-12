import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

plt.yscale('log')
plt.ylim(0.0001,100000)

plt.xticks([ 5  ,  10  ,  15  ,  20  ,  25  ,  30  ,  35  ,  40 ],
           ['5' , '10' , '15' , '20' , '25' , '30' , '35' , '40'])

plt.plot([5     , 10    , 15    , 20      , 25       , 30     , 35     , 40       ],
         [0.001 , 0.001 , 0.045 , 0.058   , 5.145    , 27.463 , 602.759, 7200.000 ],
         color='#0d00f2', linestyle='dashed', marker='s',markerfacecolor='#0d00f2', markersize=4)

plt.plot([5     , 10    , 15    , 20      , 25       , 30      ],
         [0.001 , 0.008 , 0.863 , 21.099  , 3729.157 , 7200.000],
         color='#5900a6', linestyle='dashed', marker='s',markerfacecolor='#5900a6', markersize=4)

plt.plot([5     , 10    , 15    , 20      , 25       , 30      ],
         [0.001 , 0.048 , 2.876 , 143.425 , 6320.426 , 7200.000],
         color='#73008c', linestyle='dashed', marker='s',markerfacecolor='#73008c', markersize=4)        

plt.plot([5     , 10    , 15    , 20      , 25       ],
         [0.001 , 0.069 , 3.982 , 199.750 , 9476.553 ],
         color='#8c0073', linestyle='dashed', marker='s',markerfacecolor='#8c0073', markersize=4)         

plt.plot([5     , 10    , 15    , 20      , 25       ],
         [0.001 , 0.092 , 5.447 , 277.855 , 7200.000 ],
         color='#a60059', linestyle='dashed', marker='s',markerfacecolor='#a60059', markersize=4)        

plt.plot([5     , 10    , 15    , 20      , 25       ],
         [0.002 , 0.118 , 7.104 , 359.868 , 7200.000 ],
         color='#bf0040', linestyle='dashed', marker='s',markerfacecolor='#bf0040', markersize=4)        

plt.plot([5     , 10    , 15    , 20      , 25       ],
         [0.002 , 0.140 , 8.699 , 441.354 , 7200.000 ],
         color='#d90026', linestyle='dashed', marker='s',markerfacecolor='#d90026', markersize=4)       

plt.plot([5     , 10    , 15    , 20      , 25       ],
         [0.002 , 0.163 , 9.668 , 503.923 , 7200.000 ],
         color='#ff0000', linestyle='dashed', marker='s',markerfacecolor='#ff0000', markersize=4)

plt.plot([20       , 40      ],
         [9476.553 , 9476.553],
                color='#ffffff', linestyle='-', marker='s',markerfacecolor='#ffffff', markersize = 2, linewidth = 10)

#plt.plot([5     , 10    , 15    , 20      , 25       ],
#         [0.001 , 0.001 , 0.063 , 18.917  , 4931.100 ],
#         color='#3300cc', linestyle='dashed', marker='s',markerfacecolor='#3300cc', markersize=4)

plt.axhline(7200.000, color='black')

plt.text(10, 8000.000, 'more than 2 hours')

plt.title('A11 columns dependency', fontsize=14)
plt.xlabel('columns', fontsize=12)
plt.ylabel('seconds', fontsize=12)

c1 = mpatches.Patch(facecolor='#0d00f2', label = '0.1', linewidth = 0.5, edgecolor = 'white')
c2 = mpatches.Patch(facecolor='#5900a6', label = '0.2', linewidth = 0.5, edgecolor = 'white')
c3 = mpatches.Patch(facecolor='#73008c', label = '0.3', linewidth = 0.5, edgecolor = 'white')
c4 = mpatches.Patch(facecolor='#8c0073', label = '0.4', linewidth = 0.5, edgecolor = 'white')
c5 = mpatches.Patch(facecolor='#a60059', label = '0.5', linewidth = 0.5, edgecolor = 'white')
c6 = mpatches.Patch(facecolor='#bf0040', label = '0.6', linewidth = 0.5, edgecolor = 'white')
c7 = mpatches.Patch(facecolor='#d90026', label = '0.7', linewidth = 0.5, edgecolor = 'white')
c8 = mpatches.Patch(facecolor='#ff0000', label = '0.8', linewidth = 0.5, edgecolor = 'white')

legend = plt.legend(handles=[c8,c7,c6,c5,c4,c3,c2,c1], title = 'cohesion', loc = 4, fontsize = 10, fancybox = True)

frame = legend.get_frame() #sets up for color, edge, and transparency
frame.set_facecolor('white') #color of legend
frame.set_edgecolor('white') #edge color of legend
frame.set_alpha(1) #deals with transparency

plt.show()
