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
    public:
    MonsterDecisionNode(MonsterDecisionNode& trueNode_, MonsterDecisionNode& falseNode_);
    MonsterDecisionNode makeDecision(MonsterAction data_) const;
};

enum CharacterAction
{
    followClick,
    escaping,
    sitting
};

class CharacterDecisionNode
{
    private:
    CharacterAction action;
    CharacterDecisionNode* trueNode;
    CharacterDecisionNode* falseNode;

    public:
    CharacterDecisionNode(CharacterAction action, CharacterDecisionNode* trueNode_, CharacterDecisionNode* falseNode_);
    
    inline CharacterDecisionNode makeDecision(bool monsterClose, bool followingPath) const {
        if(trueNode == NULL && falseNode == NULL) {
            return CharacterDecisionNode(action, NULL, NULL);
        } 
        switch (action) {
            case CharacterAction::followClick:
                {
                    if(followingPath) {
                        return trueNode->makeDecision(monsterClose, followingPath);
                    }
                    else {
                        return falseNode->makeDecision(monsterClose, followingPath);
                    }
                }
            case CharacterAction::escaping:
                {
                    if(monsterClose) {
                        return trueNode->makeDecision(monsterClose, followingPath);
                    }
                    else {
                        return falseNode->makeDecision(monsterClose, followingPath);
                    }
                }
            case CharacterAction::sitting:
                {
                    return CharacterDecisionNode(CharacterAction::sitting, NULL, NULL);
                }
            default: {
                cout << "exit 99" << endl;
                exit(99);
                return CharacterDecisionNode(CharacterAction::sitting, NULL, NULL);
            }
        }
    };
    inline CharacterAction getAction() const { return action; }
};


    class CharacterDecisionTree
    {
        private:

        /* Mutable state when decision is carried out. */
        Character* character;

        /** Observable state for decision making. */
        bool* monsterClose;
        bool* followingPath;

        /** Observable state for action code. */
        float* dt;
        Graph graph;
        Location* mouseLocation;


        /** Root node. */
        CharacterDecisionNode* root;

        /** followClick decision functionality. */
        Path path;
        AStar* search;
        FollowPath* pathFollowing;
        int followingIteration = 0;

        public:
        CharacterDecisionTree(const Graph& graph, Character* character_, Location* mouse_, float* dt, bool* monsterClose_, bool* followingPath_);

        inline void makeDecision() {
            CharacterAction action = root->makeDecision(*monsterClose, *followingPath).getAction();  

            // If first iteration of followingClick, find path.
            if(action == followClick && followingIteration == 0) {
                search = new AStar(graph, character->getLocation(), *mouseLocation, ManhattanHeuristic(*mouseLocation));
                path = search->search();
                path.print();
                pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
            }

            // If followingClick has complete, reset count.
            if(*followingPath == false) {
                followingIteration = 0;
                cout << character->getLocation().x << " " << character->getLocation().y << endl;
            }

            switch (action) {
                case CharacterAction::followClick:
                    {
                        // TODO: path finding broken...
                        // TODO: Sometimes gets stuck...
                        // Need to fix start position error...
                        // Apply path following to click.
                        // TODO: indexes can be infinitely large 
                        // TODO: Keeps accelerating even if out of bounds...because velocity neeeds to be set to 0... kiindad hacky
                        SteeringOutput pathAccelerations = pathFollowing->calculateAcceleration(character->getKinematic(), Kinematic());
                        cout << "accel: " << pathAccelerations.linearAcceleration.x << " " << pathAccelerations.linearAcceleration.y <<  "\n\n\n";                        
                        character->update(pathAccelerations, *dt, true);
                        followingIteration += 1;
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
            }
        }

        inline int getFollowingIteration() { return followingIteration; }

        inline Path getPath() { return path; }
    };




#endif