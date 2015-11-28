#ifndef MIDICOMBOBOX_H
#define MIDICOMBOBOX_H

#include <qcombobox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qstrlist.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midiguicomponent.h"

class MidiComboBox : public MidiGUIcomponent {

Q_OBJECT

public:
  QComboBox *combobox;
  
public:
  MidiComboBox(QStrList *itemNames, int value, QWidget * parent, const char * name=0);
  ~MidiComboBox();
  virtual void setMidiValue(int value);
  virtual int getMidiValue();

signals:
  void valueChanged(int value);
	
public slots:
  void updateValue(int value);
  void updateComboBox(int value);
    
};

#endif
