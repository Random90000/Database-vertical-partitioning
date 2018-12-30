SELECT 
    photoObj."petroMag_i", photoObj."petroMag_u", photoObj."petroMag_g", photoObj."petroMag_r", 
    photoObj."petroMag_z", photoObj."psfMag_u", photoObj."psfMag_g", photoObj."psfMag_r", photoObj."psfMag_i",
    photoObj."psfMag_z", photoObj."ra", photoObj."dec", SpecObjAll."zErr", SpecObjAll."specClass" 
    from "photoObj" as photoObj, "specObjAll" as SpecObjAll
WHERE 
    photoObj."SpecObjID" = SpecObjAll."specObjID"
    and SpecObjAll.z >0.2 and SpecObjAll.z < 0.3
    and photoObj."petroMag_i" > 17.5 
    and (photoObj."petroMag_r" > 15.5 or photoObj."petroR50_r" > 2) 
    and (photoObj."petroMag_r" > 0 and photoObj."g" > 0 and photoObj."r" > 0 and photoObj."i" > 0) 
    and ( (photoObj."petroMag_r"-photoObj."extinction_r") < 19.2 )
    and (photoObj."petroMag_r" - photoObj."extinction_r" < 
    (13.1 + (7/3) * (photoObj."dered_g" - photoObj."dered_r") + 4 * (photoObj."dered_r" - photoObj."dered_i") 
    - 4 * 0.18) ) 
    and ( (photoObj."dered_r" - photoObj."dered_i" - (photoObj."dered_g" - photoObj."dered_r")/4 - 0.18) < 0.2) 
    and ( (photoObj."dered_r" - photoObj."dered_i" - (photoObj."dered_g" - photoObj."dered_r")/4 - 0.18) > -0.2) 
    
    --and (( (photoObj."petroMag_r" - photoObj."extinction_r" + 
    ---2.5 * LOG(2 * 3.1415 * photoObj."petroR50_r" * photoObj."petroR50_r")) < 24.2) ) 
    --or ( (photoObj."petroMag_r" - photoObj."extinction_r" < 19.5) 
    --and ( (photoObj."dered_r" - photoObj."dered_i" - (photoObj."dered_g" - photoObj."dered_r")/4 - 0.18) > (0.45 - 4 * 
    --(photoObj."dered_g" - photoObj."dered_r")) ) 
    --and ( (photoObj."dered_g" - photoObj."dered_r") > (1.35 + 0.25 * (photoObj."dered_r" - photoObj."dered_i")) ) ) 

--6404ms
