TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x
CXXFLAGS += -std=c++0x


DEFINES += _GXX_EXPERIMENTAL_CXX0X_
DEFINES += __GXX_EXPERIMENTAL_CXX0X__

INCLUDEPATH += ../../

SOURCES += main.cpp \
    	   ../src/Logger.cpp \
    	   ../src/BinarySerializer.cpp \
    	../src/CoreSystem.cpp \
    IIdentifyableTest.cpp

HEADERS += \
    ../include/IStreamable.h \
    ../include/Logger.h \
    ../include/BinarySerializer.h \
    ../include/CoreSystem.h \
    ../include/IIdentifyable.h \
    ../include/ISerializer.h \
    ../include/GeneralDefinitions.h \
    IIdentifyableTest.h

