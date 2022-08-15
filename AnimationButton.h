#ifndef ANIMATIONBUTTON_H
#define ANIMATIONBUTTON_H

#include <QPushButton>
#include <QTimer>
#include <QGraphicsOpacityEffect>

class QAnimationButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QAnimationButton(QWidget *parent = nullptr);
    ~QAnimationButton();
    using QPushButton::enterEvent;
protected:
    void enterEvent(QEnterEvent *);

    void leaveEvent(QEvent *);

private slots:
    void slot_timeout();

private:
    QTimer* x_timer;
    int x_nCurrent;
    qreal nOpacity;
    QGraphicsOpacityEffect *opacity;

    enum AnimationDirect
    {
        Unknow = 0,
        Big,
        Small
    };
    AnimationDirect x_eAnimation;
};

#endif // ANIMATIONBUTTON_H
