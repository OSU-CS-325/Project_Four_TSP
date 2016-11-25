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
def popMin(prqu):
	minU = prqu[0]
	prqu.remove(prqu[0])
	return minU

# -----------------------------------------------------------------------------	
def decreaseKey(prqu, cost):
	for i in range(len(prqu)):
		for j in range(len(prqu)):
			if cost[prqu[i]] < cost[prqu[j]]:
				prqu[i] = prqu[i] + prqu[j]
				prqu[j] = prqu[i] - prqu[j]
				prqu[i] = prqu[i] - prqu[j]

# -----------------------------------------------------------------------------	
class edge:
	u = None # first vertex
	v = None # second vertex
	d = 0 # distance between vertices
	def __init__(self, u, v, d):
		self.u = u
		self.v = v
		self.d = d

def kruskalsAlg(adjMatrix):
	edges = []
	for i in range(0, len(adjMatrix) - 1):
		for j in range(i + 1, len(adjMatrix)):
			e = edge(i, j, adjMatrix[i][j])
			edges.append(e)		
	edges.sort(key = lambda x: x.d)
	
	prev = [None for x in range(len(adjMatrix))]
	vstd = []
	for e in edges:
#		print (str(e.d) + "," + str(e.u) + "," + str(e.v))
		if (not(e.v in vstd)):
			vstd.append(e.v)
#			print (vstd)
			prev[e.v] = e.u
#			print (prev)
	return prev

# -----------------------------------------------------------------------------	
def primsAlg(adjMatrix):
	cost = [sys.maxsize for x in range(len(adjMatrix))]
	prev = [None for x in range(len(adjMatrix))]
	prqu = [x for x in range(len(adjMatrix))]

	cost[0] = 0
	decreaseKey(prqu, cost)

	while len(prqu) > 0:
		u = popMin(prqu)

		for v in [v for v in range(len(adjMatrix)) if adjMatrix[u][v] > 0 and u != v]:
			dist = adjMatrix[u][v]
			if v in prqu and dist < cost[v]:
				prev[v] = u
				cost[v] = dist
				decreaseKey(prqu, cost)

	return prev

# -----------------------------------------------------------------------------	
def mstToAdjList(adjMatrix, mst):
	adjList = {}
	for i in range(0, len(adjMatrix)):
		adjV = {}
		for j in range(0, len(mst)):
			if i == mst[j]:
				adjV[j] = adjMatrix[i][j]
		adjList[i] = adjV
	return adjList

# -----------------------------------------------------------------------------	
def dfs(adjList):
	vstd = []
	stack = []
	stack.append(0)
#	print (stack)

	while(len(stack) > 0):
		u = stack[len(stack) - 1]
		stack.remove(stack[len(stack) - 1])
		if not (u in vstd):
			vstd.append(u)
			auxStack = []
			for eachAdj in sorted(adjList[u].items(), key=lambda x: x[1], reverse=True):
				v = eachAdj[0]
				if not (v in vstd):
					auxStack.append(v)
			while len(auxStack) > 0:
				v = auxStack[0]
				stack.append(v)
				auxStack.remove(v)
#			print (stack)

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
	
	# create minimum spanning tree (MST) using Prim's algorithm which is
	# faster on dense graphs than Kruskal's algorithm - I consider my graph
	# to be dense because every vertex (city) is connected to every other
#	mst = primsAlg(adjMatrix)
#	print (mst)
	mst = kruskalsAlg(adjMatrix)
#	print (mst)

	# convert mst into adjacencyList
	adjList = mstToAdjList(adjMatrix, mst)
#	print (adjList)

	# get DFS discovered order
	disc = dfs(adjList)
#	print (disc)	

	# calc the total distance from city 0 to 1 to 2 to n-1 to n to 0
	totalDist = 0
	iterCities = iter(disc)
	prevCity = cities[disc[0]]
	next(iterCities) # skip the very first city
	for eachItem in iterCities:
		eachCity = cities[eachItem]
		# get distance to eachCity from the prevCity
#		print (eachCity)
#		print (prevCity)
		addDist = dist(eachCity, prevCity)
		totalDist = totalDist + addDist 
#		print ("%s: %s" % (addDist, totalDist))
		prevCity = eachCity
	# get distance from last city back to first city
	addDist = dist(prevCity, cities[disc[0]])
	totalDist = totalDist + addDist 
#	print ("%s: %s" % (addDist, totalDist))

	# write output to file
	outFil.write(str(totalDist) + '\n')
	iterCities = iter(disc)
	for eachCity in iterCities:
		outFil.write(str(eachCity) + '\n')

	inFil.close()
	outFil.close()

# -----------------------------------------------------------------------------	
if __name__ == "__main__":
	main()
