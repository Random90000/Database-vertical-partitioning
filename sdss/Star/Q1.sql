SELECT 
 "objID", 
 "extinction_u","extinction_g", "extinction_r",
 "l", "b" 
from "Star"
 WHERE 
 "clean" = 1
 and (l between 150.055 and 150.058) and (b between -12.087 and -12.085)
 and "type" = 6

--3989ms