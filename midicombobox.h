#ifndef MIDICOMBOBOX_H
#define MIDICOMBOBOX_H

#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include "hctldata.h"
#include "midicontroller.h"
#include "midiguicomponent.h"
#include <Qt3Support/Q3StrList>

class MidiComboBox : public MidiGUIcomponent {

Q_OBJECT

public:
  QComboBox *combobox;
  
public:
  MidiComboBox(const QStringList& itemNames, int value, QWidget * parent, QString name);
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
