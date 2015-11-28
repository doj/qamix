#include <qvbox.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"

HctlElement::HctlElement(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, const char * name)
                                        : QVBox(parent, name) {

  hctl_elem = p_hctl_elem;
  snd_ctl_elem_info_malloc(&elem_info);
  snd_ctl_elem_value_malloc(&elem_value);
  snd_hctl_elem_set_callback_private(hctl_elem, (void *)this);
  snd_hctl_elem_set_callback(hctl_elem, hctl_callback);
  updateInfo();
  updateValue();
  elem_type = snd_ctl_elem_info_get_type(elem_info);
  lock = true;
  lockBox = 0;
}

HctlElement::~HctlElement() {

  free(elem_info);
  free(elem_value);
}

void HctlElement::updateInfo() {

  if (snd_hctl_elem_info(hctl_elem, elem_info) < 0) {
    fprintf(stderr, "Could not get info on element.\n");
    exit(-1);
  }
}

void HctlElement::updateValue() {

  if (snd_hctl_elem_read(hctl_elem, elem_value) < 0) {
//    fprintf(stderr, "Could not read element.\n");
//    exit(-1);
  }
  updateView();
}

void HctlElement::updateView() {

}

int HctlElement::hctl_callback(snd_hctl_elem_t *elem, unsigned int mask) {

  HctlElement *hctlElement;

  hctlElement = (HctlElement *)snd_hctl_elem_get_callback_private(elem);
  hctlElement->updateValue();
  return(0);
}

void HctlElement::midiConnect(int index, MidiController *midiController) {

}

void HctlElement::lockToggled(bool on) {

  if (lockBox) {
   lock = lockBox->isChecked();
  }
}
