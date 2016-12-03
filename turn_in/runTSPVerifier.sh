#!/bin/bash

clear

INSTANCE="example"
for NUM in {1..3}
do
	INFIL=tsp_${INSTANCE}_${NUM}.txt
	/usr/bin/python3 ./tsp-verifier.py $INFIL ${INFIL}.tour
	echo '-----------------------------------------------------------'
done

INSTANCE="test-input-"
for NUM in {1..7}
do
	INFIL=${INSTANCE}${NUM}.txt
	/usr/bin/python3 ./tsp-verifier.py $INFIL ${INFIL}.tour
	echo '-----------------------------------------------------------'
done
