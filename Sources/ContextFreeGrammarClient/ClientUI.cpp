#include "ClientUI.h"

#include "Client.h"

#include <QLineEdit>
#include <QLabel>
#include <QLayout>

#include <vector>

namespace
{
	enum class PARSER_STATE
	{
		EXPECT_FROM,
		EXPECT_TO
	};

	std::vector<std::pair<QChar, std::vector<std::pair<bool, QString>>>> _Parse(const QString& text)
	{
		std::vector<std::pair<QChar, std::vector<std::pair<bool, QString>>>> result;

		QChar cur_from = 0;
		std::vector<std::pair<bool, QString>> cur_to;

		PARSER_STATE state = PARSER_STATE::EXPECT_FROM;

		for(const QChar c : text)
		{
			switch (state)
			{
			case PARSER_STATE::EXPECT_FROM:
				if(c >= 'A' && c <= 'Z')
				{
					cur_from = c;
					state = PARSER_STATE::EXPECT_TO;
				}
				break;
			case PARSER_STATE::EXPECT_TO:
				if(c == '.' || c == '|')
				{
					result.push_back({ cur_from , cur_to });
					cur_to.clear();
					if(c == '.')
					{
						cur_from = 0;
						state = PARSER_STATE::EXPECT_FROM;
						break;
					}
				}
				else if(c >= 'A' && c <= 'Z')
				{
					cur_to.push_back({ false , c });
				}
				else if(c >= 'a' && c <= 'z')
				{
					cur_to.push_back({ true , c });
				}
				break;
			}
		}

		return result;
	}

	const QChar EPS{ 0x3b5 };
	const QUrl  NODE_URL{ QUrl(QStringLiteral("local:cfg_lab")) };
}

ClientUI::ClientUI(QObject *parent/* = Q_NULLPTR*/)
{
	setupUi(this);

	auto node = new QRemoteObjectNode(this);
	QSharedPointer<CfgAlgoRunnerReplica> ptr;

	bool is_connected = node->connectToNode(NODE_URL);
	Q_ASSERT(is_connected);
	if (!is_connected)
		mp_solution->setText("Could not connect to source");

	ptr.reset(node->acquire<CfgAlgoRunnerReplica>());
	ptr.data()->waitForSource();

	mp_client = new Client(ptr, this);

	InitConnections();

	mp_add_rule->clicked();

	setFixedSize(size());
}

void ClientUI::InitConnections()
{
	bool is_connected = false;
	is_connected = connect(mp_add_rule, &QAbstractButton::clicked, [this]()
	{
		auto n = mp_grammar2->rowCount();
		mp_grammar2->setRowCount(n + 1);

		auto left  = new QLineEdit;
		auto arrow = new QLabel(" -> ");
		auto right = new QLineEdit;

		left->setPlaceholderText("S");
		left->setMaxLength(1);
		left->setFixedWidth(50);
		arrow->setPixmap(QPixmap(":arrow.png"));
		arrow->setScaledContents(true);
		arrow->setFocusPolicy(Qt::ClickFocus);
		right->setPlaceholderText(EPS);
		right->setMinimumWidth(200);

		mp_grammar2->setCellWidget(n, 0, left);
		mp_grammar2->setCellWidget(n, 1, arrow);
		mp_grammar2->setCellWidget(n, 2, right);
		mp_grammar2->resizeColumnsToContents();
	});
	Q_ASSERT(is_connected);

	is_connected = connect(mp_del_rule, &QAbstractButton::clicked, [this]()
	{
		mp_grammar2->removeRow(mp_grammar2->currentRow());
	});
	Q_ASSERT(is_connected);

	is_connected = connect(mp_run_algo, &QAbstractButton::clicked, [this]()
	{
		mp_solution->setText("...");
		std::vector<std::pair<QChar, std::vector<std::pair<bool,QString>>>> rules;
		for (int i = 0; i < mp_grammar2->rowCount(); ++i)
		{
			auto from = qobject_cast<QLineEdit const*>(mp_grammar2->cellWidget(i, 0));
			auto to   = qobject_cast<QLineEdit const*>(mp_grammar2->cellWidget(i, 2));

			auto from_ch = from->text().length() > 0 ? from->text()[0] : QChar('S');

			auto cur = _Parse(QString(from_ch) + to->text() + ".");
			std::copy(cur.begin(), cur.end(), std::back_inserter(rules));
		}

		mp_client->Clear();
		for(auto& el :rules)
		{
			QVector<QPair<bool, QString>> to;
			std::for_each(el.second.begin(), el.second.end(), [&to](auto p) { to.push_back({ p.first , p.second }); });
			mp_client->AddRule(el.first, to);
		}
		mp_client->FindSortestWords();
	});
	Q_ASSERT(is_connected);

	is_connected = connect(mp_client, &Client::solution_ready, [this](QList<QPair<QChar, QStringList>> solution)
	{
		QString str;
		for(auto& el : solution)
		{
			el.second.erase(std::unique(el.second.begin(), el.second.end()), el.second.end());
			QString cur_string;
			for (const auto& s : el.second) 
				cur_string += s + ", ";
			cur_string.chop(2);
			if (cur_string.length() == 0 && el.second.size() > 0)
				cur_string += EPS;
			str += QString("Nonterminal %1: min_length(%1) = %2, min_len_words(%1) = { %3 }\n")
				.arg(el.first)
				.arg(el.second.length() > 0 ? QString::number(el.second.first().length()) : "INF")
				.arg(cur_string);
		}
		mp_solution->setText(str);
	});
	Q_ASSERT(is_connected);
}
