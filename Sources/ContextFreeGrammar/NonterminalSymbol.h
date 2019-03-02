#pragma once
#include "cfg_api.h"

#include "GrammarSymbol.h"

class CONTEXTFREEGRAMMAR_EXPORT NonterminalSymbol
	: public GrammarSymbol
{
public:
	NonterminalSymbol()
		: NonterminalSymbol('\0', false)
	{}

	NonterminalSymbol(NonterminalSymbol&& other)
		: GrammarSymbol { std::move(other)}
	{
		std::swap(m_is_start, other.m_is_start);
	}

	NonterminalSymbol(const NonterminalSymbol& other) = default;

	explicit NonterminalSymbol(QChar chr, bool is_start = false)
		: GrammarSymbol { chr }
		, m_is_start { is_start }
	{}

	~NonterminalSymbol() override = default;

	NonterminalSymbol& operator=(const NonterminalSymbol&) = default;
	NonterminalSymbol& operator=(NonterminalSymbol&&) = default;

	bool IsTerminal() const override
	{
		return false;
	}

	bool IsStart() const
	{
		return m_is_start;
	}

private:
	bool m_is_start;
};