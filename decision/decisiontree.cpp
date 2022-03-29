#include "decisiontree.h"

CharacterAction CharacterDecisionNode::testData(bool monsterClose, bool followingPath) const {

}

CharacterDecisionNode* CharacterDecisionNode::getBranch(bool monsterClose, bool followingPath) const {

}

CharacterDecisionNode::CharacterDecisionNode(CharacterAction action_, CharacterDecisionNode* trueNode_, CharacterDecisionNode* falseNode_) :
    trueNode(trueNode_), falseNode(falseNode_) {
    action = action_;
}

CharacterDecisionNode CharacterDecisionNode::makeDecision(bool monsterClose, bool followingPath) const {

}

CharacterAction CharacterDecisionNode::getAction() const {
    return action;
}

CharacterDecisionTree::CharacterDecisionTree(const Graph& graph_, Character* character_, Location* mouse_, float* dt_, bool* monsterClose_, bool* followingPath_) : graph(graph_) {
    dt = dt_;
    character = character_;
    mouseLocation = mouse_;
    monsterClose = monsterClose_;
    followingPath = followingPath_;
    CharacterDecisionNode* followClickActionNode = new CharacterDecisionNode(followClick, NULL, NULL);
    CharacterDecisionNode* sittingActionNode = new CharacterDecisionNode(sitting, NULL, NULL);
    CharacterDecisionNode* isFollowingClickDecisionNode = new CharacterDecisionNode(character_no_action, followClickActionNode, sittingActionNode);
    CharacterDecisionNode* escapingActionNode = new CharacterDecisionNode(CharacterAction::escaping, NULL, NULL);
    root = new CharacterDecisionNode(character_no_action, escapingActionNode, followClickActionNode);
}

