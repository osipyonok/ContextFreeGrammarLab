#pragma once
#include "ui_ClientUI.h"

#include <QPointer>

class Client;

class ClientUI
	: public QMainWindow
	, private Ui_MainWindow
{
public:
	ClientUI(QObject *parent = Q_NULLPTR);
	~ClientUI() override = default;

private:
	void InitConnections();

	QPointer<Client> mp_client;
};

