#include <vector>
#include "behaviortree.h"

using namespace std;

MonsterNearby::MonsterNearby(const Location& monsterLocation_, const Location& enemyLocation_) :
	monsterLocation(monsterLocation_), enemyLocation(enemyLocation_) {
}
MonsterChasing::MonsterChasing(const Location& monsterLocation_, const Location& enemyLocation_) :
	monsterLocation(monsterLocation_), enemyLocation(enemyLocation_) {
}
MonsterWandering::MonsterWandering(const Location& monsterLocation_, const Location& enemyLocation_) :
	monsterLocation(monsterLocation_), enemyLocation(enemyLocation_) {
}
MonsterGuessing::MonsterGuessing(const Location& monsterLocation_, const Location& enemyLocation_) :
	monsterLocation(monsterLocation_), enemyLocation(enemyLocation_) {
}
MonsterSequence::MonsterSequence(const vector<MonsterTask*>& children_) {
	children = children_;
}
MonsterSelector::MonsterSelector(const vector<MonsterTask*>& children_) {
	children = children_;
}
MonsterRandomSelector::MonsterRandomSelector(const vector<MonsterTask*>& children_) {
	children = children_;
}
MonsterBehaviorTree::MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_) : graph(graph_) {
	dt = dt_;
	character = character_;
	monster = monster_;
	search = new AStar(graph, monster->getLocation(), monster->getLocation(), ManhattanHeuristic(monster->getLocation()));
	pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);

	// Create sequence branch.
	vector<MonsterTask*> sequenceChildren;
	MonsterTask* isNearby = new MonsterNearby(character->getLocation(), monster->getLocation());
	MonsterTask* chase = new MonsterChasing(character->getLocation(), monster->getLocation());
	sequenceChildren.push_back(isNearby);
	sequenceChildren.push_back(chase);
	MonsterTask* sequence = new MonsterSequence(sequenceChildren);

	// Create random selector branch.
	vector<MonsterTask*> randomSelectorChildren;
	MonsterTask* wander = new MonsterWandering(character->getLocation(), monster->getLocation());
	MonsterTask* guess = new MonsterGuessing(character->getLocation(), monster->getLocation());
	randomSelectorChildren.push_back(guess);
	randomSelectorChildren.push_back(wander);
	MonsterTask* randomSelector = new MonsterRandomSelector(randomSelectorChildren);

	// Create selector branch (root).
	vector<MonsterTask*> selectorChildren;
	selectorChildren.push_back(sequence);
	selectorChildren.push_back(randomSelector);
	MonsterTask* selector = new MonsterSelector(selectorChildren);
	root = selector;
};