######################################################################
# Automatically generated by qmake (2.01a) ?? 11? 9 09:19:48 2016
######################################################################

TEMPLATE = lib
TARGET   = dlogin
DEPENDPATH += .
DESTDIR = $(NBENV)/lib
INCLUDEPATH += . $(NBENV)/source/include $(SYS_DB_INC)
LIBS += -L$(NBENV)/lib -ldbsql -lscdnet -lscdrdb -ldmessagebox

DEFINES += UI_LOGIN_DLL

# Input
HEADERS += login_bottom_widget.h login_top_widget.h login_widget.h
SOURCES += login_bottom_widget.cpp \
           login_top_widget.cpp \
           login_widget.cpp 
RESOURCES += dlogin.qrc