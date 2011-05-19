#include <iostream>

#include "UserInputParser.hpp"
#include "UserInputParser.moc"
#include "Connection.hpp"

UserInputParser::UserInputParser(QLineEdit* input, QTextEdit* output,
                                 Connection* connection,
                                 KeepAliveThread* thread)
  : mInput(input), mOutput(output), mConnection(connection), mThread(thread) {
}

void UserInputParser::parse() {
  mThread->reset();
  mOutput->textCursor().insertHtml("<font color=grey>" + mInput->text() +
                                   "</font><br>");

  if (mInput->text() == "/connect") {
    QString error("<font color=red>");
    error.append("You need to specify a server to connect to!");
    error.append("</font><br>");
    mOutput->textCursor().insertHtml(error);
  } else if (mInput->text().startsWith("/connect ")) {
    QString host = mInput->text().mid(QString("/connect ").length());
    int port = 23;

    if (host.contains(" ")) {
      QString p = host.mid(host.lastIndexOf(" ") + 1, host.length());

      if (p.toInt() != 0) {
        port = p.toInt();
        host = host.mid(0, host.lastIndexOf(" "));
      }
    }

    mOutput->textCursor().insertText("Connecting to " + host + ":" +
                                     QString::number(port) + " …\n");
    mConnection->connect(host, port);
  } else if (mInput->text() == "/quit" ||
             mInput->text() == "/exit" ||
             mInput->text() == "/q") {
    quit();
  } else {
    mConnection->send(mInput->text());
  }

  mInput->clear();
}
