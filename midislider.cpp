#include "midislider.h"
#include "midiguicomponent.h"
#include "defines.h"

MidiSlider::MidiSlider(int minValue, int maxValue, int pageStep, int value,
                       Qt::Orientation orientation, QWidget * parent, QString name)
           : MidiGUIcomponent(parent, name) {

  auto sliderContainer = new Q3HBox(this);
  new QWidget (sliderContainer);
  slider = new QSlider(minValue, maxValue, pageStep, 
                       value, orientation, sliderContainer);
  slider->setInvertedAppearance(true);
  new QWidget (sliderContainer);
  slider->setTickInterval((abs(maxValue) + abs(minValue)) / 10);
  slider->setTickmarks(QSlider::Left);
  QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));
}

MidiSlider::~MidiSlider(){
}

void MidiSlider::setMidiValue(int value) {

    // always enable the slider
    controllerOK = true;

  if (!controllerOK) {
    if (value > NEGATIVE_VALUE_OFFSET) {
      controllerOK = abs(value - NEGATIVE_VALUE_OFFSET - getMidiValue()) < 4;
    } else {
      controllerOK = abs(getMidiValue() - value) < 4;
    }
  } else {
    if (value > NEGATIVE_VALUE_OFFSET) {
      slider->setValue((int)(slider->minValue() + rint(float(slider->maxValue() - slider->minValue()) * (float)(value-NEGATIVE_VALUE_OFFSET) / 127.0)));
    } else {
      slider->setValue((int)(slider->maxValue() - rint(float(slider->maxValue() - slider->minValue()) * (float)value / 127.0)));
    }
  }
}

int MidiSlider::getMidiValue() {
  
  int x;
 
  x = (int)(rint(float(slider->value() -  slider->minValue())) * 127.0 / float(slider->maxValue() - slider->minValue()));
  return(x);
}

void MidiSlider::updateValue(int value) {

  emit valueChanged(value);
  emit guiComponentTouched();
}

void MidiSlider::updateSlider(int value) {

  slider->setValue(value);
}
