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

#ifndef SERVER_INPUT_PARSER_H
#define SERVER_INPUT_PARSER_H

#include <QTextEdit>
#include <QByteArray>

class ServerInputParser : public QObject {

Q_OBJECT

public:
  ServerInputParser(QTextEdit* output);
  virtual ~ServerInputParser() = 0;
  virtual void parse(const QByteArray input) = 0;

private:
  QTextEdit* mOutput;

};

#endif // SERVER_INPUT_PARSER_H
