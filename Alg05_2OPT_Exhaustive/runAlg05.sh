#!/bin/bash

clear
ALG="tsp_2opt"

INSTANCE="example"
for NUM in {0..0}
do
	INFIL=tsp_${INSTANCE}_${NUM}.txt
	if [ -f $INFIL ]
		then
		echo "running instance $INSTANCE #$NUM with the $ALG algorithm..."
		START=$(date +%s.%N)
		./${ALG} $INFIL
		END=$(date +%s.%N)
		DT=$(echo "$END - $START" | bc)
		DD=$(echo "$DT/86400" | bc)
		DT=$(echo "$DT-86400*$DD" | bc)
		DH=$(echo "$DT/3600" | bc)
		DT=$(echo "$DT-3600*$DH" | bc)
		DM=$(echo "$DT/60" | bc)
		DS=$(echo "$DT-60*$DM" | bc)
		printf "Total runtime: %d:%02d:%02d:%02.4f\n" $DD $DH $DM $DS
		echo "verifying results of $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python3 ../TSP_Files/tsp-verifier.py $INFIL ${INFIL}.tour
		echo "---------------------------------------------------------------"
	fi
done

INSTANCE="test-input-"
for NUM in {7..7}
do
	INFIL=${INSTANCE}${NUM}.txt
	if [ -f $INFIL ]
		then
		echo "running instance $INSTANCE #$NUM with the $ALG algorithm..."
		START=$(date +%s.%N)
		./${ALG} $INFIL
		END=$(date +%s.%N)
		DT=$(echo "$END - $START" | bc)
		DD=$(echo "$DT/86400" | bc)
		DT=$(echo "$DT-86400*$DD" | bc)
		DH=$(echo "$DT/3600" | bc)
		DT=$(echo "$DT-3600*$DH" | bc)
		DM=$(echo "$DT/60" | bc)
		DS=$(echo "$DT-60*$DM" | bc)
		printf "Total runtime: %d:%02d:%02d:%02.4f\n" $DD $DH $DM $DS
		echo "verifying results of $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python3 ../TSP_Files/tsp-verifier.py $INFIL ${INFIL}.tour
		echo "---------------------------------------------------------------"
	fi
done

INSTANCE="test-input-"
for NUM in {0..0}
do
	INFIL=${INSTANCE}${NUM}.txt
	if [ -f $INFIL ]
		then
		echo "running instance $INSTANCE #$NUM with the $ALG algorithm..."
		START=$(date +%s.%N)
		../TSP_Files/watch.py ./${ALG} $INFIL
		END=$(date +%s.%N)
		DT=$(echo "$END - $START" | bc)
		DD=$(echo "$DT/86400" | bc)
		DT=$(echo "$DT-86400*$DD" | bc)
		DH=$(echo "$DT/3600" | bc)
		DT=$(echo "$DT-3600*$DH" | bc)
		DM=$(echo "$DT/60" | bc)
		DS=$(echo "$DT-60*$DM" | bc)
		printf "Total runtime: %d:%02d:%02d:%02.4f\n" $DD $DH $DM $DS
		echo "verifying results of $INSTANCE #$NUM with the $ALG algorithm..."
		/usr/bin/python3 ../TSP_Files/tsp-verifier.py $INFIL ${INFIL}.tour
		echo "---------------------------------------------------------------"
	fi
done
