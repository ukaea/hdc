#!/bin/bash
PREFIX="test-hdc"
SERVER="repo.tok.ipp.cas.cz:5050/compass-tokamak/hdc"
docker login $SERVER
for FILENAME in Dockerfile.*; do
    SYSNAME=${FILENAME/Dockerfile./}
    DOCKERNAME="$PREFIX-$SYSNAME"
    REMOTEDOCKERNAME="$SERVER/$DOCKERNAME"
    docker build -t $REMOTEDOCKERNAME -f $FILENAME .
    docker push $REMOTEDOCKERNAME
done
