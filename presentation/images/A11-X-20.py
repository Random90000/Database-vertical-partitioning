import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

plt.yscale('log')
plt.ylim(0.0001,100000)
plt.axis('auto')

plt.xticks([5   , 10   , 15   , 20   , 25   , 30   , 35   , 40   , 45   ,  50  ,  55  ,  60  ,  65  ,  70  ,  75  ,  80  ,  85  ,  90  ,  95  ,  100  ,  105  , 110   ,  115  ,  120  ,  125  ,  130  ,  135  ,  140  ,  145  ,  150  ,  155  ,  160  ,  165  ,  170  ,  175  ,  180  ,  185  ,  190  ,  195  ,  200  ,  205],
           ['5' , '10' , '15' , '20' , '25' , '30' , '35' , '40' , '45' , '50' , '55' , '60' , '65' , '70' , '75' , '80' , '85' , '90' , '95' , '100' , '105' , '110' , '115' , '120' , '125' , '130' , '135' , '140' , '145' , '150' , '155' , '160' , '165' , '170' , '175' , '180' , '185' , '190' , '195' , '200' , '205'])

plt.plot([5      , 10      , 15      , 20      , 25      , 30      , 35      , 40     , 45     ,  50      ,  55      ,  60     ,  65      ,  70      ,  75      ,  80      ,  85      ,  90      ,  95      ,  100     ,  105     , 110  , 115  , 120  , 125  , 130  , 135  , 140  , 145  , 150  , 155  , 160  , 165  , 170  , 175  , 180  , 185  , 190  , 200  , 205 ],
         [0.001  , 0.001   , 0.033   , 0.115   , 1.296   , 6.735   , 19.312  , 50.026 , 49.658 ,  119.270 ,  165.173 ,  92.169 ,  199.235 ,  241.581 ,  252.871 ,  290.985 ,  176.567 ,  338.088 ,  375.096 ,  337.224 ,  500.684 , 700  , 1000 , 1300 , 1600 , 1900 , 2200 , 2600 , 2900 , 3300 , 3600 , 3900 , 4200 , 4500 , 4800 , 5000 , 5300 , 5600 , 6000 , 6200],
         color='#0d00f2', linestyle='dashed', marker='s',markerfacecolor='#0d00f2', markersize=4)

plt.plot([5     , 10    , 15    , 20     , 25     , 30      , 35      , 40      , 45      , 50      , 55      , 60      , 65      , 70      , 75      , 80      , 85      , 90       , 95       , 100      , 105      , 110  , 115  , 120  , 125  , 130  , 135  , 140  , 145  , 150  , 155  , 160  , 165  , 170  , 175  , 180  , 185 ],
         [0.001 , 0.219 , 6.430 , 21.234 , 62.101 , 162.918 , 205.986 , 240.736 , 291.865 , 342.369 , 405.924 , 475.103 , 532.644 , 625.542 , 681.911 , 763.016 , 874.505 , 1023.816 , 1108.437 , 1107.341 , 1374.046 , 1600 , 1900 , 2200 , 2600 , 2900 , 3300 , 3600 , 3900 , 4200 , 4500 , 4800 , 5000 , 5300 , 5600 , 6000 , 6200],
         color='#5900a6', linestyle='dashed', marker='s',markerfacecolor='#5900a6', markersize=4)

plt.plot([5     , 10    , 15     , 20      , 25      , 30      , 35      , 40      , 45      , 50      , 55      , 60      , 65       , 70       , 75       , 80       , 85       , 90       , 95       , 100      , 105      , 110  , 115  , 120  , 125  , 130  , 135  , 140  , 145  , 150  , 155  , 160  , 165],
         [0.008 , 2.831 , 42.990 , 145.963 , 209.679 , 284.792 , 369.327 , 416.152 , 501.649 , 591.512 , 704.737 , 878.988 , 1010.690 , 1136.497 , 1283.119 , 1446.437 , 1620.946 , 1805.146 , 1911.889 , 2065.394 , 2681.869 , 2900 , 3300 , 3600 , 3900 , 4200 , 4500 , 4800 , 5000 , 5300 , 5600 , 6000 , 6200],
         color='#73008c', linestyle='dashed', marker='s',markerfacecolor='#73008c', markersize=4)        

plt.plot([5     , 10     , 15      , 20      , 25      , 30      , 35      , 40      , 45      , 50      , 55       , 60       , 65       , 70       , 75       , 80       , 85       , 90       , 95       , 100      , 105      , 110  , 115  , 120  , 125  , 130  , 135  , 140  , 145 ],
         [0.328 , 64.584 , 135.023 , 216.732 , 315.913 , 426.717 , 546.432 , 618.719 , 750.403 , 911.121 , 1083.879 , 1265.532 , 1496.390 , 1690.949 , 1886.177 , 2156.936 , 2392.865 , 2682.908 , 2940.417 , 3259.437 , 4017.002 , 4200 , 4500 , 4800 , 5000 , 5300 , 5600 , 6000 , 6200],
         color='#8c0073', linestyle='dashed', marker='s',markerfacecolor='#8c0073', markersize=4)         

plt.plot([5     , 10     , 15      , 20      , 25      , 30      , 35      , 40      , 45       , 50       , 55       , 60       , 65       , 70       , 75       , 80       , 85       , 90       , 95       , 100      , 105  , 110  , 115  , 120  , 125  , 130],
         [3.219 , 91.668 , 185.487 , 304.423 , 439.023 , 590.555 , 772.060 , 865.352 , 1038.589 , 1252.816 , 1475.297 , 1779.785 , 2070.711 , 2393.890 , 2710.756 , 3168.187 , 3435.526 , 3804.890 , 4225.417 , 4656.515 , 4800 , 5000 , 5300 , 5600 , 6000 , 6200],
         color='#a60059', linestyle='dashed', marker='s',markerfacecolor='#a60059', markersize=4)        

plt.plot([5      , 10      , 15      , 20      , 25      , 30      , 35      , 40       , 45       , 50       , 55       , 60       , 65       , 70       , 75       , 80       , 85       , 90       , 95       , 100     ],
         [16.312 , 119.440 , 239.464 , 388.657 , 560.396 , 769.184 , 993.238 , 1119.197 , 1413.621 , 1681.481 , 1934.331 , 2464.040 , 2794.999 , 3236.291 , 3663.728 , 3984.533 , 4442.279 , 4985.871 , 5578.078 , 6233.732],
         color='#bf0040', linestyle='dashed', marker='s',markerfacecolor='#bf0040', markersize=4)        

plt.plot([5      , 10      , 15      , 20      , 25      , 30      , 35       , 40       , 45       , 50       , 55       , 60       , 65       , 70       , 75       , 80       , 85       , 90       , 95       , 100     ],
         [39.347 , 145.664 , 296.082 , 471.115 , 680.131 , 925.487 , 1216.752 , 1382.458 , 1669.633 , 2004.735 , 2450.432 , 2910.233 , 3215.990 , 3680.055 , 4196.411 , 4704.447 , 5295.111 , 5830.082 , 6534.844 , 7370.147],
         color='#d90026', linestyle='dashed', marker='s',markerfacecolor='#d90026', markersize=4)       

plt.plot([5      , 10      , 15      , 20      , 25      , 30       , 35       , 40       , 45       , 50       , 55       , 60       , 65       , 70       , 75       , 80      ],
         [44.236 , 167.845 , 340.345 , 551.977 , 795.648 , 1077.211 , 1393.164 , 1611.681 , 1993.721 , 2398.437 , 2841.085 , 3631.496 , 4152.341 , 4760.285 , 5735.120 , 6486.560],
         color='#ff0000', linestyle='dashed', marker='s',markerfacecolor='#ff0000', markersize=4)

plt.plot([20   , 200  ],
         [8200 , 8200],
         color='#ffffff', linestyle='-', marker='s',markerfacecolor='#ffffff', markersize = 2, linewidth = 10)

plt.axhline(7200.000, color='black')

plt.text(10, 8000.000, 'more than 2 hours')

plt.title('A11 rows dependency', fontsize=14)
plt.xlabel('rows', fontsize=12)
plt.ylabel('seconds', fontsize=12)

c1 = mpatches.Patch(facecolor='#ff0000', label = '0.8', linewidth = 0.5, edgecolor = 'white')
c2 = mpatches.Patch(facecolor='#d90026', label = '0.7', linewidth = 0.5, edgecolor = 'white')
c3 = mpatches.Patch(facecolor='#bf0040', label = '0.6', linewidth = 0.5, edgecolor = 'white')
c4 = mpatches.Patch(facecolor='#a60059', label = '0.5', linewidth = 0.5, edgecolor = 'white')
c5 = mpatches.Patch(facecolor='#8c0073', label = '0.4', linewidth = 0.5, edgecolor = 'white')
c6 = mpatches.Patch(facecolor='#73008c', label = '0.3', linewidth = 0.5, edgecolor = 'white')
c7 = mpatches.Patch(facecolor='#5900a6', label = '0.2', linewidth = 0.5, edgecolor = 'white')
c8 = mpatches.Patch(facecolor='#0d00f2', label = '0.1', linewidth = 0.5, edgecolor = 'white')

legend = plt.legend(handles=[c1,c2,c3,c4,c5,c6,c7,c8], title = 'cohesion', loc = 4, fontsize = 10, fancybox = True)

frame = legend.get_frame() #sets up for color, edge, and transparency
frame.set_facecolor('white') #color of legend
frame.set_edgecolor('white') #edge color of legend
frame.set_alpha(1) #deals with transparency

plt.show()