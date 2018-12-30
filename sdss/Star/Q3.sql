SELECT
  run
, rerun
, camcol
, field
, obj
, colc
, rowc
, "parentID"
, "nChild"
, ra
, "dec"
, extinction_r
, "psfMag_u"
, "psfMag_g"
, "psfMag_r"
, "psfMag_i"
, "psfMag_z"
, "psfMagErr_u"
, "psfMagErr_g"
, "psfMagErr_r"
, "psfMagErr_i"
, "psfMagErr_z"
, rowv
, colv
, "rowvErr"
, "colvErr"
, "rowc_u"
, "colc_u"
, "rowc_g"
, "colc_g"
, "rowc_r"
, "colc_r"
, "rowc_i"
, "colc_i"
, "rowc_z"
, "colc_z"
, "offsetRa_u"
, "offsetDec_u"
, "offsetRa_g"
, "offsetDec_g"
, "offsetRa_r"
, "offsetDec_r"
, "offsetRa_i"
, "offsetDec_i"
, "offsetRa_z"
, "offsetDec_z"
FROM "Star"
WHERE "nChild" = 0
AND "psfMag_u" > 0
AND "psfMag_u" < 21
AND "psfMag_g" > 0
AND "psfMag_g" < 21
AND abs("psfMagErr_u") <= 0.1
AND abs("psfMagErr_r") <= 0.1
AND abs("psfMagErr_g") <= 0.05
AND abs("psfMagErr_i") <= 0.05
AND abs("psfMagErr_z") <= 0.05
AND sqrt( ("offsetRa_u" - "offsetRa_z") * ("offsetRa_u" - "offsetRa_z") * cos("dec" * 0.01745 ) * cos("dec" * 0.01745 ) + ("offsetDec_u" - "offsetDec_z") * ("offsetDec_u" - "offsetDec_z") ) >= 0.5 --220ms