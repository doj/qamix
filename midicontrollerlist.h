#ifndef MIDICONTROLLERLIST_H
#define MIDICONTROLLERLIST_H

#include <qptrlist.h>
#include <qcollection.h>

#include "midicontroller.h"

class MidiControllerList : public QPtrList<MidiController> {

  public:
    MidiControllerList();
    ~MidiControllerList();
    int compareItems(QCollection::Item item1, QCollection::Item item2 );
};
                     
#endif
