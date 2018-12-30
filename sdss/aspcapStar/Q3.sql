SELECT 
s.apogee_id, 
s.ra, 
s.dec,
s.glon, 
s.glat, 
s.vhelio_avg,
s.vscatter,
a.teff, 
a.logg, 
a.m_h, 
a.alpha_m
FROM "aspcapStar" a
JOIN "apogeeStar" s on a.apstar_id = s.apstar_id
WHERE a.m_h < 10
and a.teff>10
and a.logg>-100
and a.m_h>-100
and a.alpha_m>-100

--983ms