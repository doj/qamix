#ifndef HCTLDATA_H
#define HCTLDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <alsa/asoundlib.h>
#include "hctl_element.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"
#include <Qt3Support/Q3ValueList>
#include <Qt3Support/Q3PtrList>

typedef Q3ValueList<int> IntList;

class HctlData : public QObject
{
  public:
    Q3PtrList<HctlElement> hctlElementList;
    Q3PtrList<HctlElement> guiHctlElementList;
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
