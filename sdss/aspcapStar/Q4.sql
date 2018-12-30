SELECT
 s.apogee_id, s.location_id,
 s.snr, s.vhelio_avg, s.vscatter,
 a.teff, a.teff_err, a.logg, a.logg_err,
 a.mg_fe, a.mg_fe_err
FROM "aspcapStar" a
 JOIN "apogeeStar" s on a.apstar_id = s.apstar_id
WHERE (a.teff != -9999.) 
and (a.mg_fe != -9999.) 
and (a.logg <> -9999.) 
and (a.teff != -1) 
and (a.mg_fe != 99) 
and (a.logg != 99) 
and (a.teff != -9999.99)
and (a.mg_fe != -9999.99)
and (a.logg <>-9999.99)

--975ms