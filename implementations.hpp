#ifndef IMPLEMENTATIONS_HPP_CHECK
#define IMPLEMENTATIONS_HPP_CHECK

#include <iostream>
#include <vector>
#include "render.hpp"

// connects each node with the closest node
std::vector<uint16_t> IMP_shortestPath(std::vector<Node> nodes) 
{
	std::vector<uint16_t> toReturn {};
	// holds the next node to connect to
	Node nextNode = nodes[0];
	
	while (1) {
		bool kill = true;
		for (uint16_t c = 0; c < nodes.size(); ++c) {
			// check if all nodes have been checked
			if (!nodes[c].check) {
				kill = false;
				break;
			}
		}
		if (kill) return toReturn;

		uint16_t closestNode;
		uint16_t closestNodeDistance = UINT16_MAX;
		
		for (uint16_t i = 0; i < nodes.size(); ++i) 
		{
			// check if the node is comparing something it shouldnt be
			if ((nodes[i].id == nextNode.id) || nodes[i].check) continue;
				
			// check if the node is closer than the next node
			float dist = distance({nextNode.X, nextNode.Y},{nodes[i].X, nodes[i].Y});
			if (dist < closestNodeDistance) {
				closestNode = nodes[i].id;
				closestNodeDistance = dist;
			}
		}
		// found the right node! use it. 
		toReturn.push_back(closestNode);
		nextNode = nodes[closestNode];
		nodes[closestNode].check = true;
	}
}

// returns the index of a node with a specific ID
uint16_t findId(const uint16_t& id) {
	for (uint16_t i = 0; i < N_NODES.size(); ++i) {
		if (N_NODES[i].id == id) 
			return i;
	}
	// shit, its not in the node list
	// what now
}

// finds the total distance of a node path
float nodeDistanceTotal(std::vector<uint16_t> path) 
{
	float totalDistance = 0;
	for (uint16_t i = 0; i < path.size(); ++i) 
	{
		// make sure its not the last node
		if (i == path.size()-1) break;
		totalDistance += distance(
			{N_NODES[findId(path[i  ])].X, N_NODES[findId(path[i  ])].Y},
			{N_NODES[findId(path[i+1])].X, N_NODES[findId(path[i+1])].Y}
		);
	}
	return totalDistance;
}

#endif