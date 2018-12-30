#!/bin/bash
DB_NAME="sdss_extract"
sudo -u postgres createdb $DB_NAME
sudo -u postgres psql -f createApogeeStarTable.sql $DB_NAME  
sudo -u postgres psql -f createAspcapStarTable.sql $DB_NAME
sudo -u postgres psql -d $DB_NAME -c "COPY \"apogeeStar\" FROM '/home/alexandr/Projects/dbdesign/source/sdss/apogeeStar.csv' WITH csv header delimiter ',';"
sudo -u postgres psql -d $DB_NAME -c "COPY \"aspcapStar\" FROM '/home/alexandr/Projects/dbdesign/source/sdss/aspcapStar.csv' WITH csv header delimiter ',';"
