#ifndef SERVER_INPUT_PARSER_H
#define SERVER_INPUT_PARSER_H

#include <QTextEdit>

class ServerInputParser : public QObject {

Q_OBJECT

public:
  ServerInputParser(QTextEdit* output);
  virtual ~ServerInputParser() = 0;
  virtual void parse(const QString input) = 0;

private:
  QTextEdit* mOutput;

};

#endif // SERVER_INPUT_PARSER_H
