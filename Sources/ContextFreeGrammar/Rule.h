#pragma once
#include "cfg_api.h"

#include "TerminalSymbol.h"
#include "NonterminalSymbol.h"

#include <iostream>
#include <vector>
#include <memory>

class CONTEXTFREEGRAMMAR_EXPORT Rule
{
public:
	explicit Rule(NonterminalSymbol from, std::vector<std::shared_ptr<GrammarSymbol>> to = {})
		: m_from { std::move(from) }
		, m_to { std::move(to) }
	{}

	NonterminalSymbol GetFrom() const
	{
		return m_from;
	}

	std::vector<std::shared_ptr<GrammarSymbol>> const& GetTo() const
	{
		return m_to;
	}

	template<typename TOutoutStream>
	friend TOutoutStream& operator<<(TOutoutStream& out, const Rule& rule)
	{
		out << rule.GetFrom().GetChar() << " ->";
		if(rule.GetTo().empty())
		{
			out << "EPS";
		}
		else
		{
			for (const auto& el : rule.GetTo())
				out << ' ' << el->GetChar();
		}
		return out;
	}

private:
	NonterminalSymbol m_from;
	std::vector<std::shared_ptr<GrammarSymbol>> m_to;
};
