#ifndef HCTL_ELEMT_H
#define HCTL_ELEMT_H

#include <qvbox.h>
#include <qcheckbox.h>
#include <alsa/asoundlib.h>
#include "midicontroller.h"

class HctlElement : public QVBox {
   
  Q_OBJECT  

  private:
    static int hctl_callback(snd_hctl_elem_t *elem, unsigned int mask);
    virtual void updateView();
                
  public:
    snd_hctl_elem_t *hctl_elem;
    snd_ctl_elem_type_t elem_type;
    snd_ctl_elem_info_t *elem_info;
    snd_ctl_elem_value_t *elem_value;
    bool lock;
    QCheckBox *lockBox;
     
  public:
    HctlElement(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name = 0);
    ~HctlElement();
    virtual void midiConnect(int index, MidiController *midiController);
        
  public slots:
    void updateInfo();
    void updateValue();
    void lockToggled(bool);
};
                              
#endif
