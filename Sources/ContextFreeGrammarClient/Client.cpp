#include "Client.h"

#include <QDebug>

Client::Client(QSharedPointer<CfgAlgoRunnerReplica> ptr, QObject *parent/* = Q_NULLPTR*/)
	: QObject(parent)
	, ptr(ptr)
{
	InitConnections();
}

Client::~Client()
{
}

void Client::AddRule(QChar from, QVector<QPair<bool, QString>> to)
{
	Q_EMIT add_rule(from, to);
}

void Client::FindSortestWords()
{
	Q_EMIT find_shortest_words();
}

void Client::Clear()
{
	Q_EMIT clear();
}

void Client::solution_found()
{
	if (!ptr.data()->solution().isEmpty())
		Q_EMIT solution_ready(ptr.data()->solution());
}

void Client::InitConnections()
{
	bool is_connected = false;
	is_connected = connect(this, &Client::add_rule, ptr.data(), &CfgAlgoRunnerReplica::add_rule);
	Q_ASSERT(is_connected);
	is_connected = connect(this, &Client::find_shortest_words, ptr.data(), &CfgAlgoRunnerReplica::find_shortest_words);
	Q_ASSERT(is_connected);
	is_connected = connect(this, &Client::clear, ptr.data(), &CfgAlgoRunnerReplica::clear);
	Q_ASSERT(is_connected);
	is_connected = connect(ptr.data(), &CfgAlgoRunnerReplica::solutionChanged, this, &Client::solution_found);
	Q_ASSERT(is_connected);
}
