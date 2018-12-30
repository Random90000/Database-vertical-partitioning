SELECT s1.ra as RA, s1.dec as DEC, s1.u as U, s1.g as G, s1.r as R, s1.i as I, s1.z as Z, s1."dered_u" as DRU, s1."dered_g" as DRG, s1."dered_r" as DRR, s1."dered_i" as DRI, s1."dered_z" as DRZ
FROM "photoObj" s1
WHERE 
s1.r between 14.1 and 22.2
OR s1.i between 13.8 and 21.3
OR s1.z between 12.3 and 20.5
AND ABS(s1.r - s1.dered_r) < 0.05
AND (s1.r-s1.i) between 0.48 and 2.90
AND (s1.i-s1.z) between 0.27 and 1.89
AND (5.190 +2.474*(s1.dered_r-s1.dered_z) + 0.4340*(s1.dered_r-s1.dered_z)*(s1.dered_r-s1.dered_z) - 0.08635*(s1.dered_r-s1.dered_z)*(s1.dered_r-s1.dered_z)*(s1.dered_r-s1.dered_z)) > 14 limit 1000000

--1456ms

select
p."psfMag_u",p.psfmagerr_u,p."psfMag_g",p.psfmagerr_g,p.psfmag_r,psfmagerr_r,p.psfmag_i,psfmagerr_i,p.psfmag_z,psfmagerr_z,p.ra,p.dec
INTO g_19_WDMS_photo_err
FROM PhotoObjAll AS p
WHERE p.type=6
and p.clean = 1
and p."psfMag_u"-"psfMag_g" > -0.6
and p."psfMag_u"-p."psfMag_g" < 0.93 - 0.27*(p."psfMag_g"-p.psfmag_r)-4.7*power(p."psfMag_g"-p.psfmag_r,2.0)+12.38*power(p."psfMag_g"-p.psfmag_r,3.0)+3.08*power(p."psfMag_g"-p.psfmag_r,4.0)-22.19*power(p."psfMag_g"-p.psfmag_r,5.0)+16.67*power(p."psfMag_g"-p.psfmag_r,6.0)-3.89*power(p."psfMag_g"-p.psfmag_r,7.0)
and p."psfMag_g"-p.psfmag_r > -0.5
and p."psfMag_g"-p.psfmag_r < 1.3
and p."psfMag_g" >= 15
and p."psfMag_g" <= 19
and p.psfmag_r-p.psfmag_i > -0.4
and p.psfmag_r-p.psfmag_i < 1.6
and (((p."psfMag_g"-p.psfmag_r < 2*(p.psfmag_r-p.psfmag_i)+0.38) and (p.psfmag_r-p.psfmag_i > -0.4) and (p.psfmag_r-p.psfmag_i <=0.1)) 
or ((p."psfMag_g"-p.psfmag_r < 0.5) and (p.psfmag_r-p.psfmag_i >0.1) and (p.psfmag_r-p.psfmag_i<=0.3))
or ((p."psfMag_g"-p.psfmag_r < 4.5*(p.psfmag_r-p.psfmag_i)-0.85) and (p.psfmag_r-p.psfmag_i >0.3) and (p.psfmag_r-p.psfmag_i<=0.5))
or ((p."psfMag_g"-p.psfmag_r <0.25*(p.psfmag_r-p.psfmag_i)+1.3) and (p.psfmag_r-p.psfmag_i >0.5) and (p.psfmag_r-p.psfmag_i <=1.6)))
and p.psfmag_i-p.psfmag_z < 1.15
and p.psfmag_i-p.psfmag_z > -0.8
and ((((p.psfmag_r-p.psfmag_i) < 0.5 + (p.psfmag_i-p.psfmag_z)) and (p.psfmag_i-p.psfmag_z <= 0))
or (((p.psfmag_r-p.psfmag_i) < 0.5 + 2*(p.psfmag_i-p.psfmag_z)) and (p.psfmag_i-p.psfmag_z > 0)))
and ((flags & 0x10000000) != 0)
and ((flags & 0x8100000c00ac) = 0)
and p.mode = 1