#ifndef MIDICHECKBOX_H
#define MIDICHECKBOX_H

#include <qcheckbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midiguicomponent.h"

class MidiCheckBox : public MidiGUIcomponent {

Q_OBJECT

public:
  QCheckBox *checkbox;
  
public:
  MidiCheckBox(bool on, QWidget * parent, const char * name=0);
  ~MidiCheckBox();
  virtual void setMidiValue(int value);
  virtual int getMidiValue();

signals:
  void valueChanged(int value);
	
public slots:
  void updateValue(bool on);
  void updateCheckBox(bool on);
    
};

#endif
