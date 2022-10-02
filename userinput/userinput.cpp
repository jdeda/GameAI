#include <vector>
#include <string>
#include <iostream>
#include "../debug/debug.h"
#include "userinput.h"

using namespace std;

vector<string> VisualizerStrings{
    "Decision Tree",
    "Behavior Tree",
    "Decision Tree Learning",
};

Visualizer* getVisualizer() {
    cout << "Choose a graph to visualize" << endl;
    for (int i = 0; i < VisualizerStrings.size(); i++) {
        cout << i + 1 << ". " << VisualizerStrings[i] << endl;
    }

    // Get visualizer number.
    int caseNum = 0;
    cout << "Enter a Visualizer choice: ";
    cin >> caseNum;
    if (cin.bad()) { fail("invalid input"); }
    cout << endl;

    // Map to enum.
    caseNum--;
    if (caseNum < 0 || caseNum >= VisualizerStrings.size()) { return NULL; }
    else { return new Visualizer[caseNum]; }
}