#include "Connection.h"
#include "Connection.moc"

Connection::Connection(QTextEdit* output) : mOutput(output) {
  mSocket = new QTcpSocket();
  QObject::connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(error(QAbstractSocket::SocketError)));
  QObject::connect(mSocket, SIGNAL(readyRead()), this, SLOT(read()));
}

Connection::~Connection() {
  delete mSocket;
}

void Connection::read() {
  char buffer[mSocket->bytesAvailable()];
  mSocket->read(buffer, mSocket->bytesAvailable());
  mOutput->append(buffer);
}

void Connection::error(QAbstractSocket::SocketError socketError) {
  mOutput->append("<font color=red><b>Error: " + mSocket->errorString() +
                  "</b></font>");
}

void Connection::send(QString text) {

  if (isConnected()) {
    mOutput->append("Sending “" + text.toAscii() + "” to the server.");
    mSocket->write(text.toAscii());
    mSocket->flush();
  } else {
    mOutput->append(QString("<font color=red>You have to connect ") +
                    "to a server first!</font>");
  }
}

void Connection::disconnect() {
  mOutput->append("<font color=red>Lost connection.</font>");
}

bool Connection::isConnected() {
  return mSocket->state() != QAbstractSocket::UnconnectedState;
}

void Connection::connect(QString host, int port) {

  if (isConnected())
    disconnect();

  mSocket->connectToHost(host, port);
}
