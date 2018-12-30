select count(*) as N
from "Star"
where
ra between 129 and 131 and
dec between 19 and 21 and
-log(0.25*exp(-"modelMag_u")+0.5*exp(-"modelMag_g")+0.25*exp(-"modelMag_r")) < 13.0

--240ms