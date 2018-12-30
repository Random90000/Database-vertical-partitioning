select run,rerun,camcol,field,obj,ra,"dec",
		"psfMag_u","psfMag_g","psfMag_r","psfMag_i","psfMag_z","extinction_u",
		"extinction_g","extinction_r","extinction_i","extinction_z" from "Star" 
	    where ra between 352.529-0.2/cos(49.13*3.14159/180) and 352.529+0.2/cos(49.13*3.14159/180) 
		and "dec" between 49.13-0.2 and 49.13+0.2
		and "psfMag_g" < 22.2 and "psfMag_r" <22.2

--233ms