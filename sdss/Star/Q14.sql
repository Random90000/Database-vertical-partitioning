SELECT 
S1."objID" as objID1, 
S1.u as u1,
S1."err_u",
S1.ra,
S2."objID" as objID2, 
S2.u as u2,
S2."Err_u",
S2.ra
FROM "Star" as S1, 
 "photoObj" as S2, 
 "neighbors" as N 
WHERE S1."objID" = N."objID"
 and S2."objID" = N."NeighborObjID"
 and distance < 0.5/60 
 and S1.run != S2.run 
 and S1.u between 1 and 27 
 and S1.g between 1 and 27
 and S1.r between 1 and 27
 and S1.i between 1 and 27
 and S1.z between 1 and 27
 and S2.u between 1 and 27 
 and S2.g between 1 and 27
 and S2.r between 1 and 27
 and S2.i between 1 and 27
 and S2.z between 1 and 27
 and ( 
 abs(S1.u-S2.u) > .1 + (abs(S1."err_u") + abs(S2."Err_u")) 
 or abs(S1.g-S2.g) > .1 + (abs(S1."err_g") + abs(S2."Err_g")) 
 or abs(S1.r-S2.r) > .1 + (abs(S1."err_r") + abs(S2."Err_r")) 
 or abs(S1.i-S2.i) > .1 + (abs(S1."err_i") + abs(S2."Err_i")) 
 or abs(S1.z-S2.z) > .1 + (abs(S1."err_z") + abs(S2."Err_z")) 
 )
and S1.ra BETWEEN 0 AND 1.5


--669ms