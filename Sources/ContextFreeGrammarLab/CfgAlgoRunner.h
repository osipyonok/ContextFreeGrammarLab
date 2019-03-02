#pragma once
#include "rep_CfgAlgoRunner_source.h"

#include <ContextFreeGrammar/Utils.h>

#include <QObject>


class CfgAlgoRunner
	: public CfgAlgoRunnerSimpleSource
{
	Q_OBJECT
public:
	CfgAlgoRunner(QObject *parent = Q_NULLPTR);
	~CfgAlgoRunner();

	void add_rule(QChar, QVector<QPair<bool, QString>>) override;
	void find_shortest_words() override;
	void clear() override;

signals:
	void solution_found();

private:
	ContextFreeGrammar cfg;
};

