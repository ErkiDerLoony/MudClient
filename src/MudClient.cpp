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

  QTextEdit* output = new QTextEdit();
  QPalette p = output->palette();
  p.setColor(QPalette::Base, Qt::black);
  p.setColor(QPalette::Text, QColor(127, 200, 255));
  output->setPalette(p);
  output->setFont(QFont("Monospace", 10));
  output->setReadOnly(true);
  output->setFocusPolicy(Qt::NoFocus);

  QLineEdit* input = new QLineEdit();

  mConnection = new Connection(output, new MudInputParser(output));
  mParser = new UserInputParser(input, output, mConnection);

  QObject::connect(input, SIGNAL(returnPressed()), mParser, SLOT(parse()));
  QObject::connect(mParser, SIGNAL(quit()), this, SLOT(close()));

  layout->addWidget(output);
  layout->addWidget(input);
  setMinimumSize(QSize(640, 480));
}

MudClient::~MudClient() {
  delete mParser;
  delete mConnection;
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  MudClient mud;
  mud.show();
  return app.exec();
}
