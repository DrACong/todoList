QT       += core gui sql core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AnimationButton.cpp \
    WrapTextDelegate.cpp \
    enterbuttondialog.cpp \
    historydialog.cpp \
    main.cpp \
    todolistdialog.cpp

HEADERS += \
    AnimationButton.h \
    WrapTextDelegate.h \
    enterbuttondialog.h \
    historydialog.h \
    todolistdialog.h

FORMS += \
    enterbuttondialog.ui \
    historydialog.ui \
    todolistdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    photo.qrc

DISTFILES +=
