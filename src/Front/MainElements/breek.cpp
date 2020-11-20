//VIEW

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QQmlProperty>
#include <QQuickItem>

#include <iostream>

#include "breek.h"

Breek::Breek(QWidget *parent):
  QPushButton(parent),
  state_(false),
  workState_(Conditions::GREY_FOREGROUND),
  width_(80),
  height_(80),
  quickWidget_(nullptr),
  graphObject_(nullptr)
{
  this->setFocusPolicy(Qt::ClickFocus);
  this->setFixedSize(width_, height_);
  this->setFlat(true);
}

Breek::Breek(int width, int height, QWidget *parent):
  QPushButton(parent),
  state_(false),
  workState_(Conditions::GREY_FOREGROUND),
  width_(width),
  height_(height),
  quickWidget_(nullptr),
  graphObject_(nullptr)
{
  this->setFocusPolicy(Qt::ClickFocus);
  this->setFixedSize(width_, height_);
  this->setFlat(true);
}

Breek::~Breek()
{
  graphObject_ = nullptr;
  delete quickWidget_;
}

void Breek::keyPressEvent(QKeyEvent *event)
{
  int iKey = event->key();

  if (iKey == Qt::Key_W) {
    if (workState_ == Conditions::RED) {
      connectToQml(nEmoj_, Directions::DOWNSIDE, workState_, Conditions::GREY_FOREGROUND);
      workState_ = Conditions::GREY_FOREGROUND;
    }
    else if (workState_ == Conditions::GREY_FOREGROUND) {
      connectToQml(nEmoj_, Directions::DOWNSIDE, workState_, Conditions::GREEN);
      workState_ = Conditions::GREEN;
    }
    else if (workState_ == Conditions::GREEN) {
      connectToQml(nEmoj_, Directions::DOWNSIDE, workState_, Conditions::GREY_BACKGROUND);
      workState_ = Conditions::GREY_BACKGROUND;
    }
    else {
      connectToQml(nEmoj_, Directions::DOWNSIDE, workState_, Conditions::RED);
      workState_ = Conditions::RED;
    }
  }

  if (iKey == Qt::Key_S) {
    if (workState_ == Conditions::RED) {
      connectToQml(nEmoj_, Directions::UPSIDE, workState_, Conditions::GREY_BACKGROUND);
      workState_ = Conditions::GREY_BACKGROUND;
    }
    else if (workState_ == Conditions::GREY_FOREGROUND) {
      connectToQml(nEmoj_, Directions::UPSIDE, workState_, Conditions::RED);
      workState_ = Conditions::RED;
    }
    else if (workState_ == Conditions::GREEN){
      connectToQml(nEmoj_, Directions::UPSIDE, workState_, Conditions::GREY_FOREGROUND);
      workState_ = Conditions::GREY_FOREGROUND;
    }
    else {
      connectToQml(nEmoj_, Directions::UPSIDE, workState_, Conditions::GREEN);
      workState_ = Conditions::GREEN;
    }
  }

  if (iKey == Qt::Key_D) {
		workState_ = Conditions::GREY_FOREGROUND;
		connectToQml(nEmoj_, workState_);
    emit moveBreek(zoneIndex_, dayIndex_, true);
  }

  if (iKey == Qt::Key_A) {
		workState_ = Conditions::GREY_FOREGROUND;
		connectToQml(nEmoj_, workState_);
    emit moveBreek(zoneIndex_, dayIndex_, false);
  }

	if (iKey == Qt::Key_E) {
		emit doubleClicked();
	}

	//for description
	if (workState_ == Conditions::GREY_FOREGROUND || workState_ == Conditions::GREY_BACKGROUND || !state_) {
		if (state_) {
			emit sendSateToLilDay(zoneIndex_, dayIndex_, 0);
		}
	}
	else if (workState_ == Conditions::GREEN) {
		emit sendSateToLilDay(zoneIndex_, dayIndex_, 1);
	}
	else {
		emit sendSateToLilDay(zoneIndex_, dayIndex_, 2);
	}
}

void Breek::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit doubleClicked();
}

bool Breek::getState()
{
  return state_;
}

void Breek::setState(bool state)
{
  state_ = state;
}

void Breek::connectToQml(int indexOfEmoji, Conditions cond)
{
  if (indexOfEmoji < 0 || indexOfEmoji > 10) return;

  // код, связывающий кнопку с qml
  if (quickWidget_ == nullptr) {
    quickWidget_ = new QQuickWidget(QUrl("qrc:/qml/Front/Qml/qml_for_breeks.qml"), this);
    quickWidget_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget_->setFixedSize(1.125*width_, 1.125*height_);
    graphObject_ = quickWidget_->rootObject();
  }

  if (!quickWidget_->isVisible()) quickWidget_->setVisible(true);

  graphObject_->setProperty("indexOfEmoji", indexOfEmoji);
  QQmlProperty(graphObject_, "animationOn").write(false);
	graphObject_->setProperty("indexOfCondFrom", cond);
	graphObject_->setProperty("indexOfCondTo", cond);

  // работа с фоном сцены
  QColor color;
	color.setNamedColor("#F7F7F7");
	quickWidget_->quickWindow()->setColor(color);

//  QWidget *container = QWidget::createWindowContainer(&quickWidget_, this);
  // конец кода, связывающего кнопку с qml
}

void Breek::connectToQml(Conditions cond)
{
	QQmlProperty(graphObject_, "animationOn").write(false);
	graphObject_->setProperty("indexOfCondFrom", cond);
	graphObject_->setProperty("indexOfCondTo", cond);

	// работа с фоном сцены
	QColor color;
	color.setNamedColor("#F7F7F7");
	quickWidget_->quickWindow()->setColor(color);
}

void Breek::connectToQml(int indexOfEmoji, Directions dir,
                         Conditions from, Conditions to)
{
  if (indexOfEmoji < 0 || indexOfEmoji > 9) return;

  // код, связывающий кнопку с qml
  if (quickWidget_ == nullptr) {
      quickWidget_ = new QQuickWidget(QUrl("qrc:/qml/Front/Qml/qml_for_breeks.qml"), this);
      quickWidget_->setResizeMode(QQuickWidget::SizeRootObjectToView);
      quickWidget_->setFixedSize(1.125*width_, 1.125*height_);
      graphObject_ = quickWidget_->rootObject();
  }

  graphObject_->setProperty("indexOfEmoji", indexOfEmoji);
  QQmlProperty(graphObject_, "direction").write(dir);
  graphObject_->setProperty("indexOfCondFrom", from);
  graphObject_->setProperty("indexOfCondTo", to);
  QQmlProperty(graphObject_, "animationOn").write(false);
	QQmlProperty(graphObject_, "animationOn").write(true);

  // работа с фоном сцены
//  QColor color = Qt::GlobalColor::gray;
//  quickWidget_->quickWindow()->setColor(color);

//  QWidget *container = QWidget::createWindowContainer(&quickWidget_, this);
  // конец кода, связывающего кнопку с qml
}

void Breek::setEmoj(int numOfEmoji)
{
  if (numOfEmoji >= 0 && numOfEmoji < 10) nEmoj_ = numOfEmoji;
}

int Breek::getEmojiNum()
{
  return nEmoj_;
}

void Breek::setIndex(const int zoneIndex, const int dayIndex)
{
  zoneIndex_ = zoneIndex;
  dayIndex_ = dayIndex;
}

void Breek::changeBreekState()
{
  state_ = !state_;
	workState_ = Conditions::GREY_FOREGROUND;
  this->setEnabled(!this->isEnabled());

  if (state_) {
		connectToQml(nEmoj_, workState_);
  } else {
    this->quickWidget_->setVisible(false);
  }

	emit isHere(zoneIndex_, dayIndex_, state_);
}

void Breek::changeEmoji(int nEmoji)
{
	nEmoj_ = nEmoji;
	connectToQml(nEmoj_, Conditions::BLUE);
}

void Breek::closeEmojiHub()
{
	connectToQml(nEmoj_, workState_);
}

void Breek::openEmojiHub()
{
	connectToQml(Conditions::BLUE);
}

int Breek::getWidth()
{
  return width_;
}

int Breek::getHeight()
{
  return height_;
}
