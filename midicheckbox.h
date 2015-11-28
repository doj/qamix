#pragma once
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midiguicomponent.h"

class MidiCheckBox : public MidiGUIcomponent {

Q_OBJECT

public:
  QCheckBox *checkbox;
  
public:
  MidiCheckBox(bool on, QWidget * parent, QString name);
  ~MidiCheckBox();
  virtual void setMidiValue(int value);
  virtual int getMidiValue();

signals:
  void valueChanged(int value);
	
public slots:
  void updateValue(bool on);
  void updateCheckBox(bool on);
    
};
