#!/bin/bash

queries=(
'SELECT star_test_1.objid, star_test_2.ra, star_test_1.dec, star_test_1.u,star_test_1.g,star_test_1.r, star_test_1.i, star_test_1.z FROM star_test_1 INNER JOIN star_test_2 ON star_test_1.objid = star_test_2.objid WHERE star_test_1.g < 21.2;'
'SELECT count(*) FROM star_test_1, star_test_2, star_test_3 WHERE (star_test_2.ra BETWEEN 129 AND 131 AND star_test_1.dec BETWEEN 19 AND 21 AND -log(0.25*exp(-star_test_3.modelmag_u)+0.5*exp(-star_test_3.modelmag_g)+0.25*exp(-star_test_3.modelmag_r)) < 13.0) AND (star_test_1.objid = star_test_3.objid) AND (star_test_1.objid = star_test_2.objid);'
'SELECT star_test_2.ra, star_test_1.dec, star_test_1.skyivar_g, star_test_1.u, star_test_1.g, star_test_1.r, star_test_1.i, star_test_1.z FROM star_test_1 INNER JOIN star_test_2 ON star_test_1.objid = star_test_2.objid WHERE star_test_2.ra BETWEEN 140 AND 145 AND star_test_1.dec BETWEEN 20 and 25;'
'SELECT star_test_2.ra, star_test_1.dec, star_test_2.psfmag_g, star_test_2.psfmag_g - star_test_2.psfmag_r, star_test_1.skyivar_g, star_test_2.skyivar_i FROM star_test_1 , star_test_2 WHERE (star_test_2.ra BETWEEN 140 AND 141 AND star_test_1.dec BETWEEN 20 AND 21 AND star_test_2.psfmag_g - star_test_2.psfmag_r BETWEEN 0.2 AND 0.8 AND star_test_2.psfmag_u < 16) AND ( star_test_1.objid = star_test_2.objid );'
'SELECT star_test_2.ra FROM star_test_1 INNER JOIN star_test_2 ON star_test_1.objid = star_test_2.objid, star_test_3 WHERE (star_test_3.modelmag_r > 0 AND star_test_3.modelmag_r < 21 AND star_test_2.ra < 205.446972452536 AND star_test_2.ra > 203.2108728477 AND star_test_1.dec > 9.860958219299748 AND star_test_1.dec < 11.62881354954442 ) AND ( star_test_1.objid = star_test_3.objid ) ORDER BY star_test_3.modelmag_r LIMIT 999;'
'SELECT star_test_1.objid FROM star_test_1 WHERE ((star_test_1.u - star_test_1.g) < 0.4 ) AND (star_test_1.g - star_test_1.r) < 0.7 AND (star_test_1.r - star_test_1.i) > 0.4 AND (star_test_1.i - star_test_1.z) > 0.4;'
'SELECT star_test_2.ra, star_test_1.dec, star_test_2.psfmag_u, star_test_2.psfmagerr_u, star_test_2.psfmag_g, star_test_2.psfmagerr_g, star_test_2.psfmag_r, star_test_2.psfmagerr_r, star_test_2.psfmag_i, star_test_2.psfmagerr_i, star_test_2.psfmag_z, star_test_2.psfmagerr_z FROM star_test_1 , star_test_2 WHERE ((star_test_2.ra between 162.446533 and 162.646533) and (star_test_1.dec between -0.109789 and 0.090211)) and ( star_test_1.objid = star_test_2.objid );'
)

for q in ${!queries[*]}
do
    echo ${queries[$q]}"\n" >> res
    for i in {1..10}
    do
	 sudo -u postgres psql -d sdss_extract -c '\timing' -c "${queries[$q]}" | tail -1 >> res 
    done

done

