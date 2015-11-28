#ifndef MIDISLIDER_H
#define MIDISLIDER_H

#include <QtGui/qslider.h>
#include <QtGui/qlabel.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midiguicomponent.h"

class MidiSlider : public MidiGUIcomponent {

Q_OBJECT

public:
  QSlider *slider;
  
public:
  MidiSlider(int minValue, int maxValue, int pageStep, int value, Qt::Orientation orientation,
             QWidget * parent, QString name);
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
