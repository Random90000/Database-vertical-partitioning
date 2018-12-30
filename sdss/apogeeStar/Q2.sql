 SELECT 
 star.apstar_id,
 star.apogee_id, 
 star.ra,
 star.dec,
 star.glon,
 star.glat,
 star.apogee_target1,
 star.apogee_target2, 
 aspcap.teff,aspcap.logg,aspcap.param_n_m
FROM "apogeeStar" as star
 JOIN "aspcapStar" as "aspcap" on aspcap.apstar_id = star.apstar_id

--1221ms