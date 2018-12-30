SELECT ra, "dec", "psfMag_g" as g, "psfMag_g" - "psfMag_r" as g_r, "skyIvar_g", "skyIvar_i"
from "Star"
where ra between 140 and 141
and "dec" between 20 and 21
and "psfMag_g" - "psfMag_r" between 0.2 and 0.8
and "psfMag_u" < 16

--233ms