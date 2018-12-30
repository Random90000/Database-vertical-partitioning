select ph.ra, ph.dec, 
ph."psfMag_u",ph."psfMag_g",ph."psfMag_r",ph."psfMag_i",ph."psfMag_z",
  ph."psfMagErr_u",ph."psfMagErr_g",ph."psfMagErr_r",ph."psfMagErr_i",ph."psfMagErr_z",
ph.obj, ph.type,ph."probPSF",
pm."pmRa", pm."pmDec", pm."pmRaErr", pm."pmDecErr"
from "photoObj" as ph, "properMotions" as pm
where ph."objID" = pm."objid" 
  AND ph."psfMag_g" - ph."psfMag_r" > 1.5
  AND ph."psfMag_g" - ph."psfMag_r" < 2.0
  and ph."psfMag_r" - ph."psfMag_z" > 1.0
  AND ph."psfMag_r" - ph."psfMag_z" < 1.6
  AND ph."psfMag_r" - ph."psfMag_z" < 1.2*(ph."psfMag_g" - ph."psfMag_r") - 0.6
  and ph."psfMag_r" > 18.0 
  AND ph."psfMag_r" < 20.0 
  AND ph."psfMagErr_r" < 0.05
  AND ph."psfMagErr_i" < 0.05
  AND sqrt(pm."pmRa" * pm."pmRa" + pm."pmDec" * pm."pmDec") > 30.0 
  ANd sqrt(pm."pmRaErr" * pm."pmRaErr" + pm."pmDecErr" * pm."pmDecErr") < 15.0


--3764ms