SELECT 
a.apogee_id,
to_char(a.glon,'999D9') as glon, 
to_char(a.glat,'999D9') as glat,
a.snr,
a.vhelio_avg,
a.vscatter,
q.teff,
q.logg,
q.m_h,
q.alpha_m 
FROM "aspcapStar" as q
JOIN "apogeeObject" as o ON q.apstar_id=o.apogee_id 
JOIN "apogeeStar" as a ON a.apstar_id=q.apstar_id 
WHERE q.teff > 4000 AND q.teff < 4600 AND q.m_h < -1 AND q.logg > 2

--164ms