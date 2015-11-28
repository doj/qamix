#include <qvbox.h>
#include <stdio.h>
#include "midicontroller.h"
#include "midicontrollerlist.h"
#include "midiguicomponent.h"

MidiGUIcomponent::MidiGUIcomponent(QWidget * parent, const char * name)
           : QVBox(parent, name) {

  controllerOK = false;
}

MidiGUIcomponent::~MidiGUIcomponent() {
}

void MidiGUIcomponent::connectToController(MidiController *p_midiController) {

//  if (!midiControllerList.contains(p_midiController)) {
    QObject::connect(p_midiController, SIGNAL(midiValueChanged(int)),
                     this, SLOT(midiValueChanged(int)));  
    midiControllerList.append(p_midiController);
    controllerOK = false;
//  }
}
   
void MidiGUIcomponent::disconnectController(MidiController *p_midiController) {

  QObject::disconnect(p_midiController, SIGNAL(midiValueChanged(int)),
                      this, SLOT(midiValueChanged(int)));
  midiControllerList.remove(p_midiController);
}  

void MidiGUIcomponent::disconnectController(int index) {

  if (midiControllerList.count()) {
    QObject::disconnect(midiControllerList.at(index), SIGNAL(midiValueChanged(int)),
                       this, SLOT(midiValueChanged(int)));
    midiControllerList.remove(midiControllerList.at(index));
  }
}

void MidiGUIcomponent::setMidiValue(int value) {

}

int MidiGUIcomponent::getMidiValue() {

  return(0);
}

void MidiGUIcomponent::midiValueChanged(int value) {

  setMidiValue(value);
}
