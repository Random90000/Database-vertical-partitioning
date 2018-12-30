SELECT
 s.apogee_id,s.ra, s.dec, s.glon, s.glat,
 s.vhelio_avg,s.vscatter,
 a.teff,a.logg, a.param_n_m, a.param_c_m_flag
FROM "apogeeStar" as s
 JOIN "aspcapStar" as "a" on a.apstar_id = s.apstar_id
WHERE (a.aspcapflag & 1) = 0 
 and a.teff > 0 and a.param_n_m < -2

--46ms