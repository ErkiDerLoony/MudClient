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

#ifndef INPUT_LINE_H
#define INPUT_LINE_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QString>

class InputLine : public QLineEdit {

Q_OBJECT

public:
  InputLine();
  virtual ~InputLine();

public slots:
  void returnPressed();

signals:
  void keyPressed();

protected:
  virtual void keyPressEvent(QKeyEvent* event);

private:
  std::vector<QString*> mHistory;
  std::vector<QString*>::size_type mIndex;

};

#endif // INPUT_LINE_H
