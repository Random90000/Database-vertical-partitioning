SELECT
p."objID", p.ra, p.dec
FROM "photoObj" as p
WHERE
(sqrt(power(ra-23.468978136, 2) +power(dec-13.702647828,2)) < 1.388889e-04)
OR (sqrt(power(ra-180.342951311, 2) +power(dec-2.185645702,2)) < 1.388889e-04)
OR (sqrt(power(ra-126.481336165, 2) +power(dec-35.542211224,2)) < 1.388889e-04)
OR (sqrt(power(ra-143.509956085, 2) +power(dec-55.239761906,2)) < 1.388889e-04)
OR (sqrt(power(ra-155.438010787, 2) +power(dec-18.088023583,2)) < 1.388889e-04)
OR (sqrt(power(ra-339.629660191, 2) +power(dec-14.008287319,2)) < 1.388889e-04)
OR (sqrt(power(ra-161.240820366, 2) +power(dec-3.88698525,2)) < 1.388889e-04)
OR (sqrt(power(ra-208.856941113, 2) +power(dec-46.86426591,2)) < 1.388889e-04)
OR (sqrt(power(ra-29.5391667, 2) +power(dec+0.1105556,2)) < 1.388889e-04)
OR (sqrt(power(ra-146.0695833, 2) +power(dec-54.1925,2)) < 1.388889e-04)
OR (sqrt(power(ra-180.51, 2) +power(dec-54.2641667,2)) < 1.388889e-04)
OR (sqrt(power(ra-214.2570833, 2) +power(dec-43.5036111,2)) < 1.388889e-04)


--588ms