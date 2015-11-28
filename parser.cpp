#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qxml.h>
#include <qframe.h>
#include <alsa/asoundlib.h>
#include "parser.h"
#include "gui.h"
#include "hctldata.h"
#include "hctl_slider.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

Parser::Parser(HctlData *p_hctlData, Gui *p_gui, int p_mode) {

  gui = p_gui;
  hctlData = p_hctlData;
  mode = p_mode;
  sign = 1;
  stretch = 0;
  single = false;
  gate = false;
}

Parser::~Parser() {

}

bool Parser::startDocument() {

  return(true);
}

bool Parser::endDocument() {

  return(true);
}

bool Parser::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) {

  int lineWidth = 2;
  int l1;
  QString qs;
  int style;

  if ((qName.compare("full") == 0) && (mode == 0)) {
    gate = true;
//    printf("start %s\n", qName.latin1());
  } else if ((qName.compare("master") == 0) && (mode == 1)) {
    gate = true;
//    printf("start %s\n", qName.latin1());
  } else if ((qName.compare("simple") == 0) && (mode == 2)) {
    gate = true;
//    printf("start %s\n", qName.latin1());
  }
  if (!gate) {
    return(true);
  }
  contentLine = "";
  if (qName.compare("tab") == 0) {
    if (atts.count()) {
      gui->startTab(atts.value(0));
    } else {
      gui->startTab("   ");
    }
  }
  if (qName.compare("group") == 0) {
    qs = "    ";
    for (l1 = 0; l1 < atts.count(); l1++) {
      if ((atts.qName(l1).compare("name")==0)) {
        qs = atts.value(l1);
      }
      if ((atts.qName(l1).compare("stretch")==0)) {
        stretch = atts.value(l1).toInt();
      }
    }
    gui->startGroupBox(qs, stretch);
    stretch = 0;
  }
  if (qName.compare("vgroup") == 0) {
    style = 0;
    for (l1 = 0; l1 < atts.count(); l1++) {
      if ((atts.qName(l1).compare("stretch")==0)) {
        stretch = atts.value(l1).toInt();
      }
      if ((atts.qName(l1).compare("width")==0)) {
        lineWidth = atts.value(l1).toInt();
      }
      if ((atts.qName(l1).compare("frame")==0) && (atts.value(l1).compare("plain")==0)) {
        style = QFrame::Panel | QFrame::Plain;
      }
      if ((atts.qName(l1).compare("frame")==0) && (atts.value(l1).compare("raised")==0)) {
        style = QFrame::Panel | QFrame::Raised;
      }
      if ((atts.qName(l1).compare("frame")==0) && (atts.value(l1).compare("sunken")==0)) {
        style = QFrame::Panel | QFrame::Sunken;
      }
    }
    gui->startVBox(style, lineWidth, stretch);
    stretch = 0;
  }
  if (qName.compare("hgroup") == 0) {
    style = 0;
    for (l1 = 0; l1 < atts.count(); l1++) {
      if ((atts.qName(l1).compare("stretch")==0)) {
        stretch = atts.value(l1).toInt();
      }
      if ((atts.qName(l1).compare("width")==0)) {
        lineWidth = atts.value(l1).toInt();
      }
      if ((atts.qName(l1).compare("frame")==0) && (atts.value(l1).compare("plain")==0)) {
        style = QFrame::Panel | QFrame::Plain;
      }
      if ((atts.qName(l1).compare("frame")==0) && (atts.value(l1).compare("raised")==0)) {
        style = QFrame::Panel | QFrame::Raised;
      }
      if ((atts.qName(l1).compare("frame")==0) && (atts.value(l1).compare("sunken")==0)) {
        style = QFrame::Panel | QFrame::Sunken;
      }
    }
    gui->startHBox(style, lineWidth, stretch);
    stretch = 0;
  }
  if (qName.compare("element") == 0) {
    ctl_index = 0;
    for (l1 = 0; l1 < atts.count(); l1++) {
      if ((atts.qName(l1).compare("stretch")==0)) {
        stretch = atts.value(l1).toInt();
      }
      if ((atts.qName(l1).compare("index")==0)) {
        ctl_index = atts.value(l1).toInt();
      }
    }
    alsaname = "";
    mixname = "";
    sign = 1;
    single = false;
    midiControllerList.clear();
    midiIndexList.clear();
  }
  if (qName.compare("midi") == 0) {
    addMidiController(atts.value(0).toInt(), atts.value(1).toInt(), atts.value(2).toInt(), atts.value(3).toInt(), atts.value(4).toInt());
  }
  if (qName.compare("inverse") == 0) {
    sign = -1;
  }
  if (qName.compare("single") == 0) {
    single = true;
  }
  if (qName.compare("space") == 0) {
    for (l1 = 0; l1 < atts.count(); l1++) {
      if ((atts.qName(l1).compare("stretch")==0)) {
        stretch = atts.value(l1).toInt();
      }
    }
    gui->addSpace(stretch);
    stretch = 0;
  }
  if (qName.compare("sliderNumbers") == 0) {
    if (atts.count()) {
      if ((atts.qName(0).compare("style")==0) && (atts.value(0).compare("off")==0)) {
        gui->setSliderValueStyle(sliderValueStyle_NoNumbers);
      }
      if ((atts.qName(0).compare("style")==0) && (atts.value(0).compare("simple")==0)) {
        gui->setSliderValueStyle(sliderValueStyle_SimpleNumbers);
      }
      if ((atts.qName(0).compare("style")==0) && (atts.value(0).compare("frame")==0)) {
        gui->setSliderValueStyle(sliderValueStyle_FrameNumbers);
      }
      if ((atts.qName(0).compare("style")==0) && (atts.value(0).compare("simple100")==0)) {
        gui->setSliderValueStyle(sliderValueStyle_SimpleNumbers100);
      }
      if ((atts.qName(0).compare("style")==0) && (atts.value(0).compare("frame100")==0)) {
        gui->setSliderValueStyle(sliderValueStyle_FrameNumbers100);
      }
    }
  }
  return(true);
}

bool Parser::endElement(const QString& namespaceURI, const QString& localName, const QString& qName) {

  QString qs;
  snd_ctl_elem_id_t *snd_ctl_elem_id;
  snd_hctl_elem_t *snd_hctl_elem;

//  printf("end %s\n", qName.latin1());
  if ((qName.compare("full") == 0)||(qName.compare("master") == 0)||(qName.compare("simple") == 0)) {
    gate = false;
//    printf("--> end %s\n", qName.latin1());
  }
  if (!gate) {
    return(true);
  }
  qs = contentLine.stripWhiteSpace();
  contentLine = "";
  if (qName.compare("tab") == 0) {
    gui->endTab();
  }
  if (qName.compare("group") == 0) {
    gui->endGroupBox();              
  }  
  if (qName.compare("vgroup") == 0) {
    gui->endVBox();              
  }  
  if (qName.compare("hgroup") == 0) {
    gui->endHBox();              
  }  
  if (qName.compare("element") == 0) { 
    if (mixname.isEmpty()) {
      mixname = alsaname;
    }
    snd_ctl_elem_id_alloca(&snd_ctl_elem_id);
    snd_ctl_elem_id_set_name(snd_ctl_elem_id, alsaname.latin1());
    snd_ctl_elem_id_set_index(snd_ctl_elem_id, ctl_index);
    snd_ctl_elem_id_set_interface(snd_ctl_elem_id, SND_CTL_ELEM_IFACE_MIXER);
    if ((snd_hctl_elem = snd_hctl_find_elem(hctlData->hctl_handle, snd_ctl_elem_id)) > 0) {
      gui->addHctlElement(snd_hctl_elem, mixname, &midiControllerList, &midiIndexList, sign, stretch, single);
    } else {
      snd_ctl_elem_id_set_interface(snd_ctl_elem_id, SND_CTL_ELEM_IFACE_PCM);
      if ((snd_hctl_elem = snd_hctl_find_elem(hctlData->hctl_handle, snd_ctl_elem_id)) > 0) {
        gui->addHctlElement(snd_hctl_elem, mixname, &midiControllerList, &midiIndexList, sign, stretch, single);
      }
    }
    stretch = 0;
  } 
  if (qName.compare("alsaname") == 0) {
    alsaname = qs; 
  }
  if (qName.compare("mixname") == 0) {
    mixname = qs;
  }
  return(true);
}

bool Parser::characters(const QString& ch) {

  if (gate) {
    contentLine += ch;
  }
  return(true);
}

void Parser::addMidiController(int index, int type, int ch, int param, int sign) {

  MidiController *midiController = new MidiController(type, ch, param, sign);
  midiControllerList.append(midiController);
  midiIndexList.append(index);
}
