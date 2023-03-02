#ifndef IMPLEMENTATIONS_HPP_CHECK
#define IMPLEMENTATIONS_HPP_CHECK

#include <iostream>
#include <vector>
#include <array>
#include "render_.hpp"

// returns a variable amount of nodes that are closest to the genesis node
std::vector<Node> closestNodes(
	const std::vector<Node>& nodes,
	const Node& genesisNode,
	const uint16_t& nodeAmount,
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
	const uint16_t& variation = 0
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

// iterate through [N1] possible paths
// then through [N2] possible subpaths
// then through [N3] possible sub-subpaths
// then chooses the shortest one and adds it to the main path
std::vector<uint16_t> IMP_smallIterPath(
	std::vector<Node> nodes,
	const uint16_t& N1 = 3,
	const uint16_t& N2 = 3,
	const uint16_t& N3 = 3
) {
	// holds the current genesis node
	Node genesisNode = nodes[0];

	std::vector<uint16_t> toReturn {genesisNode.id};
	if (N_NODEAMOUNT <= 1) return toReturn;
	// holds the nodes that are already in the path
	std::vector<Node> illegals {genesisNode};
	while (1) {
		std::cout << "\nILLEGALS: ";
		for (auto& il : illegals) std::cout << il.id << ' ';
		std::cout << '\n';

		// not enough nodes? fall back onto IMP_shortestPath
		if ((N_NODES.size() - illegals.size()) < N1*N2) {

			std::cout << "not enough nodes! using IMP_shortestPath\n";

			for (auto& e : IMP_shortestPath(nodes)) toReturn.push_back(e);
			return toReturn;
		}
		// holds the attempted paths
		std::vector<std::vector<uint16_t>> paths {};

		// fill the array with the closest nodes
		std::vector<Node> cnodes_1 = closestNodes(nodes,genesisNode,N1,illegals);

		std::cout << "closest nodes: ";
		for (auto& e : cnodes_1) std::cout << e.id << " ";
		std::cout << '\n';

		std::vector<Node> cnodes_2;
		// get subpaths for each node
		for (uint16_t x = 0; x < N1; ++x) {
			std::vector<Node> tempIllegal = illegals;
			// fill the array with the closest nodes
			cnodes_2 = closestNodes(nodes,cnodes_1[x],N2,tempIllegal);

			std::cout << "\tclosest nodes for " << cnodes_1[x].id << ": ";
			for (auto& q : cnodes_2) std::cout << q.id << " ";
			std::cout << '\n';

			// fill possible paths with [N1] + each [N2] subpath
			for (uint16_t p2 = 0; p2 < N2; ++p2) {
				paths.push_back({
					genesisNode.id,
					cnodes_1[x].id,
					cnodes_2[p2].id
				});
			}

		}

		// find the shortest path
		uint16_t shortIndex = 0;
		float shortVal = (float)UINT16_MAX;
		for (uint16_t i = 0; i < paths.size(); ++i) {
			float temp = nodeDistanceTotal(paths[i]);
			if (temp < shortVal) {
				shortVal = temp;
				shortIndex = i;
			}
		}
		std::cout << "shortest path is " << shortIndex << " with a value of " << shortVal << '\n';

		// add the shortest path to toReturn
		// also put them il illegals
		for (uint16_t i = 1; i < paths[shortIndex].size(); ++i) {
			Node node = nodes[findId(paths[shortIndex][i])];

			std::cout << "\tid: " << node.id << '\n';

			toReturn.push_back(paths[shortIndex][i]);
			illegals.push_back(node);
		}
		std::cout << '\n';

		// set genesis node as last node of path
		genesisNode = nodes[findId(toReturn[toReturn.size()-1])];
	}
	return toReturn;
}

#endif
