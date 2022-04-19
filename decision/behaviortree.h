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
#include "../debug/debug.h"

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
    Character* character;
    Character* monster;

    public:
    MonsterNearby(Character* character_, Character* monster_);
    inline MonsterAction run() {
        auto enemyLocation = character->getLocation();
        auto monsterLocation = monster->getLocation();
        float distance = sqrt(pow(enemyLocation.x - monsterLocation.x, 2) + pow(enemyLocation.y - monsterLocation.y, 2));
        bool nearby = distance < MAX_D;
        return nearby ? chasing : nothing;
    }
};

class MonsterChasing : public MonsterTask
{
    public:
    inline MonsterAction run() {
        return chasing;
    }
};

class MonsterWandering : public MonsterTask
{
    public:
    inline MonsterAction run() {
        return wandering;
    }
};

class MonsterGuessing : public MonsterTask
{
    public:
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

class MonsterRandomSelector : public MonsterTask
{
    private:
    MonsterAction choiceStatus = nothing;
    int choice = -1;
    float timer = 0;
    vector<MonsterTask*> children;
    inline MonsterAction run() {
        while (true) {
            if (choice == -1) {
                choice = rand() % (children.size());
                choiceStatus = children[choice]->run();
                if (choiceStatus != nothing) {
                    return choiceStatus;
                }
                else {
                    choice = -1;
                }
            }
            else {
                if (timer > 5.f) {
                    timer = 0.f;
                    choice = -1;
                    choiceStatus = nothing;
                }
                else {
                    if (choiceStatus == guessing) {
                        timer += 0.2;

                    }
                    else if (choiceStatus == wandering) {
                        timer += 0.05;

                    }
                    else {
                        fail("random selector invalid choice status");

                    }
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
    int chasingPauseIteration = 0;
    bool chasingPause = false;

    public:
    bool isChasing = false;
    MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_);
    inline void run() {
        auto decision = root->run();
        cout << "IT : " << chasingIteration << endl;
        cout << "[IT] : " << chasingPause << endl;

        cout << "MONSTER: " << decision << endl;
        // If been chasing long enough, pause momentarily from taking chasing action again.
        if (chasingPause == true) {
            cout << "YAY" << endl;
            if (chasingPauseIteration > 50) {
                chasingPauseIteration = 0;
                chasingPause = false;
                return;
            }
            else {
                cout << "NOOOOOO" << endl;
                while (decision != chasing) {
                    decision = root->run();
                }
                chasingPauseIteration += 1;
            }
        }

        // If been chasing long enough, do nothing and pause momentarily from taking chasing action again.
        if (decision == chasing && chasingIteration > 200) {
            chasingPause = true; // TODO: Set location or arrive to nearest square.
            return;
        }

        // If first iteration of chasingIteration, find path.
        if (decision == chasing && chasingIteration == 0) {
            search = new AStar(graph, monster->getLocation(), character->getLocation(), CustomHeuristic(character->getLocation()));
            path = search->search();
            path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
        }

        // If chasingIteration has complete, reset count. TODO: Probably bad way to do this.
        if (decision != chasing) {
            chasingIteration = 0;
            monster->stop();
            isChasing = false;
            cout << "STOPPED" << endl;
        }
        switch (decision) {
            case chasing:
                {
                    isChasing = true;
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
    inline int getChasingIteration() { return chasingIteration; }

    inline Path getPath() { return path; }
};

// TODO: Run inadmissible heuristic every time because character moves. 
       // You could adjust this to only recalibrate when character has changed an entire square (or two).
       // monster->stop();
       // search = new AStar(graph, monster->getLocation(), character->getLocation(), CustomHeuristic(character->getLocation()));
       // path = search->search();
       // path.print();
       // pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);

#endif