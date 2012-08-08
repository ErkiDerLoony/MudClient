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

#ifndef MUD_CLIENT_H
#define MUD_CLIENT_H

#include <QWidget>

#include "UserInputParser.hpp"
#include "Connection.hpp"
#include "KeepAliveThread.hpp"
#include "TimeThread.hpp"

class MudClient : public QWidget {

Q_OBJECT

public:
  MudClient();
  ~MudClient();
  void closeEvent(QCloseEvent* event);

public slots:
  void autoscroll();

private:
  UserInputParser* mParser;
  Connection* mConnection;
  QTextEdit* mOutput;
  KeepAliveThread* mKeepAliveThread;
  TimeThread* mTimeThread;

};

#endif // MUD_CLIENT_H
