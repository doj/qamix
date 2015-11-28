#ifndef MIDIGUICOMPONENT_H
#define MIDIGUICOMPONENT_H

#include <qptrlist.h>
#include <qvbox.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

class MidiGUIcomponent : public QVBox {

Q_OBJECT

public:
  bool controllerOK;
        
public:
  MidiGUIcomponent(QWidget * parent, const char * name=0);
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
  
#endif
