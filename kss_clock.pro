TEMPLATE = app
TARGET = kss_clock
BUILD_PATH = .build/release

QMAKE_C += -std=c11 -Wall -pedantic
QMAKE_CXX += -std=c++14 -Wall -pedantic

#debug {
#	DEFINES += DEBUG
#	QMAKE_C += -g -O0
#	QMAKE_CXX += -g -O0
#	QMAKE_LFLAGS += -rdynamic
#	LIBS += -g
#	BUILD_PATH = .build/debug
#}

MOC_DIR = $$BUILD_PATH/moc
UI_DIR = $$BUILD_PATH/ui
OBJECTS_DIR = $$BUILD_PATH/obj
RCC_DIR = $$BUILD_PATH/res

extraclean.commands = rm -rf .build debug release
distclean.depends = extraclean
QMAKE_EXTRA_TARGETS += distclean extraclean

QT += \
	widgets

HEADERS += \
	kss_clock.hxx

SOURCES += \
	kss_clock.cxx \
	main.cxx

FORMS +=

RESOURCES +=
