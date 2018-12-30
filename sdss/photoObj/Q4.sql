select 
     obj."objID" as "name"
   , ltrim(to_char( obj."ra", '999D9')) as ra
   , ltrim(to_char( obj."dec", '999D9')) as "dec"

   , ltrim(to_char( obj."petroRad_u", '999D9')) as opu
   , ltrim(to_char( obj."petroRad_g", '999D9')) as opg
   , ltrim(to_char( obj."petroRad_r", '999D9')) as opr
   , ltrim(to_char( obj."petroRad_i", '999D9')) as opi
   , ltrim(to_char( obj."petroRad_z", '999D9')) as opz

   , nobj."objID" as nbid
   , ltrim(to_char( 60*obj."distance", '999D9')) as dst
   , ltrim(to_char(degrees(atan2( nobj."dec" - obj."dec", - (nobj."ra" - obj."ra") )), '999D9')) as ang

   , ltrim(to_char( nobj."modelMag_u", '999D9')) as u
   , ltrim(to_char( nobj."modelMag_g", '999D9')) as g
   , ltrim(to_char( nobj."modelMag_r", '999D9')) as r
   , ltrim(to_char( nobj."modelMag_i", '999D9')) as i
   , ltrim(to_char( nobj."modelMag_z", '999D9')) as z

   , ltrim(to_char( nobj."petroRad_u", '999D9')) as pu
   , ltrim(to_char( nobj."petroRad_g",  '999D9')) as pg
   , ltrim(to_char( nobj."petroRad_r",  '999D9')) as pr
   , ltrim(to_char( nobj."petroRad_i",  '999D9')) as pi
   , ltrim(to_char( nobj."petroRad_z",  '999D9')) as pz
   from (
      select
           obj."objID"
         , "ra"
         , "dec"

         , NN."NeighborObjID"
         , NN.distance

         , "modelMag_u"
         , "modelMag_g"
         , "modelMag_r"
         , "modelMag_i"
         , "modelMag_z"

         , "petroRad_u"
         , "petroRad_g"
         , "petroRad_r"
         , "petroRad_i"
         , "petroRad_z"

      from "photoObj" as obj 
      join "neighbors" as NN on obj."objID" = NN."objID"
      where (
          ( 60*NN.distance between 0 and 30 )
      and ( NN."neighborMode" = 1 )
      and (
         ( obj."mode" = 1 ) 
         and ( obj."ra"  between 260 and 360 )
      )
      ) 
   ) as obj
   join "photoObj" as nobj on nobj."objID" = obj."objID"
   order by obj."ra", obj."dec", obj."distance"

--2240ms