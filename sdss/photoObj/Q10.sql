SELECT
    p."objID", p."SpecObjID", p.ra, p.dec, pm."pmRa", pm."pmRaErr", pm."pmDec", pm."pmDecErr", a.vhelio_avg, 
    a.verr, p."htmID"
   from "photoObj" AS p
   JOIN "properMotions" as pm ON p."objID" = pm."objid"
   JOIN "apogeeStar" as a ON p."htmID" = a."htmID"
WHERE
  (p.mode = 1)
  AND sqrt(power(abs(pm."pmRa")+pm."pmRaErr",2)+power(abs(pm."pmDec")+pm."pmDecErr",2))/abs(a.vhelio_avg)<0.02
  AND pm."pmRa"!=0
  AND pm."pmDec"!=0

--893ms