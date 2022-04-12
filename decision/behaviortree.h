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

class MonsterTask
{

public:
    virtual bool run() const = 0;
};

class MonsterChasing : public MonsterTask
{
private:
    const float MAX_D = 20.f;
    Location monsterLocation;
    Location enemyLocation;

public:
    MonsterChasing(const Location& monsterLocation_, const Location& enemyLocation_);
    inline bool run() const {
        return sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2)) < MAX_D;
    }
};

class MonsterWandering : public MonsterTask
{
private:
    const float MAX_D = 20.f;
    Location monsterLocation;
    Location enemyLocation;

public:
    MonsterWandering(const Location& monsterLocation_, const Location& enemyLocation_);
    inline bool run() const {
        return sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2)) < MAX_D;
    }
};

class MonsterGuessing : public MonsterTask
{
private:
    const float MAX_D = 20.f;
    const float MAX_T = 2.f;
    float time = 0;
    Location monsterLocation;
    Location enemyLocation;

public:
    MonsterGuessing(const Location& monsterLocation_, const Location& enemyLocation_);
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
class MonsterSelector : public MonsterTask
{
private:
    vector<MonsterTask*> children;
    inline bool run() const {
        for (const auto& c : children) {
            if (c->run()) { return true; }
        }
        return false;
    }
    public:
     MonsterSelector(const vector<MonsterTask*>& children);
};

// For monster wander.
class MonsterSequence : public MonsterTask
{
private:
    vector<MonsterTask*> children;
    inline bool run() const {
        for (const auto& c : children) {
            if (!c->run()) { return false; }
        }
        return true;
    }

    public:
    MonsterSequence(const vector<MonsterTask*>& children);

};

// For monster guess.
class MonsterRandomSelector : public MonsterTask
{
private:
    vector<MonsterTask*> children;
    inline bool run() const {
        while (true) {
            if (children[rand() % (children.size() - 1)]->run()) { return true; }
        }
    }
    public:
    MonsterRandomSelector(const vector<MonsterTask*>& children);
};


class MonsterBehaviorTree
{

private:

    /* Mutable state when decision is carried out. */
    Character* monster;

    /** Observable state for decision making. */
    bool* monsterClose;

    /** Observable state for action code. */
    float* dt;
    Graph graph;
    Character* character;

    /** Root node. */
    MonsterTask* root;

    /** Monster movement path functionality. */
    Path path;
    AStar* search;
    FollowPath* pathFollowing;
    int followingIteration = 0;

    public:
    MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_, bool* monsterClose_);
    inline void run() {
        auto decision = root->run();
    }
};
#endif