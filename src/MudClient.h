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

private:
  UserInputParser* mParser;
  Connection* mConnection;

};

#endif // MUD_CLIENT_H
