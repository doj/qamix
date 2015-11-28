#include "hctl_element.h"
#include "hctl_slider.h"
#include "midislider.h"
#include "midicontroller.h"
#include <math.h>
#include <QtGui/QFontMetrics>
#include <alsa/asoundlib.h>

HctlSlider::HctlSlider(snd_hctl_elem_t *p_hctl_elem, QWidget * parent, QString name, sliderValueStyleEnum p_sliderValueStyle, bool p_single) 
                                        : HctlElement(p_hctl_elem, parent, name) {

  long min, max, step, value;
  int l1, count;
  QString qs;
    
  count = snd_ctl_elem_info_get_count(elem_info);
  single = p_single;
  sliderValueStyle = p_sliderValueStyle;
  Q3HBox *sliderBox = new Q3HBox(this);
  if (single) {
    count = 1;
  }
  for (l1 = 0; l1 < count; l1++) {
    min = snd_ctl_elem_info_get_min(elem_info);
    max = snd_ctl_elem_info_get_max(elem_info);
    step = snd_ctl_elem_info_get_step(elem_info);
    value = snd_ctl_elem_value_get_integer(elem_value, l1);
    Q3VBox *vBox = new Q3VBox(sliderBox);
    if (sliderValueStyle) {
      Q3HBox *labelBox = new Q3HBox(vBox);
      new QWidget(labelBox);
      QLabel *number = new QLabel(labelBox);
      new QWidget(labelBox);
      numberList.append(number);
      new QWidget(labelBox);
      if ((sliderValueStyle == sliderValueStyle_SimpleNumbers100)
          ||(sliderValueStyle == sliderValueStyle_FrameNumbers100)) {
        qs.sprintf("%3d", (int)(100.0 * value / (double)max));
      } else {
        qs.sprintf("%3d", (int)value);
      }
      if ((sliderValueStyle==sliderValueStyle_FrameNumbers)
        ||(sliderValueStyle==sliderValueStyle_FrameNumbers100)) {
        QFontMetrics *qfm = new QFontMetrics(number->font());
        number->setFrameStyle (QFrame::Panel | QFrame::Sunken);
        number->setMinimumWidth(qfm->width("MMM", 3));
        number->setLineWidth(3);
        /* \todo
	   number->setPaletteBackgroundColor(QColor(190, 250, 190));
	   number->setBackgroundRole(QPalette::ColorRole role);
	*/
        delete qfm;
      }
      number->setText(qs);
    }
    MidiSlider *slider = new MidiSlider(min, max, step, max - value, Qt::Vertical, vBox, name);
    sliderList.append(slider);
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
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
  new QWidget(this);
}

HctlSlider::~HctlSlider() {

}

void HctlSlider::sliderValueChanged(int val) {
  
  int index, max, count;
  QString qs;

  count = snd_ctl_elem_info_get_count(elem_info);
  if (single) {
    max = sliderList.at(0)->slider->maximum();
    if (sliderValueStyle) {
      if ((sliderValueStyle == sliderValueStyle_SimpleNumbers100)
        ||(sliderValueStyle == sliderValueStyle_FrameNumbers100)) {
        qs.sprintf("%3d", (int)(100.0 * (double)(max - val) / (double)max));
      } else {
        qs.sprintf("%3d", max - val);
      }
      numberList.at(0)->setText(qs);
    }
    for (index = 0; index < count; index++) {
      snd_ctl_elem_value_set_integer(elem_value, index, max - val);
      snd_hctl_elem_write(hctl_elem, elem_value);
    }
  } else {
    if (!lock) {
      index = sliderList.find((MidiSlider *)sender());
      max = ((MidiSlider *)sender())->slider->maximum();
      if (sliderValueStyle) {
        if ((sliderValueStyle == sliderValueStyle_SimpleNumbers100)
          ||(sliderValueStyle == sliderValueStyle_FrameNumbers100)) {
          qs.sprintf("%3d", (int)(100.0 * (double)(max - val) / (double)max));
        } else {
          qs.sprintf("%3d", max - val);
        }
        numberList.at(index)->setText(qs);
      }
      snd_ctl_elem_value_set_integer(elem_value, index, max - val);
      snd_hctl_elem_write(hctl_elem, elem_value);
    } else {
      for (unsigned index = 0; index < sliderList.count(); ++index) {
        max = sliderList.at(index)->slider->maximum();
        if (sliderValueStyle) {
          if ((sliderValueStyle == sliderValueStyle_SimpleNumbers100) 
            ||(sliderValueStyle == sliderValueStyle_FrameNumbers100)) {
            qs.sprintf("%3d", (int)(100.0 * (double)(max - val) / (double)max));
          } else {
            qs.sprintf("%3d", max - val);
          }
          numberList.at(index)->setText(qs);
        }
        snd_ctl_elem_value_set_integer(elem_value, index, max - val);
        snd_hctl_elem_write(hctl_elem, elem_value);
      }
    }
  }
}

void HctlSlider::updateView() {

  int val, max;
  QString qs;

  for (unsigned l1 = 0; l1 < sliderList.count(); ++l1) {
    max = sliderList.at(l1)->slider->maximum();
    val = snd_ctl_elem_value_get_integer(elem_value, l1);
    sliderList.at(l1)->updateSlider(max - val);
    if (sliderValueStyle) {
      if ((sliderValueStyle == sliderValueStyle_SimpleNumbers100) 
        ||(sliderValueStyle == sliderValueStyle_FrameNumbers100)) {
        qs.sprintf("%3d", (int)((double)(val) / (double)max * 100.0));
      } else {
        qs.sprintf("%3d", val);
      }
      numberList.at(l1)->setText(qs);      
    }
  }
}

void HctlSlider::midiConnect(int index, MidiController *midiController) {

  sliderList.at(index)->connectToController(midiController);
}
