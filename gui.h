#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QtCore/qstring.h>
#include <QtGui/qlabel.h>
#include <QtGui/qslider.h>
#include <Qt3Support/Q3VBox>
#include <QtGui/qmessagebox.h>
#include <QtGui/qtabwidget.h>
#include <alsa/asoundlib.h>
#include "hctldata.h"
#include "hctl_element.h"
#include "hctl_slider.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"
#include <Qt3Support/Q3ValueStack>
#include <Qt3Support/Q3PtrStack>

class Gui : public Q3HBox
{
  Q_OBJECT

  private:
    int channel;
    HctlData *hctlData;
    QTabWidget *tabWidget;
    QWidget *currentContainer;
    Q3PtrStack<QWidget> containerList; 
    sliderValueStyleEnum sliderValueStyle;    
    bool isHBox;
    Q3ValueStack<bool> isHBoxList;

  public:
    Gui(HctlData *p_hctlData, QWidget* parent=0, const char *name=0);
    ~Gui();
    void startTab(QString tabLabel);
    void endTab();
    void startVBox(int style, int width, int stretch=0);
    void endVBox();
    void startHBox(int style, int width, int stretch=0);
    void endHBox();
    void startGroupBox(QString groupLabel, int stretch=0);
    void endGroupBox(); 
    void addSpace(int stretch=0);
    void setSliderValueStyle(sliderValueStyleEnum p_sliderValueStyle);
    void addHctlElement(snd_hctl_elem_t *hctl_elem, QString label, 
                        MidiControllerList *midiControllerList, IntList *midiIndexList, int sign, int stretch=0, bool single = false);

};
