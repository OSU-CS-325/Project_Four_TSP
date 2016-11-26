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
	# outFil = open(base + '.tour', 'w')

	# get cities from input file into a list
	cities = []
	for eachLine in inFil:
		eachCity = eachLine.split()
		thisCity = {'id':int(eachCity[0]), 'x':int(eachCity[1]), 'y':int(eachCity[2])}
		cities.append(thisCity)
#		print(thisCity)
	inFil.close()

	# init adjacency matrix for graph (every city connected to every other city)
	adjMatrix = [[-1 for x in range(len(cities))] for y in range(len(cities))]
	
	# look for tour starting at each possible vertex
	minTourDist = sys.maxsize
	minTourOrder = []
	for i in range(0, len(cities)):
#		print("starting at: " + str(i))
		tourCities = [x for x in cities]
		tourOrder = []
		tourOrder.append(tourCities[i]['id'])
#		print("tourOrder: " + str(tourOrder))
		tourCities.remove(tourCities[i])
		tourDist = 0
		while len(tourCities) > 0:
#			if len(tourCities) % 1000 == 0:
#				print (str(len(tourCities)))
			# find closest city
			thisCity = cities[tourOrder[len(tourOrder) - 1]]
#			print("thisCity: " + str(thisCity))
			minDist = sys.maxsize
			minCity = -1
#			print("tourCities: " + str(tourCities))
			for j in range(0, len(tourCities)):
				thisDist = adjMatrix[thisCity['id']][tourCities[j]['id']]
#				print("thisDist: " + str(thisDist))
				if thisDist == -1:
					thisDist = dist(thisCity, tourCities[j])
#					print("thisDist: " + str(thisDist))
					adjMatrix[thisCity['id']][tourCities[j]['id']] = thisDist
					adjMatrix[tourCities[j]['id']][thisCity['id']] = thisDist
				if thisDist < minDist:
					minDist = thisDist
					minCity = tourCities[j]
			tourOrder.append(minCity['id'])
#			print("minDist: " + str(minDist))
#			print("tourOrder: " + str(tourOrder))
			tourCities.remove(minCity)
			tourDist = tourDist + minDist

		u = cities[tourOrder[0]]['id']
		v = cities[tourOrder[len(tourOrder) - 1]]['id']
		thisDist = adjMatrix[u][v]
		if thisDist == -1:
			thisDist = dist(cities[tourOrder[0]], cities[tourOrder[len(tourOrder) - 1]])
			adjMatrix[u][v] = thisDist
			adjMatrix[v][u] = thisDist
			
		tourDist = tourDist + thisDist
#		print("tourOrder: " + str(tourOrder))
#		print("starting at: " + str(i) + " thisDist:" + str(tourDist))
		if tourDist < minTourDist:
			print("starting at: " + str(i) + " thisDist:" + str(tourDist))
			minTourDist = tourDist
			minTourOrder = [x for x in tourOrder]

			# write output to file
			outFil = open(base + '.tour', 'w')
			outFil.write(str(minTourDist) + '\n')
			iterCities = iter(minTourOrder)
			for eachCity in iterCities:
				outFil.write(str(eachCity) + '\n')
			outFil.close()
		else:
			print("starting at: " + str(i))

# -----------------------------------------------------------------------------	
if __name__ == "__main__":
	main()
