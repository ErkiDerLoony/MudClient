#include <iostream>
#include <QFont>
#include <QApplication>
#include <QPalette>

#include "MudInputParser.h"
#include "MudInputParser.moc"

MudInputParser::MudInputParser(QTextEdit* output) : ServerInputParser(output),
                                                    mOutput(output) {
  mFormat = new QTextCharFormat();
  mBuffer = new QString();
  mState = Scanning;
}

MudInputParser::~MudInputParser() {
  delete mFormat;
  delete mBuffer;
}

void MudInputParser::parse(const QByteArray input) {

  for (int i = 0; i < input.size(); i++) {
    std::cout << "Parsing character “" << input[i] << "”" << std::endl;
    char buffer[2];
    buffer[0] = input[i];
    buffer[1] = '\0';

    switch (mState) {
    case Scanning:

      if (buffer[0] == '\033')
        mState = WaitForBracket;
      else
        if (buffer[0] != '\r')
          mOutput->textCursor().insertText(buffer, *mFormat);

      break;

    case WaitForBracket:

      if (buffer[0] == '[')
        mState = WaitForFirstNumber;
      else
        mState = Scanning;

      break;

    case WaitForFirstNumber:

      if (QString(buffer[0]).toInt() != 0) {
        mBuffer->append(buffer[0]);
        mState = AccumulateNumbers;
      } else if (buffer[0] == 'm') {
        delete mFormat;
        mFormat = new QTextCharFormat();
        mState = Scanning;
      } 

      break;

    case AccumulateNumbers:

      if (buffer[0] == ';') {
        evaluate(mBuffer);
        mBuffer->clear();
      } else if (buffer[0] == 'm') {
        evaluate(mBuffer);
        mBuffer->clear();
        mState = Scanning;
      } else {
        mBuffer->append(buffer[0]);
      }

      break;
    }
  }
}

void MudInputParser::evaluate(QString* number) {
  int n = number->toInt();
  QBrush fg, bg;
  QColor red = Qt::red, green = Qt::green, yellow = Qt::yellow, blue = Qt::blue,
    magenta = Qt::magenta, cyan = Qt::cyan, white = Qt::white, grey = Qt::gray,
    black = Qt::black;

  if (n == 0)
    return;

  switch (n) {
  case 0:
    delete mFormat;
    mFormat = new QTextCharFormat();
    break;
  case 1:
    mFormat->setFontWeight(QFont::Bold);
    break;
  case 2:
    mFormat->setFontWeight(QFont::Light);
    break;
  case 3:
    mFormat->setFontItalic(true);
    break;
  case 4:
    mFormat->setFontUnderline(true);
    break;
  case 7:
  case 27:
    fg = mFormat->foreground();
    bg = mFormat->background();
    mFormat->setForeground(bg);
    mFormat->setBackground(fg);
    break;
  case 21: // Double underline in not supported
    mFormat->setFontUnderline(true); // So we do single underline.
    break;
  case 22:
    mFormat->setFontWeight(QFont::Normal);
    break;
  case 24:
    mFormat->setFontUnderline(false);
    break;
  case 30:
    mFormat->setForeground(Qt::black);
    break;
  case 31:
    mFormat->setForeground(red.darker().darker());
    break;
  case 32:
    mFormat->setForeground(green.darker().darker());
    break;
  case 33:
    mFormat->setForeground(yellow.darker().darker());
    break;
  case 34:
    mFormat->setForeground(blue.darker().darker());
    break;
  case 35:
    mFormat->setForeground(magenta.darker().darker());
    break;
  case 36:
    mFormat->setForeground(cyan.darker().darker());
    break;
  case 37:
    mFormat->setForeground(white.darker().darker());
    break;
  case 39:
    mFormat->setForeground(QApplication::palette().color(QPalette::Foreground));
    break;
  case 40:
    mFormat->setBackground(black);
    break;
  case 41:
    mFormat->setBackground(red.darker().darker());
    break;
  case 42:
    mFormat->setBackground(green.darker().darker());
    break;
  case 43:
    mFormat->setBackground(yellow.darker().darker());
    break;
  case 44:
    mFormat->setBackground(blue.darker().darker());
    break;
  case 45:
    mFormat->setBackground(magenta.darker().darker());
    break;
  case 46:
    mFormat->setBackground(cyan.darker().darker());
    break;
  case 47:
    mFormat->setBackground(white.darker().darker());
    break;
  case 49:
    mFormat->setBackground(QApplication::palette().color(QPalette::Background));
    break;
  case 90:
    mFormat->setForeground(grey);
    break;
  case 91:
    mFormat->setForeground(red);
    break;
  case 92:
    mFormat->setForeground(green);
    break;
  case 93:
    mFormat->setForeground(yellow);
    break;
  case 94:
    mFormat->setForeground(blue);
    break;
  case 95:
    mFormat->setForeground(magenta);
    break;
  case 96:
    mFormat->setForeground(cyan);
    break;
  case 97:
    mFormat->setForeground(white);
    break;
  case 99:
    mFormat->setForeground(QApplication::palette().color(QPalette::Foreground));
    break;
  case 100:
    mFormat->setBackground(grey);
    break;
  case 101:
    mFormat->setBackground(red);
    break;
  case 102:
    mFormat->setBackground(green);
    break;
  case 103:
    mFormat->setBackground(yellow);
    break;
  case 104:
    mFormat->setBackground(blue);
    break;
  case 105:
    mFormat->setBackground(magenta);
    break;
  case 106:
    mFormat->setBackground(cyan);
    break;
  case 107:
    mFormat->setBackground(white);
    break;
  case 109:
    mFormat->setBackground(QApplication::palette().color(QPalette::Background));
    break;
  default:
    mOutput->textCursor().insertHtml("<font color=red>Unknown modifier: " +
                                     *number + "</font><br>");
  }
}
