SELECT
p."objID",p.ra, p.dec,
p."fiberMag_u", p."fiberMag_g", p."fiberMag_r", p."fiberMag_i", p."fiberMag_z",
p."psfMag_u", p."psfMag_g", p."psfMag_r", p."psfMag_i", p."psfMag_z",
p."modelMag_u", p."modelMag_g", p."modelMag_r", p."modelMag_i", p."modelMag_z",
p."modelMagErr_u", p."modelMagErr_g", p."modelMagErr_r", p."modelMagErr_i", p."modelMagErr_z",
p."petroR90_u", p."petroR90_g", p."petroR90_r", p."petroR90_i", p."petroR90_z"
FROM "photoObj" as p
WHERE
p."modelMagErr_g"<0.2 AND p."modelMagErr_r"<0.2 AND p."modelMagErr_i"<0.2
AND p."modelMag_g" - p."modelMag_r" < 0.0 AND p."psfMag_g" - p."psfMag_r" < 0.0
AND p."modelMag_r" - p."modelMag_i" > 0.6 AND p."psfMag_r" - p."psfMag_i" > 0.6
OR p."modelMag_g" - p."modelMag_r" > p."modelMag_r" - p."modelMag_i" - 1.0 
AND p."psfMag_g" - p."psfMag_r" > p."psfMag_r" - p."psfMag_i" - 1.0
OR p."modelMag_i" - p."modelMag_z" < -0.5 AND p."psfMag_i" - p."psfMag_z" < -0.5


--2776ms