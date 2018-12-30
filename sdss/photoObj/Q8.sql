select 
s."bestObjID",
s."specClass",
p.ra,
p.dec,
p.u,
p.g,
p.r,
p.i,
p.z,
s.z as redshift,
p."petroRad_r",
p."psfMag_r",
p."modelMag_r",
(p."psfMag_r" - p."modelMag_r") as compactness, 
(p."petroMag_r" + (2.5*log(2*Pi()*(p."petroRad_r" * p."petroRad_r")))) as SB
from "photoObj" as p, "specObjAll" as s
where 
    p.ra between 0 and 58.6
    AND p.dec between -1.26 and 1.26
    AND (s."specClass" = 2 or s."specClass" = 3)
    AND p."objID" = s."bestObjID"
    AND s.z between 0.002 and 6
    AND (p."psfMag_r" - p."modelMag_r") between -0.05 and 5
    AND p."petroMag_r" < 17.8
    AND (p."petroMag_r" + (2.5*log(2*Pi()*(p."petroRad_r" * p."petroRad_r")))) < 24
    OR p.ra between 310 and 360
    AND p.dec between -1.26 and 1.26
    AND (s."specClass" = 2 or s."specClass" = 3)
    AND p."objID" = s."bestObjID"
    AND s.z between 0.002 and 6
    AND (p."psfMag_r" - p."modelMag_r") between -0.05 and 5
    AND p."petroMag_r" < 17.8
    AND (p."petroMag_r" + (2.5*log(2*Pi()*(p."petroRad_r" * p."petroRad_r")))) < 24


--451ms