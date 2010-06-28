#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>

class Connection {

public:
  void connect(QString host, int port);
  bool isConnected();
  void send(QString text);

private:
  

};

#endif // CONNECTION_H
