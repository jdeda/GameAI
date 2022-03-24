#ifndef USERINPUT_H
#define USERINPUT_H

#include <vector>
#include <string>

using namespace std;

extern vector<string> VisualizerStrings;

enum Visualizer
{
    DecisionTree,
    BehaviorTree,
    DecisionTreeLearning
};

Visualizer* getVisualizer();

#endif