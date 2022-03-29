#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <stddef.h>
#include "../character/character.h"
#include "../environment/environment.h"
#include "../search/search.h"
#include "../search/a*.h"
#include "../level/location.h"
#include "../steering/steering.h"
#include "../kinematic/kinematic.h"

enum MonsterAction
{
    wandering,
    chasing,
    waiting
};

class MonsterDecisionNode
{
    private:
    MonsterAction action;
    MonsterDecisionNode& trueNode;
    MonsterDecisionNode& falseNode;
    MonsterDecisionNode testData(MonsterAction data) const;
    MonsterDecisionNode& getBranch(MonsterAction data) const;
    public:
    MonsterDecisionNode(MonsterDecisionNode& trueNode_, MonsterDecisionNode& falseNode_);
    MonsterDecisionNode makeDecision(MonsterAction data_) const;
};

enum CharacterAction
{
    followClick,
    escaping,
    sitting,
    character_no_action
};

class CharacterDecisionNode
{
    private:
    CharacterAction action;
    CharacterDecisionNode* trueNode;
    CharacterDecisionNode* falseNode;
    CharacterAction testData(bool monsterClose, bool followingPath) const;
    CharacterDecisionNode* getBranch(bool monsterClose, bool followingPath) const;
    public:
    CharacterDecisionNode(CharacterAction action, CharacterDecisionNode* trueNode_, CharacterDecisionNode* falseNode_);
    CharacterDecisionNode makeDecision(bool monsterClose, bool followingPath) const;
    CharacterAction getAction() const;
};

class CharacterDecisionTree
{
    private:

    /* Mutable state when decision is carried out. */
    Character* character;

    /** Observable state for decision making. */
    float *dt;
    bool* monsterClose;
    bool* followingPath;
    Location* mouseLocation;
    Graph graph;

    /** Root node. */
    CharacterDecisionNode* root;

    /** Other. */
    Path path;

    public:
    CharacterDecisionTree(const Graph& graph, Character* character_, Location* mouse_, float* dt, bool* monsterClose_, bool* followingPath_);

    inline CharacterDecisionNode makeDecision() {
        CharacterAction action = root->makeDecision(*monsterClose, *followingPath).getAction();
        switch (action) {
            case CharacterAction::followClick:
                {
                    // Apply path finding to click.
                    AStar search(graph, character.getLocation(), mouseLocation);
                    path =  search.search();
                    FollowPath pathFollowing = FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
                    SteeringOutput pathAccelerations = pathFollowing.calculateAcceleration(character->getKinematic(), Kinematic());
                    character->update(pathAccelerations, *dt, true);
                    break;
                }
            case CharacterAction::escaping:
                {
                    break;
                }
            case CharacterAction::sitting:
                {
                    break;
                }
            case CharacterAction::character_no_action:
                {
                    break;
                }
        }

    }

    inline Path getPath() { return path; }
};




#endif