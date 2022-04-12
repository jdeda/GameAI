#include <vector>
#include "behaviortree.h"

using namespace std;


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
MonsterBehaviorTree::MonsterBehaviorTree(const Graph& graph_, Character* character_, Character* monster_, float* dt_, bool* monsterClose_) : graph(graph_) {
	dt = dt_;
	character = character_;
	monster = monster_;
	monsterClose = monsterClose_;
	search = new AStar(graph, monster->getLocation(), monster->getLocation(), ManhattanHeuristic(monster->getLocation()));
	pathFollowing = new FollowPath(path, PATH_OFFSET, 0, PREDICTION_TIME, TIME_TO_REACH_TARGET_SPEED, RADIUS_OF_ARRIVAL, RADIUS_OF_DECELERATION, MAX_SPEED);

	// Create tasks for monster.
	MonsterTask* t0 = new MonsterChasing(character->getLocation(), monster->getLocation());
	MonsterTask* t1 = new MonsterWandering(character->getLocation(), monster->getLocation());
	MonsterTask* t2 = new MonsterGuessing(character->getLocation(), monster->getLocation());

	// Create selector.
	vector<MonsterTask*> selectorChildren;
	MonsterTask* selector = new MonsterSelector(selectorChildren);

	// Create random selector.
	vector<MonsterTask*> randomSelectorChildren;
	MonsterTask* randomSelector = new MonsterRandomSelector(randomSelectorChildren);

	// Create sequence.
	vector<MonsterTask*> sequenceChildren;
	MonsterTask* sequence = new MonsterSequence(sequenceChildren);

	// Combine and create root.
	selectorChildren.push_back(sequence);
	selectorChildren.push_back(randomSelector);
	sequenceChildren.push_back(t0);


	root = selector;
	
	// Sequence...if a&&b&&c...act.
	// Sequence in selector = if else


	//
	// root = new MonsterSelector();
	// MonsterTask* t1 = new MonsterChasing();
	// root = 

		// enemy_bt = new BehaviorTree(world_objects, enemy);

		// Task a0 = new Selector(enemy, "Not Acting?");
		// 	Question b0 = new Selector(enemy, "Near Enemy?");
		// 		Task c0 = new Sequence(enemy, "Kill Enemy");
		// 			Task d0 = new PerformAction(enemy, "Pursue Enemy");
		// 			Question d3 = new Sequence(enemy, "Very close to enemy?");
		// 				Task e2 = new PerformAction(enemy, "Arrive at enemy");
		// 	Question b1 = new Selector(enemy, "Near TV?");
		// 		Question c1 = new Sequence(enemy, "Can act again?");
		// 			Task d1 = new PerformAction(enemy, "Pathfind to TV");
		// 			Question d2 = new RandomSelector(enemy, "Finish Pathfinding to TV?");
		// 				Task e0 = new PerformAction(enemy, "Sit");
		// 				Task e1 = new PerformAction(enemy, "Dance");
		// 	Question b2 = new Selector(enemy, "Not near TV or Player?");
		// 		Task c4 = new PerformAction(enemy, "Wander");

		// // Add behavior nodes
		// enemy_bt.root = a0;
		// a0.addChild(b0);
		// a0.addChild(b1);
		// a0.addChild(b2);

		// b0.addChild(c0);
		// b1.addChild(c1);
		// b2.addChild(c4);

		// c0.addChild(d0);
		// c0.addChild(d3);
		// c1.addChild(d1);
		// c1.addChild(d2);

		// d2.addChild(e0);
		// d2.addChild(e1);
		// d3.addChild(e2);

};
