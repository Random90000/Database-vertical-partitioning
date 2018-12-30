 select 
 s."objID",
 s.ra,
 s.dec,
 s.u,
 s.g,
 s.r,
 s.i,
 s.z
 from "Star" as s where s.g < 21.2

 --990ms