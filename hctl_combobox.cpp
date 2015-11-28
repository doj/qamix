#include "hctl_element.h"
#include "hctl_combobox.h"
#include "midicombobox.h"
#include "midicontroller.h"
#include <math.h>
#include <alsa/asoundlib.h>
#include <Qt3Support/Q3VBox>

HctlComboBox::HctlComboBox(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, QString name)
                                        : HctlElement(p_hctl_elem, parent, name) {

  int l1, l2, item_count, count, value;

  count = snd_ctl_elem_info_get_count(elem_info);
  Q3HBox *comboboxBox = new Q3HBox(this);
  for (l1 = 0; l1 < count; l1++) {
    item_count = snd_ctl_elem_info_get_items(elem_info);
    QStringList itemNames;
    for (l2 = 0; l2 < item_count; l2++) {
      snd_ctl_elem_info_set_item(elem_info, l2);
      updateInfo();
      itemNames.append(snd_ctl_elem_info_get_item_name(elem_info));
    }
    value = snd_ctl_elem_value_get_enumerated(elem_value, l1);
    MidiComboBox *combobox = new MidiComboBox(itemNames, value, comboboxBox, name);
    comboboxList.append(combobox);
    QObject::connect(combobox, SIGNAL(valueChanged(int)), this, SLOT(comboboxValueChanged(int)));
  }
  if (! name.isEmpty()) {
      Q3HBox *labelBox = new Q3HBox(this);
      new QWidget(labelBox);
      label = new QLabel(labelBox);
      label->setText(name);
      new QWidget(labelBox);
  }
  if (count > 1) {
    Q3HBox *lockContainer = new Q3HBox(this);
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

  unsigned index;

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

  for (unsigned l1 = 0; l1 < comboboxList.count(); l1++) {
    const int val = snd_ctl_elem_value_get_enumerated(elem_value, l1);
    comboboxList.at(l1)->updateComboBox(val);
  }
}

void HctlComboBox::midiConnect(int index, MidiController *midiController) {

  comboboxList.at(index)->connectToController(midiController);
}
