#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include <stddef.h>
#include <cmath>
#include "../character/character.h"
#include "../environment/environment.h"
#include "../search/search.h"
#include "../search/a*.h"
#include "../level/location.h"
#include "../steering/steering.h"
#include "../kinematic/kinematic.h"

using namespace std;

enum MonsterAction
{
    chasing,
    wandering,
    guessing
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

class MonsterTask
{

    public:
    virtual bool run() const = 0;
};

class MonsterChasing : MonsterTask
{
    private:
    const float MAX_D = 20.f;
    Location monsterLocation;
    Location enemyLocation;

    public:
    inline bool run() const {
        return sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2)) < MAX_D;
    }

};

class MonsterWandering : MonsterTask
{
    private:
    const float MAX_D = 20.f;
    Location monsterLocation;
    Location enemyLocation;

    public:
    inline bool run() const {
        return sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2)) < MAX_D;
    }
};

class MonsterGuessing : MonsterTask
{
    private:
    const float MAX_D = 20.f;
    const float MAX_T = 2.f;
    float time;
    Location monsterLocation;
    Location enemyLocation;

    public:
    inline bool run() const {
        bool chasing = sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2)) < MAX_D;
        if (!chasing) {
            return time < MAX_T;
        }
        else {
            return false;
        }
    }
};

// For monster chase.
class MonsterSelector: MonsterTask {
    private:
    vector<MonsterTask> children;
    inline bool run() const {
        for(const auto& c: children) {
            if(c.run()) { return true;}
        }
        return false;
    }
};

// For monster wander.
class MonsterSequence: MonsterTask {
    private:
    vector<MonsterTask> children;
    inline bool run() const {
        for(const auto& c: children) {
            if(!c.run()) { return false; }
        }
        return true;
    }
};

// For monster guess.
class MonsterRandomSelector: MonsterTask {
    private:
    vector<MonsterTask> children;
    inline bool run() const {
        while(true) {
            if(children[rand() % (children.size() - 1)].run()) { return true; }
        }
    }
};

#endif