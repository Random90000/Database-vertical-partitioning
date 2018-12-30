select
p."objID", p."ra", p."dec", p."petroMag_u", p."petroMag_g", 
p."petroMag_r", p."petroMag_i", p."petroMag_z",  specObj."z"
from "photoObj" as p, "specObjAll" as specObj
where
specObj."bestObjID" = p."objID"
AND
p."ra" BETWEEN 165 and 168
AND p."dec" BETWEEN 22 and 25
AND ((p."petroMag_r"+5)-(5*LOG((3.0*(10^5)* specObj.z)/70.0))>-17.0)
AND p."petroMag_g"-p."petroMag_r" < 0.6
AND p."petroMag_u"-p."petroMag_r" < 1.3
AND specObj."specClass" = 2
AND specObj.z<1


--447ms