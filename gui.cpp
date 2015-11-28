#include <QtGui/qpushbutton.h>
#include <QtCore/qfile.h>
//#include <QtCore/qptrstack.h>
#include <QtGui/qframe.h>
#include <QtGui/qgroupbox.h>
#include "gui.h"
#include "hctldata.h"
#include "hctl_element.h"
#include "hctl_slider.h"
#include "hctl_checkbox.h"
#include "hctl_combobox.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"
#include <Qt3Support/Q3GroupBox>

Gui::Gui(HctlData *p_hctlData, QWidget *parent, const char *name) : Q3HBox(parent, name) {

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
            ((Q3HBox *)currentContainer)->setStretchFactor(hctlCheckBox, stretch);
          } else {
            ((Q3VBox *)currentContainer)->setStretchFactor(hctlCheckBox, stretch);
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
            ((Q3HBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
          } else {
            ((Q3VBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
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
            ((Q3HBox *)currentContainer)->setStretchFactor(hctlComboBox, stretch);
          } else {
            ((Q3VBox *)currentContainer)->setStretchFactor(hctlComboBox, stretch);
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
            ((Q3HBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
          } else {
            ((Q3VBox *)currentContainer)->setStretchFactor(hctlSlider, stretch);
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
      ((Q3HBox *)currentContainer)->setStretchFactor(tmpWidget, stretch);
    } else {
      ((Q3VBox *)currentContainer)->setStretchFactor(tmpWidget, stretch);
    }
  }
}

void Gui::startTab(QString tabLabel) {

  if (!tabWidget) {
    tabWidget = new QTabWidget(this);
  } 
  Q3HBox *hbox = new Q3HBox(tabWidget);
  hbox->setMargin(5);
  hbox->setSpacing(5);
  tabWidget->addTab(hbox, tabLabel);
  currentContainer = hbox;
  containerList.push(currentContainer);  
}  

void Gui::endTab() {

  containerList.remove();
  currentContainer = (containerList.count()) ? containerList.top() : this;  
}  

void Gui::startVBox(int style, int width, int stretch) {

  Q3VBox *vbox = new Q3VBox(currentContainer);
//  vbox->setMargin(5);
  if (style) {
    vbox->setFrameStyle(style);
    vbox->setLineWidth(width);
  }
  if (stretch) {
    if (isHBox) {
      ((Q3HBox *)currentContainer)->setStretchFactor(vbox, stretch);
    } else {
      ((Q3VBox *)currentContainer)->setStretchFactor(vbox, stretch);
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

  Q3HBox *hbox = new Q3HBox(currentContainer);
//  hbox->setMargin(5);
  if (style) {
    hbox->setFrameStyle(style);
    hbox->setLineWidth(width);
  }
  if (stretch) {
    if (isHBox) {
      ((Q3HBox *)currentContainer)->setStretchFactor(hbox, stretch);
    } else {
      ((Q3VBox *)currentContainer)->setStretchFactor(hbox, stretch);
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

  auto gbox = new Q3GroupBox(1, Qt::Horizontal, groupLabel, currentContainer);
  Q3HBox *hbox = new Q3HBox(gbox);
  if (stretch) {
    if (isHBox) {
      ((Q3HBox *)currentContainer)->setStretchFactor(gbox, stretch);
    } else {
      ((Q3VBox *)currentContainer)->setStretchFactor(gbox, stretch);
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
