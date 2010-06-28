#include "Connection.h"
Connection::Connection(QTextEdit* output) : mOutput(output) {
  /*  QObject::connect(mSocket,
                   SIGNAL(error(QAbstractSocket::SocketError socketError)),
                   this,
                   SLOT(error(QAbstractSocket::SocketError socketError)));*/
}
/*
void Connection::error(QAbstractSocket::SocketError socketError) {
  mOutput->append("<font color=red><b>Error: " + mSocket->errorString() +
                  "</b></font>");
}

void Connection::send(QString text) {
  mSocket->write(text.toAscii());
  mSocket->flush();
}

void Connection::disconnect() {
  mOutput->append("<font color=red>Lost connection.</font>");
}

bool Connection::isConnected() {
  return mSocket->state() != QAbstractSocket::UnconnectedState;
}
*/
void Connection::connect(QString host, int port) {

  //if (isConnected())
  //disconnect();

  mSocket->connectToHost(host, port);
}
