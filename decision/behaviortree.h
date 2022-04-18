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
    guessing,
    nothing
};

class MonsterTask
{
    public:
    virtual MonsterAction run() = 0;
};

class MonsterNearby : public MonsterTask
{
    private:
    const float MAX_D = 20.f;
    Location monsterLocation;
    Location enemyLocation;

    public:
    MonsterNearby(const Location& monsterLocation_, const Location& enemyLocation_);
    inline MonsterAction run() {
        bool nearby = sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2)) < MAX_D;
        return nearby ? chasing : nothing;
    }
};

class MonsterChasing : public MonsterTask
{
    private:
    Location monsterLocation;
    Location enemyLocation;

    public:
    MonsterChasing(const Location& monsterLocation_, const Location& enemyLocation_);
    inline MonsterAction run() {
        return chasing;
    }
};

class MonsterWandering : public MonsterTask
{
    private:
    Location monsterLocation;
    Location enemyLocation;

    public:
    MonsterWandering(const Location& monsterLocation_, const Location& enemyLocation_);
    inline MonsterAction run() {
        return wandering;
    }
};

class MonsterGuessing : public MonsterTask
{
    private:
    Location monsterLocation;
    Location enemyLocation;

    public:
    MonsterGuessing(const Location& monsterLocation_, const Location& enemyLocation_);
    inline MonsterAction run() {
        return guessing;
    }
};

class MonsterSelector : public MonsterTask
{
    private:
    vector<MonsterTask*> children;
    inline MonsterAction run() {
        for (const auto& c : children) {
            auto status = c->run();
            if (status != nothing) { return status; }
        }
        return nothing;
    }
    public:
    MonsterSelector(const vector<MonsterTask*>& children);
};

class MonsterSequence : public MonsterTask
{
    private:
    vector<MonsterTask*> children;
    inline MonsterAction run() {
        auto status = nothing;
        for (const auto& c : children) {
            status = c->run();
            if (status == nothing) { return nothing; }
        }
        return status; // todo: which one to call status, prob last one ran
    }

    public:
    MonsterSequence(const vector<MonsterTask*>& children);

};

// int i = rand() % (children.size());
// cout << "rand: " << i << endl;
// choice = i;
class MonsterRandomSelector : public MonsterTask
{
    private:
    int choice = -1;
    float timer = 0;
    vector<MonsterTask*> children;
    inline MonsterAction run() {
        cout << "TIMER:  " << timer << endl;
        while (true) {
            if (choice == -1) {
                choice = rand() % (children.size());
                auto status = children[choice]->run();
                if (status != nothing) {
                    return status;
                }
                else {
                    choice = -1;
                }
            }
            else {
                cout << "GOOD\n";
                if (timer > 5.f) {
                    timer = 0.f;
                    choice = -1;
                }
                else { 
                    timer += 0.1;
                    cout << "INCR: " << timer << endl;
                    return children[choice]->run();
                }
            }
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

    /** Observable state for action code. */
    float* dt;
    Graph graph;
    Character* character;

    /** Root node. */
    MonsterTask* root;

    /** Last node that was successful. */
    // MonsterTask* last; // TODO: Add to constructor and add to BT.run().

    /** Monster movement path functionality. */
    Path path;
    AStar* search;
    FollowPath* pathFollowing;
    int chasingIteration = 0;

    public:
    MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_);
    inline void run() {
        auto decision = root->run();

        cout << "MONSTER: " << decision << endl;

        // If first iteration of chasingIteration, find path.
        if (decision == chasing && chasingIteration == 0) {
            search = new AStar(graph, monster->getLocation(), character->getLocation(), ManhattanHeuristic(character->getLocation()));
            path = search->search();
            path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
        }

        // If chasingIteration has complete, reset count. TODO: Probably bad way to do this.
        if (decision != chasing) {
            chasingIteration = 0;
        }

        switch (decision) {
            case chasing:
                {
                    SteeringOutput pathAccelerations = pathFollowing->calculateAcceleration(monster->getKinematic(), Kinematic());
                    if (pathAccelerations.linearAcceleration == Vector2f(-1.f, -1.f)) {
                        monster->stop(); // TODO: more hacking to be put here.
                    }
                    else {
                        monster->update(pathAccelerations, *dt, true);
                    }
                    chasingIteration += 1;
                    break;

                }
            case wandering:
                {
                    break;
                }
            case guessing:
                {
                    break;
                }
            case nothing:
                {
                    break;
                }
        }
    }
};
#endif