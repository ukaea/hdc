#!/bin/bash
PREFIX="test-hdc"
SERVER="dockerhub.tok.ipp.cas.cz:5000"
for FILENAME in Dockerfile.*; do
    SYSNAME=${FILENAME/Dockerfile./}
    DOCKERNAME="$PREFIX-$SYSNAME"
    REMOTEDOCKERNAME="$SERVER/$DOCKERNAME"
    #echo $FILENAME $SYSNAME $DOCKERNAME $REMOTEDOCKERNAME
    docker build -t $DOCKERNAME -f $FILENAME .
    docker tag $DOCKERNAME $REMOTEDOCKERNAME
    docker push $REMOTEDOCKERNAME
done
