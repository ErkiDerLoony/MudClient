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

#include <QObject>
#include <QApplication>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QTextCodec>
#include <QPalette>
#include <QColor>
#include <QSettings>

#include "MudClient.hpp"
#include "MudClient.moc"
#include "MudInputParser.hpp"
#include "InputLine.hpp"

MudClient::MudClient() {
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);

  mOutput = new QTextEdit();
  QPalette p = mOutput->palette();
  p.setColor(QPalette::Base, Qt::black);
  p.setColor(QPalette::Text, QColor(127, 200, 255));
  mOutput->setPalette(p);
  mOutput->setFont(QFont("Monospace", 10));
  mOutput->setReadOnly(true);
  mOutput->setFocusPolicy(Qt::NoFocus);

  QObject::connect(mOutput, SIGNAL(textChanged()), this, SLOT(autoscroll()));

  QLineEdit* input = new InputLine();
  input->setFont(QFont("Monospace", 10));

  mConnection = new Connection(mOutput, new MudInputParser(mOutput));

  layout->addWidget(mOutput);
  layout->addWidget(input);

  // Start keep alive and timer threads.
  mKeepAliveThread = new KeepAliveThread(input, mConnection);
  mKeepAliveThread->start();
  mTimeThread = new TimeThread(mOutput);
  mTimeThread->start();

  // Create parser for user input.
  mParser = new UserInputParser(input, mOutput, mConnection, mKeepAliveThread);

  QObject::connect(mParser, SIGNAL(quit()), this, SLOT(close()));
  QObject::connect(input, SIGNAL(returnPressed()), mParser, SLOT(parse()));

  // Load window size and position.
  QSettings settings("@soft", "MudClient");
  settings.beginGroup("MainWindow");
  resize(settings.value("size", QSize(640, 480)).toSize());
  move(settings.value("position", QPoint(0, 0)).toPoint());
  settings.endGroup();
}

MudClient::~MudClient() {
  delete mParser;
  delete mConnection;

  mKeepAliveThread->quit();
  mKeepAliveThread->wait();
  delete mKeepAliveThread;

  mTimeThread->quit();
  mTimeThread->wait();
  delete mTimeThread;
}

void MudClient::closeEvent(QCloseEvent* event) {
  QSettings settings("@soft", "MudClient");
  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

void MudClient::autoscroll() {
  QTextCursor c = mOutput->textCursor();
  c.movePosition(QTextCursor::End);
  mOutput->setTextCursor(c);
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  MudClient mud;
  mud.show();
  return app.exec();
}
