#ifndef MUD_CLIENT_H
#define MUD_CLIENT_H

#include <QWidget>

#include "UserInputParser.h"
#include "Connection.h"

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

};

#endif // MUD_CLIENT_H
