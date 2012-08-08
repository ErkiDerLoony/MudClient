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

#include <iostream>
#include <QString>
#include <QByteArray>

#include "Connection.hpp"
#include "Connection.moc"

Connection::Connection(QTextEdit* output, ServerInputParser* inputParser)
  : mOutput(output), mInputParser(inputParser) {
  mSocket = new QTcpSocket();
  mHost = NULL;
  mPort = 23;
  mConnectionRequest = false;
  QObject::connect(mSocket, SIGNAL(readyRead()), this, SLOT(read()));
  QObject::connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(error(QAbstractSocket::SocketError)));
  QObject::connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

Connection::~Connection() {
  delete mSocket;
  delete mInputParser;
}

void Connection::read() {
  mInputParser->parse(mSocket->readAll());
}

void Connection::error(QAbstractSocket::SocketError socketError) {
  mOutput->textCursor().insertHtml("<font color=red><b>" +
                                   mSocket->errorString() + "</b></font><br>");
}

void Connection::send(QString text) {

  if (isConnected()) {
    text.append("\n");
    mSocket->write(text.toAscii());
    mSocket->flush();
  } else {
    mOutput->textCursor().insertHtml("<font color=red>You have to connect " +
                                     QString("to a server first!</font><br>"));
  }
}

void Connection::disconnect() {
    mSocket->disconnectFromHost();
}

bool Connection::isConnected() {
  return mSocket->state() != QAbstractSocket::UnconnectedState;
}

void Connection::disconnected() {

  if (mConnectionRequest)
    mSocket->connectToHost(*mHost, mPort);

  mConnectionRequest = false;
}

void Connection::connect(QString host, int port) {

  if (mHost != NULL)
    delete mHost;

  mHost = new QString(host);
  mPort = port;

  if (isConnected()) {
    mConnectionRequest = true;
    disconnect();
  } else
    mSocket->connectToHost(*mHost, mPort);
}
