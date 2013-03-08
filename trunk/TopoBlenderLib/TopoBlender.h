#pragma once

#include <QObject>
#include "DynamicGraph.h"
#include "StructureGraph.h"
#include "GraphDistance.h"
#include "GraphCorresponder.h"

class Scheduler;

struct Structure::Graph;
typedef QSet<Structure::Node*> SetNodes;

class TopoBlender : public QObject
{
    Q_OBJECT
public:
    explicit TopoBlender(Structure::Graph * graph1, Structure::Graph * graph2, GraphCorresponder * useCorresponder, Scheduler * useScheduler, QObject *parent = 0);
    
	Structure::Graph * sg;
    Structure::Graph * tg;
	GraphCorresponder * gcoor;
	Scheduler * scheduler;

	void oldSetup();

    QMap<QString, QVariant> params;

	/// Super graphs:
	Structure::Graph * super_sg;
	Structure::Graph * active;
	Structure::Graph * super_tg;
	QMap<QString, QString> superNodeCorr;
	QMap<QString, QString> superEdgeCorr;
	void generateSuperGraphs();
	void correspondSuperNodes();
	void correspondSuperEdges();

	/// Helper functions:
	QVector<QString> cloneGraphNode(Structure::Graph *g, QString nodeID, int N);
	Structure::Link * addMissingLink( Structure::Graph *g, Structure::Link * link );
	Structure::Node * addMissingNode( Structure::Graph *toGraph, Structure::Graph * fromGraph, Structure::Node * fromNode );
	void tagEdge(Structure::Link *link, QString tag);
	bool taggedEdge(Structure::Link *link, QString tag);
	QString correspondingNode( Structure::Link *link, int i );
	void correspondMissingEdges( Structure::Graph * sgraph, Structure::Graph * tgraph );

	void equalizeResolutions();

	// Query
	bool isExtraNode(Structure::Node *node);
	bool isExtraEdge(Structure::Link *link);
	bool isCorrespondedEdge(Structure::Link *link);
	bool isShareCorrespondedNode( Structure::Link * slink, Structure::Link * tlink );

	// Edge lists filtering
	QVector<Structure::Link*> edgesContain(QVector<Structure::Link*> edges, QString property_name);
	QVector<Structure::Link*> edgesNotContain(QVector<Structure::Link*> edges, QString property_name);
	
	// Null sets
	QVector< SetNodes > nullNodeSets( Structure::Graph * graph );
	void connectNullSet( SetNodes nullSet, Structure::Graph * source, Structure::Graph * target );
	QVector<Structure::Link*> nonCorrespondEdges( Structure::Node * node, Structure::Graph * graph );
	
	/// Tasks:
	void generateTasks();

	/// Logging
	int stepCounter;

public slots:
	void executeBlend();

// DEBUG:
public:
    std::vector< Vector3 > debugPoints;
    std::vector< PairVector3 > debugLines;
    void drawDebug();

signals:

};