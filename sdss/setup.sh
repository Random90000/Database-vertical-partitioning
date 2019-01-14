#!/bin/bash
DB_NAME="sdss_extract"
DIR=$(dirname "$(realpath setup.sh)")'/star.csv'
sudo -u postgres createdb $DB_NAME
sudo -u postgres psql -f createStarTable.sql $DB_NAME  
sudo -u postgres psql -d $DB_NAME -c "COPY star FROM '$DIR' WITH csv header delimiter ',';"
