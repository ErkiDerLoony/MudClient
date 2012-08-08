/*
 * © Copyright 2010–2012 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of Erki’s MudClient.
 * 
 * Erki’s MudClient is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTextEdit>

#include "ServerInputParser.hpp"

class Connection : public QObject {

Q_OBJECT

public:
  Connection(QTextEdit* output, ServerInputParser* inputParser);
  ~Connection();
  void connect(QString host, int port);
  void disconnect();
  bool isConnected();
  void send(QString text);

public slots:
  void error(QAbstractSocket::SocketError socketError);
  void read();

private slots:
  void disconnected();

private:
  QTcpSocket* mSocket;
  QTextEdit* mOutput;
  QString* mHost;
  ServerInputParser* mInputParser;
  int mPort;
  bool mConnectionRequest;

};

#endif // CONNECTION_H
