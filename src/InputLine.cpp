#include <iostream>

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

  std::cout << "New history is:" << std::endl;
  std::vector<QString*>::iterator it = mHistory.begin();
  int counter = 0;
  while (it != mHistory.end()) {
    std::cout << "[" << counter << "] " << (*it)->toStdString() << std::endl;
    it++;
    counter++;
  }
  std::cout << "Index is: " << mIndex << std::endl;
}
