SELECT 
a.apogee_id, 
to_char(a.glon::real,'999D9') as glon, 
to_char(a.glat::real,'999D9') as glat, 
a.snr, 
a.vhelio_avg,
a.vscatter, 
q.teff, 
q.logg, 
q.m_h, 
q.alpha_m 
FROM "apogeeStar" as a 
JOIN "apogeeObject" as o ON a.apogee_id=o.apogee_id 
JOIN "aspcapStar" as q ON a.apstar_id=q.apstar_id
WHERE glon > 92.31 and glat < 120.3

--3426ms