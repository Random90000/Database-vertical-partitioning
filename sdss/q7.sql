SELECT star.objid FROM star WHERE ((star.u - star.g) < 0.4 ) and (star.g - star.r) < 0.7 and (star.r - star.i) > 0.4 and (star.i - star.z) > 0.4;
