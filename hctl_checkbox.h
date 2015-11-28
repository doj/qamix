#ifndef HCTL_CHECKBOX_H
#define HCTL_CHECKBOX_H

#include <qptrlist.h>
#include <qcheckbox.h>
#include <qlcdnumber.h>
#include <qlabel.h>
#include <qvbox.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midicheckbox.h"

class HctlCheckBox : public HctlElement  {
    
  Q_OBJECT

  private: 
    virtual void updateView();
    int sign;

  public:
    QPtrList<MidiCheckBox> checkboxList;
    QLabel *label;
        
  public:
    HctlCheckBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name = 0, int p_sign = 1);
    ~HctlCheckBox();
    virtual void midiConnect(int index, MidiController *midiController);
    
  public slots:
    void checkboxValueChanged(int val);  
    
};
                              
#endif
