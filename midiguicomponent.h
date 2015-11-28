#pragma once
#include "hctldata.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

class MidiGUIcomponent : public Q3VBox {

Q_OBJECT

public:
  bool controllerOK;
        
public:
  MidiGUIcomponent(QWidget * parent, QString name);
  ~MidiGUIcomponent();
  MidiControllerList midiControllerList;
  void connectToController(MidiController *p_midiController);
  void disconnectController(MidiController *p_midiController);
  virtual void setMidiValue(int value);
  virtual int getMidiValue();

signals:
  void guiComponentTouched();
  
public slots:
  void midiValueChanged(int);
  void disconnectController(int);
};
