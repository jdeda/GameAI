#ifndef USERINPUT_H
#define USERINPUT_H

#include <vector>
#include <string>

using namespace std;

enum Algorithm
{
    DIJKSTRA,
    A_STAR_H1,
    A_STAR_H2,
    A_STAR_CUSTOM,
    INVALID_ALG
};

extern vector<string> AlgorithmStrings;

Algorithm getAlgorithm();

enum Visualizer
{
    small,
    big,
    huge,
    character,
    test,
    INVALID_VIS
};

extern vector<string> VisualizerStrings;

Visualizer getVisualizer();

void greeting();

#endif