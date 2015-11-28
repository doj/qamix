#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qlabel.h>
#include <qslider.h> 
#include <qhbox.h>
#include <qvbox.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qfile.h>
#include <qptrstack.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qmessagebox.h> 
#include <alsa/asoundlib.h>
#include "gui.h"
#include "hctldata.h"
#include "hctl_element.h"
#include "hctl_slider.h"
#include "hctl_checkbox.h"
#include "hctl_combobox.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

Gui::Gui(HctlData *p_hctlData, QWidget *parent, const char *name) : QHBox(parent, name) {

  QString qs1;

  hctlData = p_hctlData;
  tabWidget = NULL;
  currentContainer = this;
  sliderValueStyle = sliderValueStyle_NoNumbers;
  isHBox = false;
}

Gui::~Gui() {
  
}

void Gui::addHctlElement(snd_hctl_elem_t *hctl_elem, QString label,
                         MidiControllerList *midiControllerList, IntList *midiIndexList, int sign, int stretch, bool single) {

  snd_ctl_elem_type_t elem_type;
  snd_ctl_elem_info_t *elem_info;
  HctlElement *hctlElement;
  int l1;

  snd_ctl_elem_info_alloca(&elem_info);
  if (snd_hctl_elem_info(hctl_elem, elem_info) < 0) {
    fprintf(stderr, "Could not get info on element.\n");
    exit(-1);
  }
  elem_type = snd_ctl_elem_info_get_type(elem_info);
  
  hctlElement = 0;
  
  switch (elem_type) {
    case SND_CTL_ELEM_TYPE_NONE:
      break;
    case SND_CTL_ELEM_TYPE_BOOLEAN:{
        HctlCheckBox *hctlCheckBox = new HctlCheckBox(hctl_elem, currentContainer, label, sign);
        if (stretch) {
          if (isHBox) {
            ((QHBox *)currentContainer)->setStretchFactor(hctlCheckBox, stretch);
          } else {
            ((QVBox *)currentContainer)->setStretchFactor(hctlCheckBox, stretch);
          }
        }
        hctlElement = hctlCheckBox;
        hctlData->addGuiHctlElement(hctlCheckBox);
      }
      break;
    case SND_CTL_ELEM_TYPE_INTEGER: {
        HctlSlider *hctlSlider = new HctlSlider(hctl_elem, currentContainer, label, sliderValueStyle, single);
        if (stretch) {
          if (isHBox) {
            ((QHBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
          } else {
            ((QVBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
          }
        }
        hctlElement = hctlSlider;
        hctlData->addGuiHctlElement(hctlSlider);
      }
      break;
    case SND_CTL_ELEM_TYPE_ENUMERATED: {
        HctlComboBox *hctlComboBox = new HctlComboBox(hctl_elem, currentContainer, label);
        if (stretch) {
          if (isHBox) {
            ((QHBox *)currentContainer)->setStretchFactor(hctlComboBox, stretch);
          } else {
            ((QVBox *)currentContainer)->setStretchFactor(hctlComboBox, stretch);
          }
        }
        hctlElement = hctlComboBox;
        hctlData->addGuiHctlElement(hctlComboBox);
      }
      break;
    case SND_CTL_ELEM_TYPE_BYTES:
      break;
    case SND_CTL_ELEM_TYPE_IEC958:
      break;
    case SND_CTL_ELEM_TYPE_INTEGER64: {
        HctlSlider *hctlSlider = new HctlSlider(hctl_elem, currentContainer, label, sliderValueStyle, single);
        if (stretch) {
          if (isHBox) {
            ((QHBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
          } else {
            ((QVBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
          }
        }
        hctlElement = hctlSlider;
        hctlData->addGuiHctlElement(hctlSlider);
      }
      break;
  }
  if (hctlElement) {
    IntList::iterator it;
    l1 = 0;
    for (it = midiIndexList->begin(); it != midiIndexList->end(); it++) {
      hctlElement->midiConnect(*it, (*midiControllerList).at(l1));
      hctlData->addMidiController((*midiControllerList).at(l1)); 
      l1++;
    }
  }
}

void Gui::addSpace(int stretch) {

  QWidget *tmpWidget =  new QWidget(currentContainer);
  if (stretch) {
    if (isHBox) {
      ((QHBox *)currentContainer)->setStretchFactor(tmpWidget, stretch);
    } else {
      ((QVBox *)currentContainer)->setStretchFactor(tmpWidget, stretch);
    }
  }
}

void Gui::startTab(QString tabLabel) {

  if (!tabWidget) {
    tabWidget = new QTabWidget(this);
  } 
  QHBox *hbox = new QHBox(tabWidget);
  hbox->setMargin(5);
  hbox->setSpacing(5);
  tabWidget->insertTab(hbox, tabLabel);
  currentContainer = hbox;
  containerList.push(currentContainer);  
}  

void Gui::endTab() {

  containerList.remove();
  currentContainer = (containerList.count()) ? containerList.top() : this;  
}  

void Gui::startVBox(int style, int width, int stretch) {

  QVBox *vbox = new QVBox(currentContainer);
//  vbox->setMargin(5);
  if (style) {
    vbox->setFrameStyle(style);
    vbox->setLineWidth(width);
  }
  if (stretch) {
    if (isHBox) {
      ((QHBox *)currentContainer)->setStretchFactor(vbox, stretch);
    } else {
      ((QVBox *)currentContainer)->setStretchFactor(vbox, stretch);
    }
  }
  currentContainer = vbox;
  containerList.push(currentContainer);  
  isHBox = false;
  isHBoxList.push(isHBox);
}  

void Gui::endVBox() {

  containerList.remove();
  currentContainer = (containerList.count()) ? containerList.top() : this;  
  isHBoxList.pop_back();
  isHBox = (isHBoxList.count()) ? isHBoxList.top() : false;
}  

void Gui::startHBox(int style, int width, int stretch) {

  QHBox *hbox = new QHBox(currentContainer);
//  hbox->setMargin(5);
  if (style) {
    hbox->setFrameStyle(style);
    hbox->setLineWidth(width);
  }
  if (stretch) {
    if (isHBox) {
      ((QHBox *)currentContainer)->setStretchFactor(hbox, stretch);
    } else {
      ((QVBox *)currentContainer)->setStretchFactor(hbox, stretch);
    }
  }
  currentContainer = hbox;
  containerList.push(currentContainer);  
  isHBox = true;
  isHBoxList.push(isHBox);
}  

void Gui::endHBox() {

  containerList.remove();
  currentContainer = (containerList.count()) ? containerList.top() : this;  
  isHBoxList.pop_back();
  isHBox = (isHBoxList.count()) ? isHBoxList.top() : false;
}  

void Gui::startGroupBox(QString groupLabel, int stretch) {

  QGroupBox *gbox = new QGroupBox(1, Qt::Horizontal, groupLabel, currentContainer);
  QHBox *hbox = new QHBox(gbox);
  if (stretch) {
    if (isHBox) {
      ((QHBox *)currentContainer)->setStretchFactor(gbox, stretch);
    } else {
      ((QVBox *)currentContainer)->setStretchFactor(gbox, stretch);
    }
  }
  currentContainer = hbox;
  containerList.push(currentContainer);
  isHBox = true;
  isHBoxList.push(isHBox);
}

void Gui::endGroupBox() {

  containerList.remove();
  currentContainer = (containerList.count()) ? containerList.top() : this;  
  isHBoxList.pop_back();
  isHBox = (isHBoxList.count()) ? isHBoxList.top() : false;
}  

void Gui::setSliderValueStyle(sliderValueStyleEnum p_sliderValueStyle) {

  sliderValueStyle = p_sliderValueStyle;
}
