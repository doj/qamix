#include <qcombobox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qstrlist.h>
#include <stdio.h>
#include <math.h>
#include "midicombobox.h"
#include "midiguicomponent.h"

MidiComboBox::MidiComboBox(QStrList *itemNames, int value, QWidget * parent, const char * name)
           : MidiGUIcomponent(parent, name) {

  setMargin(5);
  QHBox *comboContainer = new QHBox(this);
  new QWidget (comboContainer);
  combobox = new QComboBox(comboContainer);
  new QWidget (comboContainer);
  combobox->insertStrList(itemNames);
  combobox->setCurrentItem(value);
  QObject::connect(combobox, SIGNAL(highlighted(int)), this, SLOT(updateValue(int)));
}

MidiComboBox::~MidiComboBox(){
}

void MidiComboBox::setMidiValue(int value) {

  if (!controllerOK) {
    if (value > 10000) {
      controllerOK = abs(value - 10000 - getMidiValue()) < 4;
    } else {
      controllerOK = abs(getMidiValue() - value) < 4;
    }
  } else {
    if (value > 10000) {
      combobox->setCurrentItem(int((float)(combobox->count()-1) / 127.0 * (float)(10127-value)));
    } else {
      combobox->setCurrentItem(int((float)(combobox->count()-1) / 127.0 * (float)value));
    }
  }
}

int MidiComboBox::getMidiValue() {
  
  int x;

  x = (int)rint(127.0 * combobox->currentItem() / (combobox->count()-1)); 
  return(x);
}

void MidiComboBox::updateValue(int value) {

  emit valueChanged(value);
  emit guiComponentTouched();
}

void MidiComboBox::updateComboBox(int value) {

  combobox->setCurrentItem(value);
}
