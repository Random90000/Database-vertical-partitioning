SELECT 
 "objID",
 "ra",
 "dec",
 "psfMag_u",
 "psfMag_g",
 "psfMag_r",
 "psfMag_i",
 "psfMag_z",
 "run",
 "rerun",
 "camcol",
 "field", 
 "extinction_u",
 "extinction_g",
 "extinction_r",
 "extinction_i",
 "extinction_z",
 "flags",
 "b",
 "i",
 "type"
FROM "Star"
WHERE 
 "psfMag_z"-"extinction_z" < 20.2
 AND ("psfMag_i"-"extinction_i")-("psfMag_z"-"extinction_z") > 2.2
 AND "psfMag_u"-"extinction_u" <= 22.0 
 AND "psfMag_g"-"extinction_g" <= 22.2
 AND "psfMag_r"-"extinction_r" <= 22.2
 AND "psfMag_i"-"extinction_i" <= 21.3


 --3974ms