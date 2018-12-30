SELECT 
  star.apogee_id, star."ra", star.dec, star.glon, star.glat,
  star.vhelio_avg, star.vscatter,
  obj.j, obj.h, obj.k, 
  obj.j_err, obj.h_err, obj.k_err, 
  aspcap.teff,
  aspcap.teff_err, 
  aspcap.logg,
  aspcap.logg_err, 
  aspcap.mg_fe,
  aspcap.mg_fe_err, 
  aspcap.al_fe,
  aspcap.al_fe_err
FROM "aspcapStar" aspcap
JOIN "apogeeStar" star on aspcap.apstar_id = star.apstar_id
JOIN "apogeeObject" obj on aspcap.target_id = obj.target_id
WHERE (aspcap.aspcapflag & '00100000000' = 0 and aspcap.teff > 0) 
AND (aspcap.mg_fe != -9999 and aspcap.mn_fe != -9999)

