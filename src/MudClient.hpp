#ifndef MUD_CLIENT_H
#define MUD_CLIENT_H

#include <QWidget>

#include "UserInputParser.hpp"
#include "Connection.hpp"
#include "KeepAliveThread.hpp"

class MudClient : public QWidget {

Q_OBJECT

public:
  MudClient();
  ~MudClient();
  void closeEvent(QCloseEvent* event);

public slots:
  void autoscroll();

private:
  UserInputParser* mParser;
  Connection* mConnection;
  QTextEdit* mOutput;
  KeepAliveThread* mKeepAliveThread;

};

#endif // MUD_CLIENT_H
