SELECT
  p."objID"
, star."apstar_id"
, p.ra
, p.dec
, star.glon as l
, star.glat as b
, p.u
, p.g
, p.r
, p.i
, p.z
, p."Err_u"
, p."Err_g"
, p."Err_r"
, p."Err_i"
, p."Err_z"
, p."extinction_u"
, p."extinction_g"
, p."extinction_r"
, p."extinction_i"
, p."extinction_z"
, apog.j
, apog.h
, apog.h
, apog."j_err"
, apog."h_err"
, apog."k_err"
, apog.pmra
, apog.pmdec
, aspcap.teff
, aspcap.logg
, aspcap.teff_err
, aspcap.logg_err
, apog.pmra_err
, apog.pmdec_err
, aspcap.teff_flag
, aspcap.logg_flag
, aspcap."param_n_m"
, aspcap.param_n_m_flag
, aspcap.param_c_m
, aspcap.param_c_m_flag
, aspcap.fe_h
, aspcap.fe_h_err
, aspcap.fe_h_flag
, aspcap.o_fe
, aspcap.o_fe_err
, aspcap.o_fe_flag
, aspcap.si_fe
, aspcap.si_fe_err
, aspcap.si_fe_flag
, aspcap.mg_fe
, aspcap.mg_fe_err
, aspcap.mg_fe_flag
, star.vhelio_avg
, star.verr
, star.snr
FROM "apogeeStar" AS star 
JOIN "photoObj" as p on star."apstar_id" = p."objID"::CHAR
JOIN "aspcapStar" as aspcap
 ON star.apstar_id = aspcap.apstar_id
JOIN "apogeeObject" as apog
 ON star.apogee_id = apog.apogee_id

 --1098