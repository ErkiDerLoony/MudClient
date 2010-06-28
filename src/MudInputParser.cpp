#include <QFont>

#include "MudInputParser.h"
#include "MudInputParser.moc"

MudInputParser::MudInputParser(QTextEdit* output) : ServerInputParser(output) {
  mFormat = new QTextCharFormat();
  mBuffer = new QString();
  mState = Scanning;
}

MudInputParser::~MudInputParser() {
  delete mFormat;
  delete mBuffer;
}

void MudInputParser::parse(const char* input, int size) {

  for (int i = 0; i < size; i++) {
    char current[1];
    current[0] = input[i];

    switch (mState) {
    case Scanning:

      if (current[0] == '\033')
        mState = WaitForBracket;
      else
        mOutput->textCursor().insertText(current, *mFormat);

      break;

    case WaitForBracket:

      if (current[0] == '[')
        mState = WaitForFirstNumber;
      else
        mState = Scanning;

      break;

    case WaitForFirstNumber:

      if (QString(current).toInt() != 0) {
        mBuffer->append(current);
        mState = AccumulateNumbers;
      } else if (current[0] == 'm') {
        delete mFormat;
        mFormat = new QTextCharFormat();
        mState = Scanning;
      } 

      break;

    case AccumulateNumbers:

      if (current[0] == ';') {
        evaluate(mBuffer);
        mBuffer->clear();
      } else if (current[0] == 'm') {
        evaluate(mBuffer);
        mBuffer->clear();
        mState = Scanning;
      } else {
        mBuffer->append(current);
      }

      break;
    }
  }
}

void MudInputParser::evaluate(QString* number) {
  int n = number->toInt();

  if (n == 0)
    return;

  switch (n) {
  case 0:
    delete mFormat;
    mFormat = new QTextCharFormat();
    break;
  case 1:
    mFormat->setFontWeight(QFont::Bold);
  }
}
