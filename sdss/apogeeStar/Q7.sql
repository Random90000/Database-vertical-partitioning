SELECT
 star.apogee_id, star.ra, star.dec, star.glon, star.glat, 
 star.vhelio_avg, star.vscatter,
 obj.j, obj.h, obj.k, obj.ak_targ, obj.ak_targ_method, obj.ak_wise,
 aspcap.teff, aspcap.logg, aspcap.param_n_m
FROM "apogeeStar" star
 JOIN "aspcapStar" aspcap on aspcap.apstar_id = star.apstar_id
 JOIN "apogeeObject" obj on aspcap.target_id = obj.target_id
WHERE aspcap.teff > 0
 and (star.vhelio_avg > 300)
 and star.nvisits > 2 order by aspcap.apogee_id


--3171ms