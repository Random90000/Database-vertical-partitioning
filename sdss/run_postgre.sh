#!/bin/bash
#set -x verbose #echo on

UID_ROOT=0
DIR=/tmp/pg_cluster
LOG=/tmp/serverlog
POSTGRE=/usr/lib/postgresql/11
USERNAME=alexandr
DBNAME=sdss_extract
 
if [ "$UID" -ne "$UID_ROOT" ]; then
  echo "::=> Requires root privileges"
  exit 1
fi

if [ -d "$DIR" ]; then
  echo "::=> Directory $DIR already exist. Seems like postgre is already running."
  su - postgres -c "$POSTGRE/bin/pg_ctl stop -D $DIR/db"
  rm -rf $DIR/db
  rm -rf $LOG
else
  mkdir $DIR
  mount none -t tmpfs $DIR
fi

su - postgres -c"
cd $DIR
mkdir db
$POSTGRE/bin/initdb -D $DIR/db
"

/etc/init.d/postgresql stop
cp postgresql.conf $DIR/db

su - postgres -c"
$POSTGRE/bin/pg_ctl start -D $DIR/db -l $LOG -s
createuser $USERNAME
createdb $DBNAME"
sudo -u postgres psql -d sdss_extract -c"CREATE EXTENSION cube;"
