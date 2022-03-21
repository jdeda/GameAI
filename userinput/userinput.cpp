#include <string>
#include <iostream>
#include <vector>
#include "userinput.h"
#include "../debug/debug.h"

using namespace std;

/** Program's initial message. */
void greeting() {
	cout << "**************************************" << endl;
	cout << "*****Welcome to PathPlanning*****" << endl;
	cout << "**************************************" << endl;
}

vector<string> AlgorithmStrings{
	"Dijkstra's",
	"A* (manhattan heuristic)",
	"A* (euclidean heuristic)"
};

vector<string> VisualizerStrings{
	"Small Graph",
	"Big Graph",
	"Huge Graph",
	"Character Steering Graph"
};

/** Returns an Algorithm if input is valid, otherwise fail program. */
Algorithm getAlgorithm() {
	cout << "Choose an algorithm for pathfinding:" << endl;
	for (int i = 0; i < AlgorithmStrings.size(); i++) {
		cout << i + 1 << ". " << AlgorithmStrings[i] << endl;
	}

	// Get algorithm number.
	int caseNum = 0;
	cout << "Enter an algorithm number: ";
	cin >> caseNum;
	if (cin.bad()) { fail("invalid algorithm choice"); }
	cout << endl;

	// Map to enum.
	caseNum--;
	if (caseNum < 0 || caseNum > AlgorithmStrings.size()) { return Algorithm::INVALID_ALG; }
	else { return Algorithm(caseNum); }
}

/** Returns a Visualizer if input is valid, otherwise fail program. */
Visualizer getVisualizer() {
	cout << "Choose a graph to visualize" << endl;
	for (int i = 0; i < VisualizerStrings.size(); i++) {
		cout << i + 1 << ". " << VisualizerStrings[i] << endl;
	}

	// Get visualizer number.
	int caseNum = 0;
	cout << "Enter a Visualizer choice: ";
	cin >> caseNum;
	if (cin.bad()) { fail("invalid visualizer choice"); }
	cout << endl;

	// Map to enum.
	caseNum--;
	if (caseNum < 0 || caseNum > VisualizerStrings.size()) { return Visualizer::INVALID_VIS; }
	else { return Visualizer(caseNum); }
}