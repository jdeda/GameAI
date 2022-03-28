#include "decisiontree.h"


DecisionTreeNode& Action::makeDecision() const {

}

DecisionTreeNode& Action::getNode() const {

}

template <typename T>
T& Decision<T>::testValue(T value) const {

}

template <typename T>
DecisionTreeNode& Decision<T>::makeDecision() const {

}

template <typename T>
DecisionTreeNode& Decision<T>::getBranch(T value) const {

}

template <typename T>
DecisionTreeNode& Decision<T>::getTrueNode() const {

}

template <typename T>
DecisionTreeNode& Decision<T>::getFalseNode() const {

}

float FloatDecision::testValue() const {

}

DecisionTreeNode& FloatDecision::getBranch(float testValue) const {

}
