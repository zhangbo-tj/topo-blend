#include "Relink.h"
#include "Scheduler.h"

Relink::Relink(Scheduler *scheduler)
{
    this->s = scheduler;
    this->activeGraph = scheduler->activeGraph;
    this->targetGraph = scheduler->targetGraph;
}

void Relink::execute(int globalTime)
{
	// initial
	constraints.clear();
	propagationQueue.clear();
	foreach (Task* t, s->tasks) 
	{
		t->property["relinked"] = false;
		t->property["propagated"] = false;
	}

	// Set up starting nodes
	QVector<QString> activeNodeIDs = activeGraph->property["activeTasks"].value< QVector<QString> >();
	foreach (QString nID, activeNodeIDs)
	{
		Task* task = s->getTaskFromNodeID(nID);

		if (isRelinkable(task))
		{
			propagationQueue.enqueue(task);
			task->property["propagated"] = true;
		}
	}

	// propagating
	while(!propagationQueue.isEmpty())
	{
		Task* task = propagationQueue.dequeue();

		// Fix task according to constraints
		fixTask(task, globalTime);

		// Propagate from task
		propagateFrom(task);
	}
}


void Relink::propagateFrom( Task* task )
{
	Structure::Node * n = task->node();
	QVector<Structure::Link*> edges = activeGraph->getEdges(n->id);

	foreach(Structure::Link* link, edges)
	{
		Structure::Node * other = link->otherNode(n->id);
		Task * otherTask = s->getTaskFromNodeID(other->id);

		// Skip un-relinkable tasks
		if (!isRelinkable(otherTask)) continue;

		// Add to queue
		if (!otherTask->property["propagated"].toBool())
		{
			propagationQueue.enqueue(otherTask);
			otherTask->property["propagated"] = true;
		}

		// generate constrains to unfixed task
		if (!otherTask->property["relinked"].toBool())
			constraints[ otherTask ].push_back( LinkConstraint(link, task, otherTask) );
	}
}


void Relink::fixTask( Task* task, int globalTime )
{
	task->property["relinked"] = true;

	Structure::Node * n = task->node();

	QVector<LinkConstraint> consts;
	if (constraints.contains(task)) consts = constraints[task];

	int N = consts.size();

	// Translate done tasks
	if (task->isDone && N > 0)
	{
		// Use all constraints
		Vec3d translation(0);
		foreach(LinkConstraint c, consts)
		{
			Structure::Link * link = c.link;

			Vec3d oldPos = link->position(n->id);
			Vec3d linkPosOther = link->positionOther(n->id);
			Vec3d delta = getToDelta(link, n->id);
			Vector3 newPos = linkPosOther + delta;

			translation += newPos - oldPos;
		}
		n->moveBy(translation / N);
		
		// Visualization
		foreach(LinkConstraint c, consts)
		{
			Structure::Link* link = c.link;
			Vector3 linkPosOther = link->positionOther(n->id);
			Vec3d delta = getToDelta(link, n->id);

			activeGraph->vs2.addVector( linkPosOther, delta );
		}

	}
	// Translate future tasks if only one constraint
	else if( N == 1 )
	{
		LinkConstraint constraint = consts.front();
		Structure::Link * link = constraint.link;

		Vec3d oldPos = link->position(n->id);
		Vec3d linkPosOther = link->positionOther(n->id);
		Vec3d delta = getToDelta(link, n->id);
		Vector3 newPos = linkPosOther + delta;

		// Translate
		n->moveBy(newPos - oldPos);

		// Visualization
		activeGraph->vs2.addVector( linkPosOther, delta );
	}
	// Deform future tasks by two handles if multiple constrains exist
	else if (N > 1)
	{
		// Pickup two constrains to deform the node
		LinkConstraint cA = consts.front();
		LinkConstraint cB = consts.back();
		Structure::Link *linkA = cA.link, *linkB = cB.link;

		// Two Handles and newPos:
		Vec4d handleA = linkA->getCoord(n->id).front();
		Vector3 linkPosOtherA = linkA->positionOther(n->id);
		Vector3 deltaA = getToDelta(linkA, n->id);
		Vector3 newPosA = linkPosOtherA + deltaA;

		Vec4d handleB = linkB->getCoord(n->id).front();
		Vector3 linkPosOtherB = linkB->positionOther(n->id);
		Vector3 deltaB = getToDelta(linkB, n->id);
		Vector3 newPosB = linkPosOtherB + deltaB;

		// Deform two handles
		n->deformTwoHandles(handleA, newPosA, handleB, newPosB);

		// Visualization
		activeGraph->vs2.addVector( linkPosOtherA, deltaA );
		activeGraph->vs2.addVector( linkPosOtherB, deltaB );
	}

	// Visualization
	foreach(LinkConstraint c, consts)
	{
		Structure::Link* link = c.link;
		Vector3 linkPosOther = link->positionOther(n->id);
		Vec3d delta = getToDelta(link, n->id);

		activeGraph->vs.addVector( linkPosOther, delta );
	}
}

Vector3 Relink::getToDelta( Structure::Link * link, QString toOtherID )
{
	// by default delta = n2 - n1
	Vector3 delta = link->property["blendedDelta"].value<Vector3>();
	if(link->n1->id == toOtherID) delta *= -1;
	return delta;
}


bool Relink::isRelinkable(Task* task)
{
	// Shrink/Grow cut node 
	if (task->type != Task::MORPH && task->property["isCutNode"].toBool()) return true;

	// Shrunk node 
	if (task->node()->property["shrunk"].toBool()) return false;

	// Not fully grown
	if (task->type == Task::GROW && !task->isDone) return false;

	// Grow/shrink and crossing morph
	if (task->type != Task::MORPH) return false;
	if (task->property["isCrossing"].toBool() && !task->isDone) return false;

	return true;
}