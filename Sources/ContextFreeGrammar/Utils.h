#pragma once
#include "cfg_api.h"

#include "ContextFreeGrammar.h"

#include <QPair>

class QStringList;

namespace CFG
{
	CONTEXTFREEGRAMMAR_EXPORT ContextFreeGrammar EliminateEpsilonRules(const ContextFreeGrammar& cfg);
	CONTEXTFREEGRAMMAR_EXPORT ContextFreeGrammar EliminateLeftRecursion(const ContextFreeGrammar& cfg);
	CONTEXTFREEGRAMMAR_EXPORT ContextFreeGrammar TransformToGreibach(const ContextFreeGrammar& cfg);
	CONTEXTFREEGRAMMAR_EXPORT std::vector<NonterminalSymbol> FindEpsilonNonterminals(const ContextFreeGrammar& cfg);
	CONTEXTFREEGRAMMAR_EXPORT QStringList FindShortestWords(const ContextFreeGrammar& cfg, size_t max_size = 1000);
	CONTEXTFREEGRAMMAR_EXPORT QList<QPair<QChar, QStringList>> FindShortestWordsFromAllNonterminals(const ContextFreeGrammar& cfg);

} //namespace CFG