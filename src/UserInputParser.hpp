#ifndef USER_INPUT_PARSER_H
#define USER_INPUT_PARSER_H

#include <QTextEdit>
#include <QLineEdit>
#include <QString>

#include "Connection.hpp"
#include "KeepAliveThread.hpp"

class UserInputParser : public QObject {

Q_OBJECT

public:
  UserInputParser(QLineEdit* input, QTextEdit* output, Connection* connection,
                  KeepAliveThread* thread);

public slots:
  void parse();

signals:
  void quit();

private:
  QLineEdit* mInput;
  QTextEdit* mOutput;
  Connection* mConnection;
  KeepAliveThread* mThread;

};

#endif // USER_INPUT_PARSER_H
