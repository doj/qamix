#ifndef MIDISLIDER_H
#define MIDISLIDER_H

#include <qslider.h>
#include <qhbox.h>
#include <qlabel.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midiguicomponent.h"

class MidiSlider : public MidiGUIcomponent {

Q_OBJECT

public:
  QSlider *slider;
  
public:
  MidiSlider(int minValue, int maxValue, int pageStep, int value, QSlider::Orientation orientation,
             QWidget * parent, const char * name=0);
  ~MidiSlider();
  virtual void setMidiValue(int value);
  virtual int getMidiValue();

signals:
  void valueChanged(int value);
	
public slots:
  void updateValue(int);
  void updateSlider(int);
    
};

#endif
