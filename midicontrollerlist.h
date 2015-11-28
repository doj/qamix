#pragma once
#include "midicontroller.h"
#include <Qt3Support/Q3PtrList>

class MidiControllerList : public Q3PtrList<MidiController> {

  public:
    MidiControllerList();
    ~MidiControllerList();
    int compareItems(MidiController * item1, MidiController * item2);
};
