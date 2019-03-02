#pragma once

#include "cfg_api.h"

#include "Rule.h"

class CONTEXTFREEGRAMMAR_EXPORT ContextFreeGrammar
{
public:
	ContextFreeGrammar()
		: ContextFreeGrammar(NonterminalSymbol('S', true))
	{
		m_nonterminals.push_back(m_start);
	}

	explicit  ContextFreeGrammar(NonterminalSymbol start)
		: m_start(start)
	{}

	ContextFreeGrammar(ContextFreeGrammar&& other)
	{
		std::swap(m_start, other.m_start);
		m_rules.swap(other.m_rules);
		m_nonterminals.swap(other.m_nonterminals);
		m_terminals.swap(other.m_terminals);
	}

	ContextFreeGrammar(const ContextFreeGrammar&) = default;
	ContextFreeGrammar& operator=(const ContextFreeGrammar&) = default;
	ContextFreeGrammar& operator=(ContextFreeGrammar&&) = default;

	void AddRule(Rule rule)
	{
		m_rules.push_back(rule);

		if (m_nonterminals.end() == std::find(m_nonterminals.begin(), m_nonterminals.end(), rule.GetFrom()))
			m_nonterminals.push_back(rule.GetFrom());

		std::for_each(rule.GetTo().begin(), rule.GetTo().end(), [&](std::shared_ptr<GrammarSymbol> symbol)
		{
			if (symbol->IsTerminal())
			{
				if (m_terminals.end() == std::find(m_terminals.begin(), m_terminals.end(), *symbol))
					m_terminals.push_back(dynamic_cast<TerminalSymbol&>(*symbol));
			}
			else
			{
				if (m_nonterminals.end() == std::find(m_nonterminals.begin(), m_nonterminals.end(), *symbol))
					m_nonterminals.push_back(dynamic_cast<NonterminalSymbol&>(*symbol));
			}
		});
	}

	std::vector<Rule> const& GetRules() const
	{
		return m_rules;
	}

	NonterminalSymbol GetStartSymbol() const
	{
		return m_start;
	}

	void SetStartSymbol(const NonterminalSymbol& symbol)
	{
		m_start = symbol;
		if (m_nonterminals.end() == std::find(m_nonterminals.begin(), m_nonterminals.end(), symbol))
			m_nonterminals.push_back(symbol);
	}

	std::vector<NonterminalSymbol> const& GetNonterminalSymbols() const
	{
		return m_nonterminals;
	}

	std::vector<TerminalSymbol> const& GetTerminalSymbols() const
	{
		return m_terminals;
	}

private:
	NonterminalSymbol m_start;
	std::vector<Rule> m_rules;
	std::vector<NonterminalSymbol> m_nonterminals;
	std::vector<TerminalSymbol> m_terminals;
};
