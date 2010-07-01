#include <iostream>
#include <QString>
#include <QByteArray>

#include "Connection.h"
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
