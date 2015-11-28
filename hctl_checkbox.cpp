#include <math.h>
#include <qcheckbox.h>
#include <qvbox.h>
#include <qhbox.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "hctl_checkbox.h"
#include "midicheckbox.h"
#include "midicontroller.h"

HctlCheckBox::HctlCheckBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name, int p_sign) 
                                        : HctlElement(p_hctl_elem, parent, name) {

  int l1, count, value;
  
  count = snd_ctl_elem_info_get_count(elem_info);
  sign = p_sign;
  QHBox *checkboxBox = new QHBox(this);
  for (l1 = 0; l1 < count; l1++) {
    value = snd_ctl_elem_value_get_boolean(elem_value, l1);
    MidiCheckBox *checkbox = new MidiCheckBox(sign > 0 ? value > 0 : value <= 0, checkboxBox, name);
    checkboxList.append(checkbox);
    QObject::connect(checkbox, SIGNAL(valueChanged(int)), this, SLOT(checkboxValueChanged(int)));
  }
  if (name) {
      QHBox *labelBox = new QHBox(this);
      new QWidget(labelBox);
      label = new QLabel(labelBox);
      label->setText(name);
      new QWidget(labelBox);
  }
  if (count > 1) {
    QHBox *lockContainer = new QHBox(this);
    new QWidget(lockContainer); 
    lockBox = new QCheckBox(lockContainer);
    new QWidget(lockContainer); 
    lockBox->setText("Lock");
    lockBox->setChecked(lock);
    QObject::connect(lockBox, SIGNAL(toggled(bool)), this, SLOT(lockToggled(bool)));
  }
  new QWidget(this);
}

HctlCheckBox::~HctlCheckBox() {

}

void HctlCheckBox::checkboxValueChanged(int val) {
  
  int index;

  if (!lock) {
    index = checkboxList.find((MidiCheckBox *)sender());
    if (sign > 0) {
      snd_ctl_elem_value_set_boolean(elem_value, index, val > 0);
    } else {
      snd_ctl_elem_value_set_boolean(elem_value, index, val <= 0);
    }
    snd_hctl_elem_write(hctl_elem, elem_value);
  } else {
    if (sign > 0) {
      for (index = 0; index < checkboxList.count(); index++) {
        snd_ctl_elem_value_set_boolean(elem_value, index, val > 0);
        snd_hctl_elem_write(hctl_elem, elem_value);
      }
    } else {
      for (index = 0; index < checkboxList.count(); index++) {
        snd_ctl_elem_value_set_boolean(elem_value, index, val <= 0);
        snd_hctl_elem_write(hctl_elem, elem_value);
      }
    }
  }
}

void HctlCheckBox::updateView() {

  int l1, val;

  for (l1 = 0; l1 < checkboxList.count(); l1++) {
    val = snd_ctl_elem_value_get_boolean(elem_value, l1);
    if (sign > 0) {
      checkboxList.at(l1)->updateCheckBox(val > 0);
    } else {
      checkboxList.at(l1)->updateCheckBox(val <= 0);
    }
  }
}

void HctlCheckBox::midiConnect(int index, MidiController *midiController) {

  checkboxList.at(index)->connectToController(midiController);
}
