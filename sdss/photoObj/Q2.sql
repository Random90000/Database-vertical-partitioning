select
"objID","SpecObjID","primTarget","type","flags","psfMag_u","psfMag_g","psfMag_r","psfMag_i","psfMag_z",
"petroMag_u","petroMag_g","petroMag_r","petroMag_i","petroMag_z","ra","dec","petroR50_u",
"modelMag_u","modelMag_g","modelMag_r","modelMag_i","modelMag_z","petroR50_g","petroR50_r",
"petroR50_i","petroR50_z","extinction_u","extinction_g","extinction_r",
"extinction_i","extinction_z","dered_u","dered_g","dered_r","dered_i","dered_z",
"u","g","r","i","z" from "photoObj" where
"objID" > 587739214426194000 and "objID" <= 587739508714924125
and ra > 150 and ra < 180
and dec > 30 and dec < 45

--142msw