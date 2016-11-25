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
def nearestNeighbor(adjMatrix, start):
	vstd = []
	vstd.append(start)
#	print (vstd)
	u = start

	while len(vstd) < len(adjMatrix):
		dist = sys.maxsize
		next = -1
		for v in range(len(adjMatrix)):
			if adjMatrix[u][v] < dist and not v in vstd:
				next = v
				dist = adjMatrix[u][v]
		vstd.append(next)
#		print (vstd)
		u = next
		
	return vstd 

# -----------------------------------------------------------------------------	
def main():
	# get input file name from command line
	if (len(sys.argv) != 2):
		print ('ERROR: Exactly one argument expected.  See usage instructions.\n')
		quit()
	else:
		inFil = sys.argv[1]
		if not(os.path.isfile(inFil)):
			print ('ERROR: File \'' + str(inFil) + '\' not found.\n')
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
#		print(thisCity)

	# init adjacency matrix for graph (every city connected to every other city)
	adjMatrix = [[0 for x in range(len(cities))] for y in range(len(cities))]
	for i in range(0, len(cities)):
		for j in range(i + 1, len(cities)):
			ij = dist(cities[i], cities[j])
			adjMatrix[i][j] = ij
			adjMatrix[j][i] = ij
#		print (adjMatrix[i])
	
	# get tour order
	minDist = sys.maxsize
	minTour = []
	for i in range(0, len(adjMatrix)):
		tourAdjMatrix = [x[:] for x in adjMatrix]
		tour = nearestNeighbor(tourAdjMatrix, i)
#		print (tour)	

	 	# calc the total distance from city 0 to 1 to 2 to n-1 to n to 0
		totalDist = 0
		iterCities = iter(tour)
		prevCity = cities[tour[0]]
		next(iterCities) # skip the very first city
		for eachItem in iterCities:
			eachCity = cities[eachItem]
			# get distance to eachCity from the prevCity
#			print (eachCity)
#			print (prevCity)
			addDist = dist(eachCity, prevCity)
			totalDist = totalDist + addDist 
#			print ("%s: %s" % (addDist, totalDist))
			prevCity = eachCity
		# get distance from last city back to first city
		addDist = dist(prevCity, cities[tour[0]])
		totalDist = totalDist + addDist 
#		print ("%s: %s" % (addDist, totalDist))
#		print (totalDist)
		if totalDist < minDist:
			minTour = []
			minDist = totalDist
			for j in range(0, len(adjMatrix)):
				minTour.append(tour[j])

	# write output to file
	outFil.write(str(minDist) + '\n')
	iterCities = iter(minTour)
	for eachCity in iterCities:
		outFil.write(str(eachCity) + '\n')

	inFil.close()
	outFil.close()

# -----------------------------------------------------------------------------	
if __name__ == "__main__":
	main()
