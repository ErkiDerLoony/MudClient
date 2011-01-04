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
