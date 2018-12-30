SELECT
  ap.field, ap.dec, ap.commiss,
  ap.ra, ap.dec,
  ap.apstar_id, ap.apogee_id, ap.ra, ap.dec,
  asp.teff,
  asp.logg,
  asp.fe_h,
  asp.fe_h_err,
  asp.fe_h_flag,
  asp.c_fe,
  asp.c_fe_err,
  asp.c_fe_flag,
  asp.n_fe,
  asp.n_fe_err,
  asp.n_fe_flag,
  asp.o_fe,
  asp.o_fe_err,
  asp.o_fe_flag
FROM "aspcapStar" as asp 
 JOIN "apogeeStar" AS ap ON ap.apstar_id = asp.apstar_id
WHERE
 ap.verr-ap.verr_med >= 0.2
 AND ap.verr BETWEEN 12 AND 18
 AND ap.verr_med BETWEEN 12 AND 18
 AND ap.snr BETWEEN 4500 AND 6050
 AND ap.glat BETWEEN 1.8 AND 2.9
 AND asp.teff > 0  
 AND SQRT( (t.teffadop-asp.teff)*(t.teffadop-asp.teff) ) < 500
 AND SQRT( (t.loggadop-asp.logg)*(t.loggadop-asp.logg) ) < 0.25
ORDER BY ap.field,ap.dec,ap.commiss

--34ms