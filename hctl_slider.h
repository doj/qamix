#ifndef HCTL_SLIDER_H
#define HCTL_SLIDER_H

#include <qptrlist.h>
#include <qslider.h>
#include <qlabel.h>
#include <qvbox.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midislider.h"

enum sliderValueStyleEnum { sliderValueStyle_NoNumbers, 
                            sliderValueStyle_SimpleNumbers, sliderValueStyle_SimpleNumbers100,
                            sliderValueStyle_FrameNumbers, sliderValueStyle_FrameNumbers100 };

class HctlSlider : public HctlElement  {
    
  Q_OBJECT

  private: 
    virtual void updateView();
    sliderValueStyleEnum sliderValueStyle;
    bool single;

  public:
    QPtrList<MidiSlider> sliderList;
    QLabel *label;
    QPtrList<QLabel> numberList;
        
  public:
    HctlSlider(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name = 0, 
               sliderValueStyleEnum p_sliderValueStyle = sliderValueStyle_NoNumbers, bool p_single = false);
    ~HctlSlider();
    virtual void midiConnect(int index, MidiController *midiController);
    
  public slots:
    void sliderValueChanged(int val);  
    
};
                              
#endif
