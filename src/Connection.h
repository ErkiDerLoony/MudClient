#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTextEdit>

class Connection {

Q_OBJECT

public:
  Connection(QTextEdit* output);
  void connect(QString host, int port);
  //void disconnect();
  //bool isConnected();
  //void send(QString text);

public slots:
  //void error(QAbstractSocket::SocketError socketError);

private:
  QTcpSocket* mSocket;
  QTextEdit* mOutput;

};

#endif // CONNECTION_H
