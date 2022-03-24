#ifndef DECISION_TREE_H
#define DECISION_TREE_H

class DecisionTreeNode
{
public:
    virtual DecisionTreeNode& makeDecision() const = 0;
};

class Action : protected DecisionTreeNode
{
private:
    const DecisionTreeNode& node;

public:
    DecisionTreeNode& makeDecision() const;
    DecisionTreeNode& getNode() const;
};

template <typename T>
class Decision : protected DecisionTreeNode
{
private:
    DecisionTreeNode& trueNode;
    DecisionTreeNode& falseNode;

public:
    T testValue(T value) const;;
    DecisionTreeNode& makeDecision() const;
    DecisionTreeNode& getBranch(T value) const;;
    DecisionTreeNode& getTrueNode() const;
    DecisionTreeNode& getFalseNode() const;
};

class FloatDecision : protected Decision<float>
{
    private:
    float minValue;
    float maxValue;

    public:
    float testValue() const;
    DecisionTreeNode& getBranch(float testValue) const;

};

#endif