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

#ifndef MUD_INPUT_PARSER
#define MUD_INPUT_PARSER

#include <QTextEdit>
#include <QTextCharFormat>
#include <QByteArray>

#include "ServerInputParser.hpp"

class MudInputParser : public ServerInputParser {

Q_OBJECT

enum State {
  Scanning, WaitForBracket, WaitForFirstNumber, AccumulateNumbers
};

public:
  MudInputParser(QTextEdit* output);
  ~MudInputParser();
  void parse(const QByteArray input);

private:
  void evaluate(QString* number);
  QTextEdit* mOutput;
  QTextCharFormat* mFormat;
  QString* mBuffer;
  State mState;

};

#endif // MUD_INPUT_PARSER
