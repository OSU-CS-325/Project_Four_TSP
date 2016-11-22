#!/bin/bash

clear
ALG="braindeadTour"

INSTANCE="example"
for NUM in {1..3}
do
	INFIL=tsp_${INSTANCE}_${NUM}.txt
	if [ -f $INFIL ]
		then
		echo "running instance $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python ${ALG}.py $INFIL
		echo "verifying results of $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python ../TSP_Files/tsp-verifier.py $INFIL ${INFIL}.tour
		echo "---------------------------------------------------------------"
	fi
done

INSTANCE="competition"
for NUM in {1..7}
do
	INFIL=tsp_${INSTANCE}_${NUM}.txt
	if [ -f $INFIL ]
		then
		echo "running instance $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python ${ALG}.py $INFIL
		echo "verifying results of $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python ../TSP_Files/tsp-verifier.py $INFIL ${INFIL}.tour
		echo "---------------------------------------------------------------"
	fi
done
