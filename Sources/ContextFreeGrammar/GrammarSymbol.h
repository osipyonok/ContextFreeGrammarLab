#pragma once
#include "cfg_api.h"

#include <QChar>

class CONTEXTFREEGRAMMAR_EXPORT GrammarSymbol
{
protected:
	explicit GrammarSymbol(QChar chr)
		: m_char{ chr }
	{}

	GrammarSymbol(GrammarSymbol&& other)
	{
		std::swap(m_char, other.m_char);
	}

	GrammarSymbol(const GrammarSymbol& other) = default;

	GrammarSymbol& operator=(const GrammarSymbol&) = default;
	GrammarSymbol& operator=(GrammarSymbol&&) = default;

public:
	QChar GetChar() const
	{
		return m_char;
	}

	virtual ~GrammarSymbol() = default;

	virtual bool IsTerminal() const = 0;
	bool IsNonterminal() const
	{
		return !IsTerminal();
	}

	friend bool operator==(const GrammarSymbol& a, const GrammarSymbol& b)
	{
		return a.GetChar() == b.GetChar() && a.IsTerminal() == b.IsTerminal();
	}

	friend bool operator<(const GrammarSymbol& a, const GrammarSymbol& b)
	{
		return  a.GetChar() < b.GetChar();
	}

private:
	QChar m_char;
};
