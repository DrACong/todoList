/*************************************************
Copyright:bupt
Author:Wencong Su
Date:2022-06-16
Description:Reload QPshuButton to realize the ani-
mation effect and transparency change when the but-
ton enters and leaves
**************************************************/
#include "AnimationButton.h"

#define STEP 3
#define STEPCOUNT 4 //max is 4

QAnimationButton::QAnimationButton(QWidget* parent)
    : QPushButton(parent)
    , x_timer(nullptr)
    , x_nCurrent(0)
{
    x_eAnimation = Unknow;
    nOpacity = 0.4;
    x_timer = new QTimer();
    x_timer->setInterval(50);
    opacity = new QGraphicsOpacityEffect;
    opacity->setOpacity(nOpacity);
    this->setGraphicsEffect(opacity);
    connect(x_timer,SIGNAL(timeout()),
            this, SLOT(slot_timeout()));
}

QAnimationButton::~QAnimationButton()
{
    if(x_timer)
    {
        if(x_timer->isActive())
            x_timer->stop();
    }
    delete x_timer;
    if(opacity != nullptr)
        delete opacity;
}

void QAnimationButton::enterEvent(QEnterEvent*)
{
    x_eAnimation = Big;

    if(x_timer->isActive())
        x_timer->stop();
    x_timer->start();
}

void QAnimationButton::leaveEvent(QEvent *)
{
     x_eAnimation = Small;

     if(x_timer->isActive())
         x_timer->stop();
     x_timer->start();
}

void QAnimationButton::slot_timeout()
{
    QRect _rect = this->geometry();
    switch (x_eAnimation) {
    case Big:
        if(x_nCurrent >= 0 && x_nCurrent < STEPCOUNT)
        {
            x_nCurrent++;
            QRect _newRect = QRect(_rect.left() - STEP, _rect.top() - STEP,
                                   _rect.width() + 2 * STEP, _rect.height() + 2 * STEP);
            nOpacity += 0.15;
            opacity->setOpacity(nOpacity);
            this->setGeometry(_newRect);
        }
        else
            x_timer->stop();

        break;
    case Small:
        if(x_nCurrent > 0 && x_nCurrent <= STEPCOUNT)
        {
            x_nCurrent--;
            QRect _newRect = QRect(_rect.left() + STEP, _rect.top() + STEP,
                                   _rect.width() - 2 * STEP, _rect.height() - 2 * STEP);
            nOpacity -= 0.15;
            opacity->setOpacity(nOpacity);
            this->setGeometry(_newRect);
        }
        else
            x_timer->stop();

    default:
        break;
    }
}
