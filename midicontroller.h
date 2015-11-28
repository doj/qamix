#ifndef MIDICONTROLLER_H
#define MIDICONTROLLER_H

#include <qobject.h>

class MidiController : public QObject  {
    
  Q_OBJECT
    
  public:
    int type;
    int ch;
    int param;
    int sign;
          
  public:
    MidiController();
    MidiController(int p_type, int p_ch, int p_param, int p_sign);
    ~MidiController();
    void sendMidiValue(int value);

  signals:
    void midiValueChanged(int);
};
                              
#endif
