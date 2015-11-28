#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <qstring.h>
#include <qlabel.h>
#include <qslider.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qmessagebox.h>
#include <qptrlist.h>
#include <qptrstack.h>
#include <qvaluelist.h>
#include <qvaluestack.h>
#include <qtabwidget.h>
#include <alsa/asoundlib.h>
#include "hctldata.h"
#include "hctl_element.h"
#include "hctl_slider.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

class Gui : public QHBox
{
  Q_OBJECT

  private:
    int channel;
    HctlData *hctlData;
    QTabWidget *tabWidget;
    QWidget *currentContainer;
    QPtrStack<QWidget> containerList; 
    sliderValueStyleEnum sliderValueStyle;    
    bool isHBox;
    QValueStack<bool> isHBoxList;

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
  
#endif
