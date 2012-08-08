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

#include "InputLine.hpp"
#include "InputLine.moc"

InputLine::InputLine() : mIndex(1) {
  mHistory.push_back(new QString("/connect discworld.atuin.net"));
  mHistory.push_back(new QString(""));
  QObject::connect(this, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
}

InputLine::~InputLine() {
  for (unsigned int i = 0; i < mHistory.size(); i++)
    delete mHistory[i];
}

void InputLine::returnPressed() {
  if (mIndex < mHistory.size())
    *mHistory[mHistory.size() - 1] = text();
  mHistory[mIndex] = new QString(text());
  mHistory.push_back(new QString(""));
  mIndex = mHistory.size() - 1;
}

void InputLine::keyPressEvent(QKeyEvent* event) {
  QLineEdit::keyPressEvent(event);

  if (event->key() != Qt::Key_Enter && event->key() != Qt::Key_Return
      && event->key() != Qt::Key_Up && event->key() != Qt::Key_Down
      && mIndex != mHistory.size() - 1) {

    if (*mHistory.back() == QString(""))
      *mHistory[mHistory.size() - 1] = text();
    else
      mHistory.push_back(new QString(text()));

    mIndex = mHistory.size() - 1;
  }

  if (event->key() == Qt::Key_Up && mIndex > 0) {
    mHistory[mIndex] = new QString(text());
    mIndex--;
    setText(*mHistory[mIndex]);
  }

  if (event->key() == Qt::Key_Down && mIndex < mHistory.size() - 1) {
    mHistory[mIndex] = new QString(text());
    mIndex++;
    setText(*mHistory[mIndex]);
  }
}
