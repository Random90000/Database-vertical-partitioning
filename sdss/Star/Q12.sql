SELECT
  round( p.ra ) AS ra
, round( p.dec ) as "dec"
, p.run
, p.rerun
, p.camcol
, p.field
, p.obj
, round( p.colc ) as colc
, round( p.rowc ) as rowc
, round( p."extinction_r"::numeric,3 ) as rExt
, round( p."psfMag_u"::numeric,3 ) as upsf
, round( p."psfMag_g"::numeric,3 ) as gpsf
, round( p."psfMag_r"::numeric,3 ) as rpsf
, round( p."psfMag_i"::numeric,3 ) as ipsf
, round( p."psfMag_z"::numeric,3 ) as zpsf
, round( p."psfMagErr_u"::numeric,3 ) as uErr
, round( p."psfMagErr_g"::numeric,3 ) as gErr
, round( p."psfMagErr_r"::numeric,3 ) as rErr
, round( p."psfMagErr_i"::numeric,3 ) as iErr
, round( p."psfMagErr_z"::numeric,3 ) as zErr
, to_hex(p.flags_u)::INT as flags1_u
, to_hex((p.flags_u / 256 / 256 / 256 / 256))::INT as flags2_u
, to_hex(p.flags_g)::INT as flags1_g
, to_hex(p.flags_g / 256 / 256 / 256 / 256)::INT as flags2_g
, to_hex(p.flags_r)::INT as flags1_r
, to_hex(p.flags_r / 256 / 256 / 256 / 256)::INT as flags2_r
, to_hex(p.flags_i)::INT as flags1_i
, to_hex(p.flags_i / 256 / 256 / 256 / 256)::INT as flags2_i
, to_hex(p.flags_z)::INT as flags1_z
, to_hex(p.flags_z / 256 / 256 / 256 / 256)::int as flags2_z
, s.match
, round( s."pmL"::NUMERIC, 3 ) as pmL
, round( s."pmB"::NUMERIC, 3 ) as pmB
, "pmRaErr"
, p."specObjID"
FROM "Star" p
LEFT JOIN "properMotions" s
 ON ( p."objID" = s."objid" and s."sigRa" < 350 and s."sigDec" < 350 )
WHERE p."psfMag_r" < 22.5
AND p.ra > 228.5963333
AND p.ra < 229.4296667
AND p.dec > - 0.5396666667
AND p.dec <= 0.2936666667


--209ms