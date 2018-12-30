SELECT ra, "dec", "psfMag_u", "psfMagErr_u", "psfMag_g", "psfMagErr_g", "psfMag_r", "psfMagErr_r", "psfMag_i", "psfMagErr_i", "psfMag_z", "psfMagErr_z"
FROM "Star" 
WHERE (ra between 162.446533 and 162.646533) and (dec between -0.109789 and 0.090211) 

--241ms