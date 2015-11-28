#include "midicombobox.h"
#include "midiguicomponent.h"
#include "defines.h"

MidiComboBox::MidiComboBox(const QStringList& itemNames, int value, QWidget * parent, QString name)
           : MidiGUIcomponent(parent, name) {

  setMargin(5);
  auto comboContainer = new Q3HBox(this);
  new QWidget (comboContainer);
  combobox = new QComboBox(comboContainer);
  new QWidget (comboContainer);
  combobox->addItems(itemNames);
  combobox->setCurrentItem(value);
  QObject::connect(combobox, SIGNAL(highlighted(int)), this, SLOT(updateValue(int)));
}

MidiComboBox::~MidiComboBox(){
}

void MidiComboBox::setMidiValue(int value) {

  if (!controllerOK) {
    if (value > NEGATIVE_VALUE_OFFSET) {
      controllerOK = abs(value - NEGATIVE_VALUE_OFFSET - getMidiValue()) < 4;
    } else {
      controllerOK = abs(getMidiValue() - value) < 4;
    }
  } else {
    if (value > NEGATIVE_VALUE_OFFSET) {
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
