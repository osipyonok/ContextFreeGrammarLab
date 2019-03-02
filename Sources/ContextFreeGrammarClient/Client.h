#pragma once

#include "rep_CfgAlgoRunner_replica.h"

#include <QObject>
#include <QSharedPointer>

class Client
	: public QObject
{
	Q_OBJECT
public:
	Client(QSharedPointer<CfgAlgoRunnerReplica> ptr, QObject *parent = Q_NULLPTR);
	~Client();

	void AddRule(QChar, QVector<QPair<bool, QString>>);
	void FindSortestWords();
	void Clear();


signals:
	void add_rule(QChar, QVector<QPair<bool, QString>>);
	void find_shortest_words();
	void clear();
	void solution_ready(QList<QPair<QChar, QStringList>>);

public slots:
	void solution_found();

private:
	void InitConnections();

	QSharedPointer<CfgAlgoRunnerReplica> ptr;
};

