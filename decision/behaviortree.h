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
    const float MAX_D = 5.f;
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
    inline MonsterAction generateAction(MonsterAction decision) {
        MonsterAction newDecision = decision;
        while (newDecision != decision) {
            newDecision = root->run();
        }
        return newDecision;
    }

    inline bool setupChasing(MonsterAction decision) {
        // If been chasing long enough, pause momentarily from taking chasing action again.
        if (chasingPause == true) {
            if (chasingPauseIteration > 50) {
                chasingPauseIteration = 0;
                chasingPause = false;
                return true;
            }
            else {
                return false;
                chasingPauseIteration += 1;
            }
        }

        // If been chasing long enough, do nothing and pause momentarily from taking chasing action again.
        if (decision == chasing && chasingIteration > 200) {
            chasingPause = true; // TODO: Set location or arrive to nearest square.
            return true;
        }

        // If first iteration of chasingIteration, find path.
        if (decision == chasing && chasingIteration == 0) {
            cout << "YEP" << endl;
            if (monster->getLocation() == Location(-1, -1)) {
                cout << "HOLY SHIT" << endl;

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

                // TODO: Get closest one!
                // cout << "CHECK IT" << endl;
                // for (const auto& neighbor : neighbors) {
                //     auto v = mapToLevel(22, 29.0909, neighbor);
                //     cout << neighbor.x << " " << neighbor.y << endl;
                //     cout << v.x << " " << v.y << endl << endl;
                // }
                // cout << endl;


                // cout << monster->getLocation().x << " " << monster->getLocation().y << endl;
                // cout << p.x << " " << p.y << endl;
                bool found = false;
                for (const auto& neighbor : neighbors) {
                    auto v = mapToLevel(22, 29.0909, neighbor);
                    if (!(v == Location(-1, -1))) {
                        found = true;
                        monster->moveTo(neighbor);
                        cout << "WOAH" << endl;
                    }
                }
                if (!found) {
                    cout << "FUCK" << endl;
                    exit(99);
                }
            }
            cout << "OK" << endl;
            search = new AStar(graph, monster->getLocation(), character->getLocation(), CustomHeuristic(character->getLocation()));
            path = search->search();
            path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
            cout << "DIDN'T GET HERE" << endl;
        }

        // If chasingIteration has complete, reset count. TODO: Probably bad way to do this.
        if (decision != chasing) {
            chasingIteration = 0;
            monster->stop();
            isChasing = false;
            cout << "STOPPED" << endl;
        }
        return true;
    }

    inline bool setupWandering() {
        if (wanderIteration > 200) {
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
            path.print();
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
                    cout << "YAY 1" << endl;
                    auto status = setupChasing(decision);
                    cout << "DAMMIT" << endl;
                    return status ? decision : generateAction(decision);
                }
            case wandering:
                {
                    cout << "YAY 2" << endl;
                    auto status = setupWandering();
                    cout << "DAMMIT" << endl;
                    return decision;
                }
            case guessing:
                {
                    cout << "YAY 3" << endl;
                    auto status = setupGuessing();
                    cout << "DAMMIT" << endl;
                    return decision;

                }
            default:
                {
                    cout << "YAY 4" << endl;
                    return decision;
                    cout << "DAMMIT" << endl;
                }
        }
    }

    inline float makeRandom(const Vector2f& p) {
        int offset = 15;
        int max = 0;
        int min = 0;
        if (p.x > p.y) {
            max = p.x + offset;
            min = p.y - offset;
        }
        else {
            max = p.y + offset;
            min = p.x - offset;
        }
        float x = float(rand() % (max - min + 1) + min);
        return x;
    }

    inline Vector2f makeGuess(const Vector2f& p) {
        float timer = 0.0;
        Vector2f guessPosition(makeRandom(p), makeRandom(p));
        cout << "WTF: " << guessPosition.x << " " << guessPosition.y << endl;
        while (mapToLevel(22, 29.0909, guessPosition) == Location(-1, -1)) {
            guessPosition = Vector2f(makeRandom(p), makeRandom(p));
            cout << "WTF: " << guessPosition.x << " " << guessPosition.y << endl;

            if (timer > 5.0) {
                return p;
            }
            timer += 0.1;
        }
        cout << "RAND: " << guessPosition.x << " " << guessPosition.y << endl;
        return guessPosition;
    }
    bool isChasing = false;

    MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_);

    inline void run() {
        cout << "K." << endl;
        auto decision = setupAction(root->run());
        cout << "K..." << endl;
        lastDecision = decision;
        switch (decision) {
            case chasing:
                {
                    cout << "CHASING" << endl;
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
                    cout << "GP: " << guessPosition.x << " " << guessPosition.y << endl;
                    monster->moveTo(guessPosition);
                    cout << "GP: " << monster->getPosition().x << " " << monster->getPosition().y << endl;
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

// TODO: Run inadmissible heuristic every time because character moves. 
       // You could adjust this to only recalibrate when character has changed an entire square (or two).
       // monster->stop();
       // search = new AStar(graph, monster->getLocation(), character->getLocation(), CustomHeuristic(character->getLocation()));
       // path = search->search();
       // path.print();
       // pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);

#endif