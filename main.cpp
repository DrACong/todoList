#include "enterbuttondialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置第一个窗口为subwindow，setQuitOnLastWindowClosed(false)防止关闭主窗口时app会退出.
    QApplication::setQuitOnLastWindowClosed(false);
    EnterButtonDialog enterkey;
    enterkey.show();

    return a.exec();
}
