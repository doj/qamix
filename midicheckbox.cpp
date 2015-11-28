#include "midicheckbox.h"
#include "midiguicomponent.h"
#include "defines.h"

MidiCheckBox::MidiCheckBox(bool on, QWidget * parent, QString name)
           : MidiGUIcomponent(parent, name) {

  auto checkContainer = new Q3HBox(this);
  new QWidget (checkContainer);
  checkbox = new QCheckBox(checkContainer);
  new QWidget (checkContainer);
  checkbox->setChecked(on);
  QObject::connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(updateValue(bool)));
}

MidiCheckBox::~MidiCheckBox(){
}

void MidiCheckBox::setMidiValue(int value) {

  if (value > NEGATIVE_VALUE_OFFSET) {
    checkbox->setChecked((value - NEGATIVE_VALUE_OFFSET) <= 63);
  } else {
    checkbox->setChecked(value > 63);
  }
}

int MidiCheckBox::getMidiValue() {
  
  int x;
 
  x = checkbox->isChecked() ? 127 : 0;
  return(x);
}

void MidiCheckBox::updateValue(bool on) {

  emit valueChanged(on ? 1 : 0);
  emit guiComponentTouched();
}

void MidiCheckBox::updateCheckBox(bool on) {

  checkbox->setChecked(on);
}
