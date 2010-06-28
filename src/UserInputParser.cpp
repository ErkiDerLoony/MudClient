#include <iostream>

#include "UserInputParser.h"
#include "UserInputParser.moc"
#include "Connection.h"

UserInputParser::UserInputParser(QLineEdit* input, QTextEdit* output,
                                 Connection* connection)
  : mInput(input), mOutput(output), mConnection(connection) {
}

void UserInputParser::parse() {
  mOutput->append("<font color=grey>" + mInput->text() + "</font>");

  if (mInput->text() == "/connect") {
    QString error("<font color=red>");
    error.append("You need to specify a server to connect to!");
    error.append("</font>");
    mOutput->append(error);
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

    mOutput->append("Connecting to " + host + ":" + QString::number(port) +
                    " …");
    mConnection->connect(host, port);
  } else if (mInput->text() == "/quit" ||
             mInput->text() == "/exit" ||
             mInput->text() == "/q") {
    quit();
  }

  mInput->clear();
}
