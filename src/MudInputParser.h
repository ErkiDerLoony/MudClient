#ifndef MUD_INPUT_PARSER
#define MUD_INPUT_PARSER

#include <QTextEdit>
#include <QTextCharFormat>

#include "ServerInputParser.h"

class MudInputParser : public ServerInputParser {

Q_OBJECT

enum State {
  Scanning, WaitForBracket, WaitForFirstNumber, AccumulateNumbers
};

public:
  MudInputParser(QTextEdit* output);
  ~MudInputParser();
  void parse(const char* input, int size);

private:
  void evaluate(QString* number);
  QTextEdit* mOutput;
  QTextCharFormat* mFormat;
  QString* mBuffer;
  State mState;

};

#endif // MUD_INPUT_PARSER
