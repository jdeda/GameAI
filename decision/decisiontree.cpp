#include "decisiontree.h"

CharacterDecisionNode::CharacterDecisionNode(CharacterAction action_, CharacterDecisionNode* trueNode_, CharacterDecisionNode* falseNode_) :
    trueNode(trueNode_), falseNode(falseNode_) {
    action = action_;
}

// CharacterDecisionTree::CharacterDecisionTree(const Graph& graph_, Character* character_, Location* mouse_, float* dt_, bool* monsterClose_, bool* followingPath_) : 
// graph(graph_), search(graph_, character->getLocation(), *mouse_, ManhattanHeuristic(*mouse_)),
// pathFollowing(Path(), PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED) {
//     dt = dt_;
//     character = character_;
//     mouseLocation = mouse_;
//     monsterClose = monsterClose_;
//     followingPath = followingPath_;
//     CharacterDecisionNode* followClickActionNode = new CharacterDecisionNode(followClick, NULL, NULL);
//     CharacterDecisionNode* sittingActionNode = new CharacterDecisionNode(sitting, NULL, NULL);
//     CharacterDecisionNode* isFollowingClickDecisionNode = new CharacterDecisionNode(CharacterAction::followClick, followClickActionNode, sittingActionNode);
//     CharacterDecisionNode* escapingActionNode = new CharacterDecisionNode(CharacterAction::escaping, NULL, NULL);
//     root = new CharacterDecisionNode(CharacterAction::escaping, escapingActionNode, followClickActionNode);
// }

CharacterDecisionTree::CharacterDecisionTree(const Graph& graph_, Character* character_, Location* mouse_, float* dt_, bool* monsterClose_, bool* followingPath_) : graph(graph_) {
    dt = dt_;
    character = character_;
    mouseLocation = mouse_;
    monsterClose = monsterClose_;
    followingPath = followingPath_;
    CharacterDecisionNode* followClickActionNode = new CharacterDecisionNode(followClick, NULL, NULL);
    CharacterDecisionNode* sittingActionNode = new CharacterDecisionNode(sitting, NULL, NULL);
    CharacterDecisionNode* isFollowingClickDecisionNode = new CharacterDecisionNode(CharacterAction::followClick, followClickActionNode, sittingActionNode);
    CharacterDecisionNode* escapingActionNode = new CharacterDecisionNode(CharacterAction::escaping, NULL, NULL);
    root = new CharacterDecisionNode(CharacterAction::escaping, escapingActionNode, followClickActionNode);
}

