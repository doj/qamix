# -*- mode: Makefile; -*-

PREFIX?=/usr/local
BIN_DIR=$(DESTDIR)/$(PREFIX)/bin/
DOC_DIR=$(DESTDIR)/$(PREFIX)/share/doc/$(EXE)-$(VERSION)

QT_BASE_DIR?=/usr
QT_LIB_DIR=$(QT_BASE_DIR)/lib64/qt4
QT_BIN_DIR=$(QT_BASE_DIR)/bin
QT_INCLUDE_DIR=$(QT_BASE_DIR)/include/qt4
X11_LIB_DIR=/usr/X11R6/lib
X11_INCLUDE_DIR=/usr/X11R6/include

MOC=$(QT_BIN_DIR)/moc

CXXFLAGS+=-I$(QT_INCLUDE_DIR) -I$(X11_INCLUDE_DIR) -I. -O2 -g -Wall -std=c++11 -DQT3_SUPPORT=1 -MMD -DQAMIX_VERSION=\"$(VERSION)\"
LDFLAGS+=-L$(QT_LIB_DIR) -L$(X11_LIB_DIR) -L/usr/local/lib
LDLIBS+=-lQtCore -lQtGui -lQt3Support -lQtXml -lasound -lm

VERSION=0.0.8
EXE=qamix
CPP=	gui.cpp gui.moc.cpp parser.cpp \
	hctl_element.cpp hctl_element.moc.cpp hctl_slider.cpp hctl_slider.moc.cpp \
	hctl_checkbox.cpp hctl_checkbox.moc.cpp \
	hctl_combobox.cpp hctl_combobox.moc.cpp \
	hctldata.cpp \
	mixlaunch.cpp mixlaunch.moc.cpp \
	mixer.cpp mixer.moc.cpp main.cpp \
	midicontroller.cpp midicontroller.moc.cpp \
	midicontrollerlist.cpp \
	midiguicomponent.cpp midiguicomponent.moc.cpp \
	midislider.cpp midislider.moc.cpp \
	midicheckbox.cpp midicheckbox.moc.cpp \
	midicombobox.cpp midicombobox.moc.cpp \
	utils.cpp myconfig.cpp

all:	$(EXE)

$(EXE):	$(CPP:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

install:	$(EXE)
	strip $(EXE)
	install -m 755 -d $(BIN_DIR)
	install -m 755 $(EXE) $(BIN_DIR)
	install -m 755 -d $(DOC_DIR)
	install -m 644 *.xml README.md $(DOC_DIR)

uninstall:
	$(RM) -r $(BIN_DIR)/$(EXE) $(DOC_DIR)

clean:
	$(RM) *.o *.moc.cpp *.d *~ $(EXE)

%.moc.cpp:	%.h
	$(MOC) -o $@ $<

-include $(CPP:.cpp=.d)
