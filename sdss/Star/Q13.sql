SELECT 
 p.run, 
 p.camCol,
 p.rerun,
 p.field,
 p."objID",
 p."specObjID",
 p.u, 
 p.g,
 p.r, 
 p.i, 
 p.z,
 p.ra, 
 p.dec, 
 p."err_u",
 p."err_g", 
 p."err_r", 
 p."err_i", 
 p."err_z",
 p."extinction_u", 
 p."extinction_g", 
 p."extinction_r", 
 p."extinction_i", 
 p."extinction_z"
from "Star" as p
WHERE 
 (p."dered_u" - p."dered_g") > -0.50
 and (p."dered_u" - p."dered_g") < +0.15
 and (p."dered_g" - p."dered_r") > -0.4
 and (p."dered_g" - p."dered_r") < +0.05
 and (p."dered_r" - p."dered_i") > -0.3
 and (p."dered_r" - p."dered_i") < 0.0
 and p.g > 10 and p."dered_g" < 20.5
 and p.u > 10 and p.u < 25
 and p.r > 10 and p.r < 25
 and p.i > 10 and p.i < 25
 and p.z > 10 and p.z < 25


--208ms
