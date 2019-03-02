#include "Utils.h"

#include <QStringList>
#include <QPair>
#include <QDebug>

#include <set>
#include <functional>
#include <queue>
#include <unordered_map>

using Word = std::vector<std::shared_ptr<GrammarSymbol>>;

namespace
{
	size_t _Norm(const Word& a)
	{
		return std::count_if(a.begin(), a.end(), [](std::shared_ptr<GrammarSymbol> symbol) { return symbol->IsTerminal(); });
	}

	bool _Compare(const Word& a, const Word& b)
	{
		const auto norm1 = _Norm(a);
		const auto norm2 = _Norm(b);

		if (norm1 != norm2)
			return norm1 < norm2;

		const auto inv1 = a.size() - norm1;
		const auto inv2 = b.size() - norm2;

		if (inv1 != inv2)
			return inv1 < inv2;

		return a < b;
	}

	bool _IsFinal(const Word& a)
	{
		return a.size() == _Norm(a);
	}

	bool _AreEquals(std::pair<QChar,Word> const& a, std::pair<QChar, Word> const& b)
	{
		if(a.first != b.first)
			return false;

		if (a.second.size() != b.second.size())
			return false;

		for(size_t i = 0; i < a.second.size(); ++i)
		{
			if(*(a.second[i]) == *(b.second[i]))
				continue;
			return false;
		}

		return true;
	}

	std::vector<NonterminalSymbol> _GetNonterminalsOrder(const ContextFreeGrammar &cfg)
	{
		std::vector<std::vector<size_t>> dag(cfg.GetNonterminalSymbols().size());
		std::vector<std::vector<size_t>> rdag(cfg.GetNonterminalSymbols().size());
		std::map<NonterminalSymbol, size_t> order;
		std::map<size_t, NonterminalSymbol> inv_order;
		for(size_t i = 0; i < cfg.GetNonterminalSymbols().size(); ++i)
		{
			order[cfg.GetNonterminalSymbols()[i]] = i;
			inv_order[i] = cfg.GetNonterminalSymbols()[i];
		}

		for(const auto& rule : cfg.GetRules())
		{
			if (rule.GetTo().empty())
				continue;
			if (rule.GetTo()[0]->IsTerminal())
				continue;

			dag[order[rule.GetFrom()]].push_back(order[dynamic_cast<const NonterminalSymbol&>(*rule.GetTo()[0])]);
			rdag[order[dynamic_cast<const NonterminalSymbol&>(*rule.GetTo()[0])]].push_back(order[rule.GetFrom()]);
		}

		std::unordered_map<size_t, size_t> res;
		for (size_t i = 0; i < cfg.GetNonterminalSymbols().size(); ++i) res[i] = static_cast<size_t>(-1);
		size_t num = cfg.GetNonterminalSymbols().size();
		std::function<void(size_t)> impl = [&](size_t cur)
		{
			res[cur] = num++;
			for(const auto& to : dag[cur])
			{
				if(static_cast<size_t>(-1) == res[to])
				{
					impl(to);
				}
				res[cur] = std::min(res[cur], res[to] - 1);
			}
		};

		for(const auto& symbol : cfg.GetNonterminalSymbols())
		{
			if (rdag[order[symbol]].empty() && static_cast<size_t>(-1) == res[order[symbol]])
				impl(order[symbol]);
		}

		std::vector<NonterminalSymbol> solution(cfg.GetNonterminalSymbols().size(), NonterminalSymbol('\0'));

		{
			std::vector<std::pair<size_t, NonterminalSymbol>> v;
			v.reserve(cfg.GetNonterminalSymbols().size());
			for(const auto& el : res)
			{
				v.push_back({ el.second , inv_order[el.first] });
			}
			std::sort(v.begin(), v.end());

			for(size_t i = 0; i < cfg.GetNonterminalSymbols().size(); ++i)
			{
				solution[i] = v[i].second;
			}
		}
		
		/*for(size_t i = 0; i < cfg.GetNonterminalSymbols().size(); ++i)
		{
			for(const auto& v : order)
			{
				if(v.second == i)
				{
					solution[res[i]] = v.first;
				}
			}
		}*/

		return std::move(solution);
	}
}

namespace CFG
{
	ContextFreeGrammar EliminateEpsilonRules(const ContextFreeGrammar& cfg)
	{
		auto eps = CFG::FindEpsilonNonterminals(cfg);

		std::vector < std::pair<QChar, Word>> rules;
		for(const auto& el : cfg.GetRules())
		{
			rules.emplace_back( el.GetFrom().GetChar() , el.GetTo() );
		}

		for(size_t i = 0; i < cfg.GetRules().size(); ++i)
		{
			std::vector<size_t> cur_eps_idx;
			for(size_t j = 0; j < rules[i].second.size(); ++j)
			{
				if(rules[i].second[j]->IsTerminal())
					continue;

				if (eps.end() == std::find(eps.begin(), eps.end(), dynamic_cast<NonterminalSymbol&>(*rules[i].second[j])))
					continue;

				cur_eps_idx.push_back(j);
			}

			for(size_t mask = 0; mask < (1 << cur_eps_idx.size()); ++mask)
			{
				Word current_right_part;
				size_t todo = 0;
				for(size_t j = 0; j < rules[i].second.size(); ++j)
				{
					if(todo < cur_eps_idx.size() && ((1 << todo) & mask) && cur_eps_idx[todo] == j)
					{
						++todo;
						continue;
					}

					if (todo < cur_eps_idx.size() && cur_eps_idx[todo] == j)
						++todo;

					current_right_part.push_back(rules[i].second[j]);
				}

				rules.emplace_back(rules[i].first, current_right_part);
			}
		}

		ContextFreeGrammar cfg_without_eps;
		cfg_without_eps.SetStartSymbol(cfg.GetStartSymbol());
		
		for(size_t i = 0; i < rules.size(); ++i)
		{
			if(rules[i].second.empty())
				continue;
			bool was = false;
			for(size_t j = 0; j < i; ++j)
			{
				if(_AreEquals(rules[i], rules[j]))
				{
					was = true;
					break;
				}
			}
			if (!was)
				cfg_without_eps.AddRule(Rule{ NonterminalSymbol(rules[i].first,
					rules[i].first == cfg.GetStartSymbol().GetChar()) , rules[i].second });
		}

		return std::move(cfg_without_eps);
	}

	ContextFreeGrammar EliminateLeftRecursion(const ContextFreeGrammar& cfg)
	{
		auto grammar = EliminateEpsilonRules(cfg);

		std::vector<NonterminalSymbol> nonterminals(grammar.GetNonterminalSymbols());
		{
			std::map<NonterminalSymbol, size_t> freq;
			for(const auto& rule : grammar.GetRules())
			{
				if (rule.GetTo().empty())
					continue;
				if (rule.GetTo()[0]->IsTerminal())
					continue;

				++freq[dynamic_cast<const NonterminalSymbol&>(*rule.GetTo()[0])];
			}
			std::sort(nonterminals.begin(), nonterminals.end(), [&freq](auto a, auto b)
			{
				return freq[a] < freq[b];
			});
		}

		ushort can = 0;
		for (const auto& el : nonterminals)
			can = std::max(can, el.GetChar().unicode());
		++can;

		std::vector<std::pair<QChar,Word>> rules;
		for(const auto& rule : grammar.GetRules())
		{
			rules.emplace_back( rule.GetFrom().GetChar() , rule.GetTo() );
		}

		for(size_t i = 0; i < nonterminals.size(); ++i)
		{
			qDebug() << QString("i = %1").arg(i);
			for(size_t j = 0; j < i; ++j)
			{
				qDebug() << QString("j = %1").arg(j);
				for(size_t p = 0; p < rules.size(); ++p)
				{
					qDebug() << QString("p = %1, size is %2").arg(p).arg(rules.size());
					
					if(rules[p].first != nonterminals[i].GetChar())
						continue;
					if(rules[p].second.empty())
						continue;
					if(!(*rules[p].second[0] == nonterminals[j]))
						continue;

					Word gamma(rules[p].second.begin() + 1, rules[p].second.end());
					rules.erase(rules.begin() + p);
					--p;

					for(size_t pp = 0; pp < rules.size(); ++pp)
					{
						if (rules[pp].first != nonterminals[j].GetChar())
							continue;
						if (rules[pp].second.empty())
							continue;

						Word x(rules[pp].second.begin(), rules[pp].second.end());
						std::copy(gamma.begin(), gamma.end(), std::back_inserter(x));

						rules.emplace_back(nonterminals[i].GetChar(), x);
					}

				}
			}

			qDebug() << "Current rules set:";
			for (const auto& r : rules)
			{
				qDebug() << Rule(NonterminalSymbol(r.first), r.second);
			}

			auto new_nonterminal = std::make_shared<NonterminalSymbol>(QChar(can));
			++can;
			std::vector<Word> alpha, beta;
			for(size_t p = 0; p < rules.size(); ++p)
			{
				if(rules[p].first != nonterminals[i].GetChar() || rules[p].second.empty())
					continue;
				
				if (*rules[p].second[0] == nonterminals[i])
				{
					auto it = std::find_if(rules[p].second.begin(), rules[p].second.end(), [nt = nonterminals[i]](std::shared_ptr<GrammarSymbol> symbol)
					{
						return *symbol == nt;
					});
					alpha.push_back({ it, rules[p].second.end() });
					/*if (alpha.back().empty())
						alpha.pop_back();
					else
						*/alpha.back().push_back(new_nonterminal);
					rules.erase(rules.begin() + p);
					--p;
				}
				else
				{
					beta.push_back(rules[p].second);
					beta.back().push_back(new_nonterminal);
				}
			}

			if(alpha.empty() || beta.empty())
			{
				--can;
				continue;
			}

			for(auto& b : beta)
			{
				rules.emplace_back(nonterminals[i].GetChar(), b);
				b.pop_back();
				rules.emplace_back(nonterminals[i].GetChar(), b);
			}

			for(auto& a : alpha)
			{
				rules.emplace_back(new_nonterminal->GetChar(), a);
				a.pop_back();
				rules.emplace_back(new_nonterminal->GetChar(), a);
			}
		}

		ContextFreeGrammar cfg_without_left_recursion;
		cfg_without_left_recursion.SetStartSymbol(cfg.GetStartSymbol());

		std::set<NonterminalSymbol> used_in_right_side;
		for(const auto& rule : rules)
		{
			for(const auto& symbol : rule.second)
			{
				if(symbol->IsNonterminal())
				{
					used_in_right_side.insert(dynamic_cast<const NonterminalSymbol&>(*symbol));
				}
			}
		}

		for (size_t i = 0; i < rules.size(); ++i)
		{
			if (rules[i].second.empty())
				continue;
			if(rules[i].first != cfg.GetStartSymbol().GetChar() && used_in_right_side.find(NonterminalSymbol(rules[i].first)) == used_in_right_side.end())
				continue;

			bool was = false;
			for (size_t j = 0; j < i; ++j)
			{
				if (_AreEquals(rules[i], rules[j]))
				{
					was = true;
					break;
				}
			}
			if (!was)
				cfg_without_left_recursion.AddRule(Rule{ NonterminalSymbol(rules[i].first,
					rules[i].first == cfg.GetStartSymbol().GetChar()) , rules[i].second });
		}

		return std::move(cfg_without_left_recursion);
	}

	ContextFreeGrammar TransformToGreibach(const ContextFreeGrammar& cfg)
	{
		auto grammar = EliminateLeftRecursion(cfg);

		{
			qDebug() << "Without left rec:";
			for (auto& r : grammar.GetRules())
				qDebug() << r;
			qDebug() << ' ';
		}

		std::map<NonterminalSymbol, size_t> order;
		std::map<size_t, NonterminalSymbol> inv_order;
		{
			auto _order = _GetNonterminalsOrder(grammar);
			for(size_t i = 0; i < _order.size(); ++i)
			{
				order[_order[i]] = i;
				inv_order[i] = _order[i];
			}
		}

		auto comp = [&order](const auto& a, const auto& b) { return order[a] < order[b]; };

		std::map<NonterminalSymbol, std::vector<Word>, decltype(comp)> rules(comp);
		for (const auto& rule : grammar.GetRules())
		{
			rules[rule.GetFrom()].push_back(rule.GetTo());
		}

		int n = static_cast<int>(rules.size());
		for(int i = n - 1; i >= 0; --i)
		{
			for(int j = i + 1; j < n; ++j)
			{
				size_t cnt = rules[inv_order[i]].size();
				for(size_t p = 0, d = 0; p - d < cnt; ++p)
				{
					auto cur = rules[inv_order[i]][p - d];
					if (rules[inv_order[i]][p - d][0]->IsTerminal())
						continue;
					if (order[dynamic_cast<const NonterminalSymbol&>(*rules[inv_order[i]][p - d][0])] != j)
						continue;

					Word w(rules[inv_order[i]][p - d].begin() + 1, rules[inv_order[i]][p - d].end());
					rules[inv_order[i]].erase(rules[inv_order[i]].begin() + p - d);
					++d;
					if (cnt > 0) --cnt;
					for(const auto& pref : rules[inv_order[j]])
					{
						Word ww(pref);
						std::copy(w.begin(), w.end(), std::back_inserter(ww));
						rules[inv_order[i]].push_back(ww);
					}
				}
			}
		}

		ContextFreeGrammar cfg_in_greibach_form;
		cfg_in_greibach_form.SetStartSymbol(cfg.GetStartSymbol());

		for(const auto& el : rules)
		{
			for(const auto& r : el.second)
			{
				cfg_in_greibach_form.AddRule(Rule(el.first, r));
			}
		}

		return std::move(cfg_in_greibach_form);
	}

	std::vector<NonterminalSymbol> FindEpsilonNonterminals(const ContextFreeGrammar& cfg)
	{
		std::map<NonterminalSymbol, bool> is_eps;
		std::map<NonterminalSymbol, std::vector<size_t>> concerned_rules;
		std::vector<size_t> cnt(cfg.GetRules().size());
		std::queue<NonterminalSymbol> q;

		size_t n_rules = cfg.GetRules().size();
		for(size_t i = 0; i < n_rules; ++i)
		{
			cnt[i] = cfg.GetRules()[i].GetTo().size();

			if(cnt[i] == 0)
			{
				q.push(cfg.GetRules()[i].GetFrom());
			}

			for(const auto& el : cfg.GetRules()[i].GetTo())
			{
				if(el->IsNonterminal())
				{
					concerned_rules[dynamic_cast<NonterminalSymbol&>(*el)].push_back(i);
				}
			}
		}

		for(auto& el : concerned_rules)
		{
			el.second.erase(std::unique(el.second.begin(), el.second.end()), el.second.end());
		}

		while (!q.empty())
		{
			auto top = q.front();
			q.pop();

			is_eps[top] = true;

			for(const auto& i : concerned_rules[top])
			{
				--cnt[i];
				if(cnt[i] == 0)
				{
					q.push(cfg.GetRules()[i].GetFrom());
				}
			}
		}

		std::vector<NonterminalSymbol> solution;
		solution.reserve(is_eps.size());
		std::transform(is_eps.begin(), is_eps.end(), std::back_inserter(solution), [](auto x) { return x.first; });
		return std::move(solution);
	}

	QStringList FindShortestWords(const ContextFreeGrammar& cfg, size_t max_size)
	{
		{
			auto eps = FindEpsilonNonterminals(cfg);
			if (eps.end() != std::find(eps.begin(), eps.end(), cfg.GetStartSymbol()))
				return { "" };
		}

		auto grammar = TransformToGreibach(cfg);

		{
			qDebug() << "Greibach";
			for (const auto& r : grammar.GetRules())
				qDebug() << r;
		}

		std::set<Word, decltype(&_Compare)> q(&_Compare);

		{
			Word word;
			word.emplace_back(new NonterminalSymbol(grammar.GetStartSymbol()));
			q.insert(word);
		}
		
		std::vector<Word> res;
		size_t len = max_size;

		for(auto& el : grammar.GetRules())
		{
			qDebug() << el;
		}

		while(!q.empty())
		{
			/*{
				QStringList lst;
				{
					for (const auto word : q)
					{
						QString dbgstr;
						for (const auto& el : word)
						{
							dbgstr += el->GetChar();
						}
						lst.push_back(dbgstr);
					}
				}
				qDebug() << lst;
			}*/
			Word top_word = *q.begin();
			q.erase(q.begin(), std::next(q.begin()));

			/*{
				QString dbgstr;
				for (const auto& el : top_word)
				{
					dbgstr += el->GetChar();
				}
				qDebug() << dbgstr;
			}*/

			if(_IsFinal(top_word) && len >= top_word.size())
			{
				res.push_back(top_word);
				len = std::min(len, top_word.size());
				continue;
			}

			if(_Norm(top_word) > len)
			{
				break;
			}
			
			for(const auto& rule : grammar.GetRules())
			{
				auto it = top_word.begin();
				while(true)
				{
					it = std::find_if(it, top_word.end(), [target =rule.GetFrom()](std::shared_ptr<GrammarSymbol> symbol)
					{
						return *symbol == target;
					});
					if(top_word.end() == it)
						break;
					Word word(top_word.begin(), it);
					std::copy(rule.GetTo().begin(), rule.GetTo().end(), std::back_inserter(word));
					std::advance(it, 1);
					std::copy(it, top_word.end(), std::back_inserter(word));
					q.insert(word);
				}
			}
		}
		
		QStringList solution;
		for(const auto& word : res)
		{
			QString str;
			std::transform(word.begin(), word.end(), std::back_inserter(str), [](std::shared_ptr<GrammarSymbol> symbol)
			{
				return symbol->GetChar();
			});
			solution.push_back(str);
		}

		solution.erase(std::unique(solution.begin(), solution.end()), solution.end());

		return std::move(solution);
	}

	QList<QPair<QChar, QStringList>> FindShortestWordsFromAllNonterminals(const ContextFreeGrammar& cfg)
	{
		QList<QPair<QChar, QStringList>> solution;

		ContextFreeGrammar grammar = cfg;
		size_t lim = 16;
		for(const auto& rule : cfg.GetRules())
		{
			lim += rule.GetTo().size();
		}
		lim *= 3;

		for(const auto& symbol : cfg.GetNonterminalSymbols())
		{
			grammar.SetStartSymbol(symbol);

			auto cur = FindShortestWords(grammar, lim);
			solution.push_back({ symbol.GetChar() , std::move(cur) });
		}

		return std::move(solution);
	}
} //namespace CFG