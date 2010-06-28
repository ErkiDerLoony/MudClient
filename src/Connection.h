#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTextEdit>

class Connection : public QObject {

Q_OBJECT

public:
  Connection(QTextEdit* output);
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
  int mPort;

};

#endif // CONNECTION_H
