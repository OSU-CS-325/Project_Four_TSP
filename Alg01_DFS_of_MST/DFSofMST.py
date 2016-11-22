# Program:              braindeadTour.py
# Author:               OSU Fa16 CS325-400 Project Group 12
# Description:          Really bad TSP algorithm.  No logic whatsoever
#                       other than to go to each destination in the order
#                       originally provided.
# Usage:                From the command line type the following...
#                       /usr/bin/python braindeadTour.py inputFileName.txt

import math
import os
import sys

# -----------------------------------------------------------------------------	
def dist(cityOne, cityTwo):
 	dx = cityOne['x'] - cityTwo['x']
	dy = cityOne['y'] - cityTwo['y']
	dxSq = math.pow(dx, 2)
	dySq = math.pow(dy, 2)
	return int(round(math.sqrt(dxSq + dySq)))

# -----------------------------------------------------------------------------	
def main():
	# get input file name from command line
	if (len(sys.argv) != 2):
		print 'ERROR: Exactly one argument expected.  See usage instructions.\n'
		quit()
	else:
		inFil = sys.argv[1]
        if not(os.path.isfile(inFil)):
			print 'ERROR: File \'' + str(inFil) + '\' not found.\n'
			quit()

	# open input/output files
	base = os.path.basename(inFil)
	inFil = open(base, 'r')
	outFil = open(base + '.tour', 'w')

	# get cities from input file into a list
	cities = []
	for eachLine in inFil:
		eachCity = eachLine.split()
		thisCity = {'id':int(eachCity[0]), 'x':int(eachCity[1]), 'y':int(eachCity[2])}
		cities.append(thisCity)

	# calc the total distance from city 0 to 1 to 2 to n-1 to n to 0
	totalDist = 0
	iterCities = iter(cities)
	prevCity = cities[0]
	next(iterCities) # skip the very first city
	for eachCity in iterCities:
		# get distance to eachCity from the prevCity
		totalDist = totalDist + dist(eachCity, prevCity) 
		prevCity = eachCity
	# get distance from last city back to first city
	totalDist = totalDist + dist(prevCity, cities[0])

	# write output to file
	outFil.write(str(totalDist) + '\n')
	iterCities = iter(cities)
	for eachCity in iterCities:
		outFil.write(str(eachCity['id']) + '\n')

	inFil.close()
	outFil.close()

# -----------------------------------------------------------------------------	
if __name__ == "__main__":
	main()
