BEGIN;

-- CREATE TABLE "properMotions" --------------------------------
CREATE TABLE "public"."properMotions" (  
"delta"	REAL,
"match"	INT,
"pmL"	REAL,
"pmB"	REAL,
"pmRa"	REAL,
"pmDec"	REAL,
"pmRaErr"	REAL,
"pmDecErr"	REAL,
"sigRa"	REAL,
"sigDec"	REAL,
"nFit"	INT,
"O"	REAL,
"E"	REAL,
"J"	REAL,
"F"	REAL,
"N"	REAL,
"dist20"	REAL,
"dist22"	REAL,
"objid"	BIGINT
);
 ;
-- -------------------------------------------------------------

COMMIT;
