#pragma once
#include "cfg_api.h"

#include "GrammarSymbol.h"


class CONTEXTFREEGRAMMAR_EXPORT TerminalSymbol
	: public GrammarSymbol
{
public:
	explicit TerminalSymbol(QChar chr)
		: GrammarSymbol { chr }
	{}

	TerminalSymbol(TerminalSymbol&& other)
		: GrammarSymbol { std::move(other) }
	{}

	TerminalSymbol(const TerminalSymbol& other) = default;

	TerminalSymbol& operator=(const TerminalSymbol&) = default;
	TerminalSymbol& operator=(TerminalSymbol&&) = default;

	~TerminalSymbol() override = default;

	bool IsTerminal() const override
	{
		return true;
	}
};
