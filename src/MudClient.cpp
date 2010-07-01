#include <QObject>
#include <QApplication>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QTextCodec>
#include <QPalette>
#include <QColor>

#include "MudClient.h"
#include "MudClient.moc"
#include "MudInputParser.h"

MudClient::MudClient() {
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);

  mOutput = new QTextEdit();
  QPalette p = mOutput->palette();
  p.setColor(QPalette::Base, Qt::black);
  p.setColor(QPalette::Text, QColor(127, 200, 255));
  mOutput->setPalette(p);
  mOutput->setFont(QFont("Monospace", 10));
  mOutput->setReadOnly(true);
  mOutput->setFocusPolicy(Qt::NoFocus);

  QObject::connect(mOutput, SIGNAL(textChanged()), this, SLOT(autoscroll()));

  QLineEdit* input = new QLineEdit();
  input->setFont(QFont("Monospace", 10));

  mConnection = new Connection(mOutput, new MudInputParser(mOutput));
  mParser = new UserInputParser(input, mOutput, mConnection);

  QObject::connect(input, SIGNAL(returnPressed()), mParser, SLOT(parse()));
  QObject::connect(mParser, SIGNAL(quit()), this, SLOT(close()));

  layout->addWidget(mOutput);
  layout->addWidget(input);
  setMinimumSize(QSize(640, 480));
}

MudClient::~MudClient() {
  delete mParser;
  delete mConnection;
}

void MudClient::autoscroll() {
  QTextCursor c = mOutput->textCursor();
  c.movePosition(QTextCursor::End);
  mOutput->setTextCursor(c);
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  MudClient mud;
  mud.show();
  return app.exec();
}
