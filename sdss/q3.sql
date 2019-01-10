select count(*) from star where star.ra between 129 and 131 and star.dec between 19 and 21 and -log(0.25*exp(-star.modelmag_u)+0.5*exp(-star.modelmag_g)+0.25*exp(-star.modelmag_r)) < 13.0;
