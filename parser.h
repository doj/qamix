#pragma once

#include "gui.h"
#include "hctldata.h"
#include "hctl_slider.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"
#include <QtXml/qxml.h>

#define MIXER_FULL     0
#define MIXER_MASTER   1
#define MIXER_SIMPLE   2

class Parser : public QXmlDefaultHandler 
{
  private:
    QString contentLine, alsaname, mixname;
    HctlData *hctlData;
    Gui *gui;
    MidiControllerList midiControllerList;
    IntList midiIndexList;
    int sign, stretch, mode, ctl_index;
    bool gate, single;
    
  private:
    void addMidiController(int index, int type, int ch, int param, int sign);

  public:
    Parser(HctlData *p_hctlData, Gui *p_gui, int p_mode);
    ~Parser();
    bool startDocument();
    bool endDocument();
    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
    bool characters(const QString& ch); 
};
