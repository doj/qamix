#pragma once
#include <QtGui/qcombobox.h>
#include <QtGui/qlcdnumber.h>
#include <QtGui/qlabel.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midicombobox.h"

class HctlComboBox : public HctlElement  {
    
  Q_OBJECT

  private: 
    virtual void updateView();

  public:
    Q3PtrList<MidiComboBox> comboboxList;
    QLabel *label;
        
  public:
    HctlComboBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, QString name);
    ~HctlComboBox();
    virtual void midiConnect(int index, MidiController *midiController);
    
  public slots:
    void comboboxValueChanged(int val);  
    
};
