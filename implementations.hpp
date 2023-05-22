#ifndef IMPLEMENTATIONS_HPP_CHECK
#define IMPLEMENTATIONS_HPP_CHECK

#include <iostream>
#include <vector>
#include <array>
#include "render.hpp"

// returns a variable amount of nodes that are closest to the genesis node
std::vector<Node> closestNodes(
	const std::vector<Node>& nodes,
	const Node& genesisNode,
	uint16_t nodeAmount,
	const std::vector<Node>& illegal = {}
) {
	// holds [nodeAmount] of the closest nodes
	std::vector<Node> toReturn {};
	// closest nodes
	// fill it with the first [nodeAmount] nodes just so the logic works
	std::vector<std::pair<Node,float>> closestNodes {};
	bool still = false;
	for (uint16_t m = 0; m < nodeAmount; ++m) {
		if (nodes[m].id == genesisNode.id) {
			still = true;
			continue;
		}
		closestNodes.push_back({
			nodes[m],
			distance({genesisNode.X,genesisNode.Y},{nodes[m].X,nodes[m].Y})
		});
	}
	if (still) {
		closestNodes.push_back({
			nodes[nodeAmount],
			distance(
				{genesisNode.X,genesisNode.Y},
				{nodes[nodeAmount].X,nodes[nodeAmount].Y}
			)
		});
	}
	if (N_NODEAMOUNT <= 1) return toReturn;
	// loop through all nodes
	for (auto& i : nodes) {
		// dont compare to anything in illegal
		bool kill = false;
		for (auto& x : illegal) if (x.id == i.id) kill = true;
		if (kill) continue;
		// also dont compare to yourself idiot
		if (i.id == genesisNode.id) continue;
		
		// distance from genesis node to current node
		double dist = distance({genesisNode.X,genesisNode.Y},{i.X,i.Y});
		// check if the node is shorter than everything in toReturn
		for (auto& n : closestNodes) {
			if (dist < n.second) {
				// replace the largest node in closestNodes with the new node
				uint16_t largestInd = 0;
				uint16_t largestVal = 0;
				for (uint16_t m = 0; m < closestNodes.size(); ++m) {
					if (closestNodes[m].second > largestVal) {
						largestVal = closestNodes[m].second;
						largestInd = m;
					}
				}
				closestNodes[largestInd] = {i,dist};
				break;
			}
		}
	}
	for (auto& b : closestNodes) toReturn.push_back(b.first);
	return toReturn;
}

// returns the index of a node with a specific ID
inline uint16_t findId(const uint16_t& id) {
	for (uint16_t i = 0; i < N_NODES.size(); ++i) 
		if (N_NODES[i].id == id) 
			return i;
	// shit, its not in the node list
	// what now
	// pray to god it never gets to this
	return UINT16_MAX;
}

// finds the total distance of a node path
double nodeDistanceTotal(std::vector<uint16_t> path) 
{
	double totalDistance = 0;
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

// connects each node with the closest node
std::vector<uint16_t> IMP_shortestPath(
	std::vector<Node> nodes,
	uint16_t variation = 0
) {
	std::vector<uint16_t> toReturn {};
	if (N_NODEAMOUNT <= 1) return toReturn;
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
			double dist = distance({nextNode.X, nextNode.Y},{nodes[i].X, nodes[i].Y});
			if (variation) {
				if (dist+(rand()%variation) < closestNodeDistance+(rand()%variation)) {
					closestNode = nodes[i].id;
					closestNodeDistance = (uint16_t)dist;
				}
			}
			else {
				if (dist < closestNodeDistance) {
					closestNode = nodes[i].id;
					closestNodeDistance = (uint16_t)dist;
				}
			}
		}
		// found the right node! use it. 
		toReturn.push_back(closestNode);
		nextNode = nodes[closestNode];
		nodes[closestNode].check = true;
	}
}


// makes small shortest paths and puts them all together
std::vector<uint16_t> IMP_shortestPath_branched(
	std::vector<Node> nodes,
	uint16_t branchAmount = 3,
	uint16_t branchLength = 3,
	uint16_t V1 = 0,
	uint16_t V2 = 0
) {
	std::vector<uint16_t> toReturn {};
	
	if (N_NODEAMOUNT <= 1) return toReturn;
	if ((branchAmount <= 1) || (branchAmount <= nodes.size())) return IMP_shortestPath(nodes,V1);
	
	std::vector<Node> illegal {};
	Node currentNode = nodes[0];

	while (1) {
		// if there arent enough nodes to branch
		if (branchAmount < (nodes.size()-illegal.size())) {
			// create a vector full of the remaining nodes, with the current node in the front
			std::vector<Node> remainingNodes {currentNode};
			std::sort(nodes.begin(),nodes.end());
			std::sort(illegal.begin(),illegal.end());
			std::set_difference(
				nodes.begin(),nodes.end(),
				illegal.begin(),illegal.end(),
				std::back_inserter(remainingNodes)
			);
			// connect the remaining nodes
			std::vector<uint16_t> path = IMP_shortestPath(remainingNodes,V1);
			// add the path to the return vector
			for (auto& p : path) toReturn.push_back(p);
			
			return toReturn;
		}
		// find branchAmount closest nodes
		std::vector<Node> cnodes = closestNodes(nodes,currentNode,branchAmount,illegal);
	}
}

#endif