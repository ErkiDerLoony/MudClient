#ifndef MUD_INPUT_PARSER
#define MUD_INPUT_PARSER

#include <QTextEdit>
#include <QTextCharFormat>
#include <QByteArray>

#include "ServerInputParser.hpp"

class MudInputParser : public ServerInputParser {

Q_OBJECT

enum State {
  Scanning, WaitForBracket, WaitForFirstNumber, AccumulateNumbers
};

public:
  MudInputParser(QTextEdit* output);
  ~MudInputParser();
  void parse(const QByteArray input);

private:
  void evaluate(QString* number);
  QTextEdit* mOutput;
  QTextCharFormat* mFormat;
  QString* mBuffer;
  State mState;

};

#endif // MUD_INPUT_PARSER
