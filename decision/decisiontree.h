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
        if (trueNode == NULL && falseNode == NULL) {
            cout << "made a decision:" << action << endl;
            return CharacterDecisionNode(action, NULL, NULL);
        }

        cout << "making decision: " << monsterClose << endl;
        cout << "for action: " << action << endl;
        switch (action) {
            case CharacterAction::followClick:
                {
                    if (followingPath) {
                        return trueNode->makeDecision(monsterClose, followingPath);
                    }
                    else {
                        return falseNode->makeDecision(monsterClose, followingPath);
                    }
                }
            case CharacterAction::escaping:
                {
                    if (monsterClose) {
                        cout << "YAY!" << endl;
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
    int escapingIteration = 0;

    /** escaping functionality*/
    Vector2f escapeEndPoint = Vector2f(1, 1);

    public:
    CharacterDecisionTree(const Graph& graph, Character* character_, Location* mouse_, float* dt, bool* monsterClose_, bool* followingPath_);

    inline float makeRandom(const Vector2f& p) {
        int offset = 200;
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

        // int max = 600;
        // int min = 30;
        // float x = float(rand() % (max - min + 1) + min);
        // return x;
    }

    inline Vector2f makeGuess(const Vector2f& p) {
        float timer = 0.0;
        Vector2f guessPosition(makeRandom(p), makeRandom(p));
        while (mapToLevel(22, 29.0909, guessPosition) == Location(-1, -1)) {
            guessPosition = Vector2f(makeRandom(p), makeRandom(p));
            if (timer > 5.0) {
                cout << "BAD" << endl;
                exit(1);
                return p;
            }
            timer += 0.1;
        }
        return guessPosition;
    }

    inline void makeDecision() {
        cout << "CLOSE? " << *monsterClose << endl;
        CharacterAction action = root->makeDecision(*monsterClose, *followingPath).getAction();

        // If first iteration of followingClick, find path.
        cout << "IT: " << followingIteration << endl;
        if (action == followClick && followingIteration == 0) {
            search = new AStar(graph, character->getLocation(), *mouseLocation, ManhattanHeuristic(*mouseLocation));
            path = search->search();
            path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
        }
        // If first iteration of escape, find path.
        if (action == escaping && followingIteration == 0) {
            *followingPath = true;
            cout << "HERE WE ARE" << endl;
            cout << "START: " << character->getLocation().x << " " << character->getLocation().y << endl;
            escapeEndPoint = makeGuess(character->getPosition());
            Location escapeEndPointLocation = mapToLevel(22, 29.0909, escapeEndPoint);
            cout << "END: " << escapeEndPointLocation.x << " " << escapeEndPointLocation.y << endl;
            search = new AStar(graph, character->getLocation(), escapeEndPointLocation, CustomHeuristic(escapeEndPointLocation));
            path = search->search();
            path.print();
            pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);
        }

        // If followingClick has complete, reset count.
        if (*followingPath == false) {
            followingIteration = 0;
        }

        switch (action) {
            case CharacterAction::followClick:
                {
                    // TODO: path finding broken...
                    // TODO: Sometimes gets stuck...
                    // Need to fix start position error...
                    // Apply path following to click.
                    // TODO: Keeps accelerating even if out of bounds...because velocity neeeds to be set to 0... kinda hacky
                    SteeringOutput pathAccelerations = pathFollowing->calculateAcceleration(character->getKinematic(), Kinematic());
                    if (pathAccelerations.linearAcceleration == Vector2f(-1.f, -1.f)) {
                        character->stop(); // TODO: more hacking to be put here.
                    }
                    else {
                        character->update(pathAccelerations, *dt, true);
                    }
                    followingIteration += 1;
                    break;
                }
            case CharacterAction::escaping:
                {
                    cout << "ESCAPING" << endl;
                    SteeringOutput pathAccelerations = pathFollowing->calculateAcceleration(character->getKinematic(), Kinematic());
                    if (pathAccelerations.linearAcceleration == Vector2f(-1.f, -1.f)) {
                        character->stop(); // TODO: more hacking to be put here.
                    }
                    else {
                        character->update(pathAccelerations, *dt, true);
                    }
                    followingIteration += 1;
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