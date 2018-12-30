select "objID","ra","dec","u","g","r","i","z","modelMagErr_u","modelMagErr_g","modelMagErr_r",
"modelMagErr_i","modelMagErr_z","petroMag_u","petroMag_g","petroMag_r","petroMag_i",
"petroMag_z","petroMagErr_u","petroMagErr_g","petroMagErr_r","petroMagErr_i","petroMagErr_z",
"psfMag_u","psfMag_g","psfMag_r","psfMag_i","psfMag_z","extinction_i"
from "photoObj"
where ra between 27.9 and 28.5 and dec between 0.75 and 1.3

--145ms