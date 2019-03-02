#include "CfgAlgoRunner.h"

#include <QRemoteObjectHost>
#include <QtCore/QCoreApplication>
#include <QDebug>

namespace
{
	const QUrl  NODE_URL{ QUrl(QStringLiteral("local:cfg_lab")) };
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CfgAlgoRunner runner;
	QRemoteObjectHost srcNode(NODE_URL);
	srcNode.enableRemoting(&runner);

	return a.exec();
}
