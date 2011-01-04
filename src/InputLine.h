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

protected:
  virtual void keyPressEvent(QKeyEvent* event);

private:
  std::vector<QString*> mHistory;
  std::vector<QString*>::size_type mIndex;

};

#endif // INPUT_LINE_H
