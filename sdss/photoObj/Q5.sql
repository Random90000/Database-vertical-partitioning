SELECT  
        (p."sky_u"*power(10.0,14.0)) as sky_u,
        (p."sky_g"*power(10.0,14.0)) as sky_g,
        (p."sky_r"*power(10.0,14.0)) as sky_r,
        (p."sky_i"*power(10.0,14.0)) as sky_i,
        (p."sky_z"*power(10.0,14.0)) as sky_z,
        (p."skyErr_u"*power(10.0,14.0)) as skyErr_u,
        (p."skyErr_g"*power(10.0,14.0)) as skyErr_g,
        (p."skyErr_r"*power(10.0,14.0)) as skyErr_r,
        (p."skyErr_i"*power(10.0,14.0)) as skyErr_i,
        (p."skyErr_z"*power(10.0,14.0)) as skyErr_z
FROM "photoObj" AS p
ORDER by p."ra"

--2338ms