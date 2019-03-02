#include "ClientUI.h"

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ClientUI ui;
	ui.show();

	return a.exec();
}
