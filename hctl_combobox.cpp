#include <math.h>
#include <qcombobox.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qstrlist.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "hctl_combobox.h"
#include "midicombobox.h"
#include "midicontroller.h"

HctlComboBox::HctlComboBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name) 
                                        : HctlElement(p_hctl_elem, parent, name) {

  int l1, l2, item_count, count, value;
  QString qs;
  
  count = snd_ctl_elem_info_get_count(elem_info);
  QHBox *comboboxBox = new QHBox(this);
  for (l1 = 0; l1 < count; l1++) {
    item_count = snd_ctl_elem_info_get_items(elem_info);
    QStrList *itemNames = new QStrList(true);
    for (l2 = 0; l2 < item_count; l2++) {
      snd_ctl_elem_info_set_item(elem_info, l2);
      updateInfo();
      qs = QString(snd_ctl_elem_info_get_item_name(elem_info));
      itemNames->append(qs);
    }
    value = snd_ctl_elem_value_get_enumerated(elem_value, l1);
    MidiComboBox *combobox = new MidiComboBox(itemNames, value, comboboxBox, name);
    comboboxList.append(combobox);
    QObject::connect(combobox, SIGNAL(valueChanged(int)), this, SLOT(comboboxValueChanged(int)));
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
}

HctlComboBox::~HctlComboBox() {

}

void HctlComboBox::comboboxValueChanged(int val) {
  
  int index;

  if (!lock) {
    index = comboboxList.find((MidiComboBox *)sender());
    snd_ctl_elem_value_set_enumerated(elem_value, index, val);
    snd_hctl_elem_write(hctl_elem, elem_value);
  } else {
    for (index = 0; index < comboboxList.count(); index++) {
      snd_ctl_elem_value_set_enumerated(elem_value, index, val);
      snd_hctl_elem_write(hctl_elem, elem_value);
    }
  }
}

void HctlComboBox::updateView() {

  int l1, val;

  for (l1 = 0; l1 < comboboxList.count(); l1++) {
    val = snd_ctl_elem_value_get_enumerated(elem_value, l1);
    comboboxList.at(l1)->updateComboBox(val);
  }
}

void HctlComboBox::midiConnect(int index, MidiController *midiController) {

  comboboxList.at(index)->connectToController(midiController);
}
