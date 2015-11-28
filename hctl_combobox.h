#ifndef HCTL_COMBOBOX_H
#define HCTL_COMBOBOX_H

#include <qptrlist.h>
#include <qcombobox.h>
#include <qlcdnumber.h>
#include <qlabel.h>
#include <qvbox.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midicombobox.h"

class HctlComboBox : public HctlElement  {
    
  Q_OBJECT

  private: 
    virtual void updateView();

  public:
    QPtrList<MidiComboBox> comboboxList;
    QLabel *label;
        
  public:
    HctlComboBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name = 0);
    ~HctlComboBox();
    virtual void midiConnect(int index, MidiController *midiController);
    
  public slots:
    void comboboxValueChanged(int val);  
    
};
                              
#endif
