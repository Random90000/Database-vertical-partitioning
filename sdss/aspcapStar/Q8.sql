SELECT s."apstar_id", s."target_id", s."apogee_id", s.ra, s.dec, s.glon, s.glat, 
  s.snr, s."vhelio_avg", s.vscatter, s.verr, s."verr_med", o.j, o."j_err", o.k, o."k_err", 
  a."alpha_m", a."alpha_m_err", a."fe_h", a."fe_h_err", a."m_h", a."m_h_err", a.teff, a."teff_err" from "apogeeStar" as s, "apogeeObject" as o, "aspcapStar" as a 
WHERE (s.glon < 20 or 340 < s.glon) and s.glat < 20 and s.glat > -20 
and o."apogee_id" = s."apogee_id"
 and o."apogee_id" = s."apogee_id"


 --12555ms