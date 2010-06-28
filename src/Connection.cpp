#include "Connection.h"
#include "Connection.moc"

Connection::Connection(QTextEdit* output) : mOutput(output) {
  mSocket = new QTcpSocket();
  mHost = NULL;
  mPort = 23;
  QObject::connect(mSocket, SIGNAL(readyRead()), this, SLOT(read()));
  QObject::connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(error(QAbstractSocket::SocketError)));
  QObject::connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

Connection::~Connection() {
  delete mSocket;
}

void Connection::read() {
  int size = mSocket->bytesAvailable();
  char buffer[size + 1];
  mSocket->read(buffer, size);
  buffer[size] = '\0';
  mOutput->append(buffer);
}

void Connection::error(QAbstractSocket::SocketError socketError) {
  mOutput->append("<font color=red><b>" + mSocket->errorString() +
                  "</b></font>");
}

void Connection::send(QString text) {

  if (isConnected()) {
    text.append("\n");
    mSocket->write(text.toAscii());
    mSocket->flush();
  } else {
    mOutput->append(QString("<font color=red>You have to connect ") +
                    "to a server first!</font>");
  }
}

void Connection::disconnect() {
  mSocket->disconnectFromHost();
}

bool Connection::isConnected() {
  return mSocket->state() != QAbstractSocket::UnconnectedState;
}

void Connection::disconnected() {
  mSocket->connectToHost(*mHost, mPort);
}

void Connection::connect(QString host, int port) {

  if (mHost != NULL)
    delete mHost;

  mHost = new QString(host);
  mPort = port;

  if (isConnected())
    disconnect();
  else
    mSocket->connectToHost(*mHost, mPort);
}
