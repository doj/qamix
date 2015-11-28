#ifndef HCTLDATA_H
#define HCTLDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <qobject.h>
#include <qstring.h>
#include <qptrlist.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

typedef QValueList<int> IntList;

class HctlData : public QObject
{
  public:
    QPtrList<HctlElement> hctlElementList;
    QPtrList<HctlElement> guiHctlElementList;
    MidiControllerList midiControllerList;
    snd_hctl_t *hctl_handle;
    snd_ctl_t *ctl_handle;
    int element_count;
    snd_seq_t *seq_handle;
    int channel;
      
  public:
    HctlData();
    ~HctlData();
    void addHctlElement(HctlElement *hctlElement);
    void addGuiHctlElement(HctlElement *hctlElement);
    void addMidiController(MidiController *midiController);
    
};
  
#endif
