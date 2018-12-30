SELECT s1."objID" as s1
FROM "Star" S1
WHERE 
((S1.u - S1.g) < 0.4 ) 
and (S1.g - S1.r) < 0.7 
and (S1.r - S1.i) > 0.4 
and (S1.i - S1.z) > 0.4 

--280ms