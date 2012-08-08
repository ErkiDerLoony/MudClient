/*
 * © Copyright 2010–2012 by Edgar Kalkowski <eMail@edgar-kalkowski.de>
 * 
 * This file is part of Erki’s MudClient.
 * 
 * Erki’s MudClient is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
