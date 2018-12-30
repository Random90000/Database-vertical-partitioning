SELECT ra, "dec", "modelMag_u", "modelMag_g", "modelMag_r", "modelMag_i", "modelMag_z" AS z, "objID"
FROM "photoObj"
WHERE ra BETWEEN 194.9 AND 195.1 AND
"modelMag_g" < 18.0 AND
"modelMag_u"-"modelMag_g" > 2.2 AND

--3464ms