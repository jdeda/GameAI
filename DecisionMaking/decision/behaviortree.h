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
    const float MAX_D = 10.f;
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

    /** Last decision functionality. */
    // MonsterTask* last; // TODO: Add to constructor and add to BT.run().
    MonsterAction lastDecision = nothing;

    /** Monster movement path functionality. */
    Path path;
    AStar* search;
    FollowPath* pathFollowing;
    int chasingIteration = 0;
    int chasingPauseIteration = 0;
    bool chasingPause = false;

    /** Monster wandering functionality. */
    Wander* wander;
    int wanderIteration = 0;
    bool wanderPause = false;

    /** Monster guessing functionality. */
    int guessIteration = 0;
    bool guessPause = false;

    public:
    bool isChasing = false;
    inline MonsterAction generateAction(MonsterAction decision) {
        MonsterAction newDecision = decision;
        while (newDecision != decision) {
            newDecision = root->run();
        }
        return newDecision;
    }

    inline bool setupChasing(MonsterAction decision) {
        // cout << "CHASE IT:  " <<  chasingIteration << endl;
        // If been chasing long enough, pause momentarily from taking chasing action again.
        if (chasingPause == true) {
            // cout << "YES" << endl;
            // cout << "CPAUSE IT: " << chasingPauseIteration << endl;
            if (chasingPauseIteration > 50) {
                chasingPauseIteration = 0;
                chasingPause = false;
                return true;
            }
            else {
                chasingPauseIteration += 1;
                return false;
            }
        }

        // If been chasing long enough, do nothing and pause momentarily from taking chasing action again.
        if (decision == chasing && chasingIteration > 200) {
            chasingIteration = 0;
            isChasing = false;
            chasingPause = true; // TODO: Set location or arrive to nearest square.
            return true;
        }
        

        // If first iteration of chasingIteration, find path.
        if (decision == chasing && chasingIteration == 0) {
            // cout << "YEP" << endl;
            // isChasing = true;
            if (monster->getLocation() == Location(-1, -1)) {
                auto p = monster->getPosition();

                auto p1 = p;
                auto p2 = p;
                auto p3 = p;
                auto p4 = p;
                p1.x += 29.0909 / 2.f; // Right.
                p2.x -= 29.0909 / 2.f; // Left.
                p3.y += 29.0909 / 2.f; // Up.
                p4.y -= 29.0909 / 2.f; // Down.


                auto p5 = p1;
                auto p6 = p2;
                auto p7 = p3;
                auto p8 = p4;
                p5.y += 29.0909 / 2.f; // Right.
                p6.y -= 29.0909 / 2.f; // Left.
                p7.x += 29.0909 / 2.f; // Up.
                p8.x -= 29.0909 / 2.f; // Down.

                vector<Vector2f> neighbors;
                neighbors.push_back(p1);
                neighbors.push_back(p2);
                neighbors.push_back(p3);
                neighbors.push_back(p4);
                neighbors.push_back(p5);
                neighbors.push_back(p6);
                neighbors.push_back(p7);
                neighbors.push_back(p8);

                bool found = false;
                for (const auto& neighbor : neighbors) {
                    auto v = mapToLevel(22, 29.0909, neighbor);
                    if (!(v == Location(-1, -1))) {
                        found = true;
                        monster->moveTo(neighbor);
                    }
                }
                if (!found) {
                    exit(99);
                }
            }
            search = new AStar(graph, monster->getLocation(), character->getLocation(), CustomHeuristic(character->getLocation()));
            path = search->search();
            // path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
        }

        // If chasingIteration has complete, reset count. TODO: Probably bad way to do this.
        if (decision != chasing) {
            chasingIteration = 0;
            monster->stop();
            isChasing = false;
        }
        return true;
    }

    inline bool setupWandering() {
        if (wanderIteration > 100) {
            wanderIteration = 0;
            wanderPause = true; // TODO: Set location or arrive to nearest square.
            return true;
        }

        if (wanderIteration == 0) {
            wander = new Wander(
                WANDER_OFFSET, WANDER_RADIUS, WANDER_RATE, WANDER_ORIENTATION, WANDER_MAX_ACCELERATION,
                TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED
            );
            search = new AStar(graph, monster->getLocation(), monster->getLocation(), CustomHeuristic(monster->getLocation()));
            path = search->search();
            // path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
        }

        return true;

    }

    inline bool setupGuessing() {
        if (guessIteration > 100) {
            guessIteration = 0;
            guessPause = true;
        }
        return true;
    }

    inline void preCheck(MonsterAction decision) {
        if (lastDecision == chasing && decision != chasing) {
            chasingIteration = 0;
            chasingPauseIteration= 0;
            monster->stop();
            isChasing = false;
        }
        else if (lastDecision == wandering && decision != wandering) {
            wanderIteration = 0;
            monster->stop();
            wanderPause = false;
        }
        else if (lastDecision == guessing && decision != guessing) {
            guessIteration = 0;
            monster->stop();
            guessPause = false;
        }
        else {
            // Do nothing.
        }
    }

    inline MonsterAction setupAction(MonsterAction decision) {
        preCheck(decision);
        switch (decision) {
            case chasing:
                {
                    auto status = setupChasing(decision);
                    return status ? decision : generateAction(decision);
                }
            case wandering:
                {
                    auto status = setupWandering();
                    return decision;
                }
            case guessing:
                {
                    auto status = setupGuessing();
                    return decision;

                }
            default:
                {
                    return decision;
                }
        }
    }

    inline float makeRandom(const Vector2f& p) {
        // int offset = 15;
        // int max = 600 - offset;
        // int min = 30 + offset;
        // if (p.x > p.y) {
        //     max = p.x + offset;
        //     min = p.y - offset;
        // }
        // else {
        //     max = p.y + offset;
        //     min = p.x - offset;
        // }
        // float x = float(rand() % (max - min + 1) + min);
        // return x;

        // int max = 600;
        // int min = 30;
        // float x = float(rand() % (max - min + 1) + min);
        // return x;

        int offset = 15;
        int max = SCENE_WINDOW_X - offset; // Global.
        int min = SCENE_WINDOW_X + offset;
        if (p.x > p.y) { // Localize to p with some offset.
            max = p.x + offset;
            min = p.y - offset;
        }
        else { // Localize to p with some offset.
            max = p.y + offset;
            min = p.x - offset;
        }
        float x = float(rand() % (max - min + 1) + min);
        return x;


        // float x = float(rand() % (max - min + 1) + min);
    }

    inline Vector2f makeGuess(const Vector2f& p) {
        float timer = 0.0;
        Vector2f guessPosition(makeRandom(p), makeRandom(p));
        while (mapToLevel(22, 29.0909, guessPosition) == Location(-1, -1)) {
            guessPosition = Vector2f(makeRandom(p), makeRandom(p));
            if (timer > 5.0) {
                return p;
            }
            timer += 0.1;
        }
        return guessPosition;
    }

    MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_);

    inline void run() {
        auto decision = setupAction(root->run());
        lastDecision = decision;
        switch (decision) {
            case chasing:
                {
                    cout << "CHASING" << endl;
                    SteeringOutput pathAccelerations = pathFollowing->calculateAcceleration(monster->getKinematic(), Kinematic());
                    if (pathAccelerations.linearAcceleration == Vector2f(-1.f, -1.f)) {
                        monster->stop();
                    }
                    else {
                        monster->update(pathAccelerations, *dt, true);
                    }
                    chasingIteration += 1;
                    break;

                }
            case wandering:
                {
                    cout << "WANDERING" << endl;
                    // SteeringOutput wanderAccelerations = wander->calculateAcceleration(monster->getKinematic(), monster->getKinematic());
                    // monster->update(wanderAccelerations, *dt, true);
                    // cout << "OK" << endl;
                    // cout << "oh: " << wanderAccelerations.linearAcceleration.x << " " << wanderAccelerations.linearAcceleration.y << endl;
                    SteeringOutput wanderAccelerations = wander->calculateAcceleration(monster->getKinematic(), monster->getKinematic());
                    Kinematic check;
                    check.position.x = monster->getKinematic().position.x;
                    check.position.y = monster->getKinematic().position.y;
                    check.orientation = monster->getKinematic().orientation;
                    check.linearVelocity.x = monster->getKinematic().linearVelocity.x;
                    check.linearVelocity.y = monster->getKinematic().linearVelocity.y;
                    check.angularVelocity = monster->getKinematic().angularVelocity;
                    check.update(wanderAccelerations, *dt, true);
                    if (mapToLevel(22, 29.09, check.position) == Location(-1, -1)) {
                        monster->update(SteeringOutput(), *dt, true);
                    }
                    else {
                        monster->update(wanderAccelerations, *dt, true);
                    }
                    wanderIteration += 1;
                    break;
                }
            case guessing:
                {
                    cout << "GUESSING" << endl;
                    // what if guess is bad
                    // handle index out of bounds.
                    Vector2f guessPosition = makeGuess(monster->getPosition());
                    monster->moveTo(guessPosition);
                    guessIteration += 1;
                    break;
                }
            case nothing:
                {
                    cout << "DOING NOTHING" << endl;
                    break;
                }
        }
    }

    inline int getChasingIteration() { return chasingIteration; }

    inline Path getPath() { return path; }
};

#endif