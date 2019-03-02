#include "CfgAlgoRunner.h"



CfgAlgoRunner::CfgAlgoRunner(QObject *parent/* = Q_NULLPTR*/)
	: CfgAlgoRunnerSimpleSource(parent)
{
}


CfgAlgoRunner::~CfgAlgoRunner()
{
}

void CfgAlgoRunner::add_rule(QChar from, QVector<QPair<bool, QString>> to)
{
	NonterminalSymbol f(from, from == 'S');
	std::vector<std::shared_ptr<GrammarSymbol>> t;
	t.reserve(to.size());
	for(const auto& el : to)
	{
		if(el.first)
		{
			t.push_back(std::make_shared<TerminalSymbol>(el.second[0]));
		}
		else
		{
			t.push_back(std::make_shared<NonterminalSymbol>(el.second[0]));
		}
	}
	cfg.AddRule(Rule(f , t));
}

void CfgAlgoRunner::find_shortest_words()
{
	setSolution(CFG::FindShortestWordsFromAllNonterminals(cfg));
	Q_EMIT solution_found();
}

void CfgAlgoRunner::clear()
{
	cfg = ContextFreeGrammar();
	setSolution({});
}
