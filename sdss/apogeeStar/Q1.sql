SELECT
 s.apogee_id,
 s.ra,
 s.dec,
 s.glon,
 s.glat,
 s.vhelio_avg,
 s.vscatter,
 a.teff, a.teff_err, a.logg, a.logg_err, a.param_c_m, a.param_c_m_flag,
 a.param_n_m, a.param_n_m_flag
FROM "apogeeStar" s
 JOIN "aspcapStar" a on a.apstar_id = s.apstar_id
WHERE 
 s.commiss = 0

 --2969ms
 