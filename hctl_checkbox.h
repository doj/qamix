#ifndef HCTL_CHECKBOX_H
#define HCTL_CHECKBOX_H

#include <QtGui/qcheckbox.h>
#include <QtGui/qlcdnumber.h>
#include <QtGui/qlabel.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midicheckbox.h"

class HctlCheckBox : public HctlElement  {
    
  Q_OBJECT

  private: 
    virtual void updateView();
    int sign;

  public:
    Q3PtrList<MidiCheckBox> checkboxList;
    QLabel *label;
        
  public:
    HctlCheckBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, QString name, int p_sign = 1);
    ~HctlCheckBox();
    virtual void midiConnect(int index, MidiController *midiController);
    
  public slots:
    void checkboxValueChanged(int val);  
    
};
                              
#endif
