QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cards.cpp \
    changeinformation.cpp \
    client.cpp \
    clientorserver.cpp \
    common.cpp \
    forgetpassword.cpp \
    history.cpp \
    importantcards.cpp \
    main.cpp \
    menu.cpp \
    player.cpp \
    server.cpp \
    signin.cpp \
    signup.cpp \
    skullking.cpp

HEADERS += \
    SlowTyper.h \
    cards.h \
    changeinformation.h \
    client.h \
    clientorserver.h \
    common.h \
    forgetpassword.h \
    history.h \
    importantcards.h \
    menu.h \
    player.h \
    server.h \
    signin.h \
    signup.h \
    skullking.h

FORMS += \
    changeinformation.ui \
    client.ui \
    clientorserver.ui \
    forgetpassword.ui \
    history.ui \
    menu.ui \
    server.ui \
    signin.ui \
    signup.ui \
    skullking.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    ../resource/Beige Plain Textured Wallpaper R8200 - Sample Price.jpg \
    ../resource/download (5).jpg
