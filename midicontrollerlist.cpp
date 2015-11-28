#include "midicontrollerlist.h"
#include "midicontroller.h"

MidiControllerList::MidiControllerList(){
}

MidiControllerList::~MidiControllerList(){
}

int MidiControllerList::compareItems(MidiController * item1, MidiController * item2) {

    if ((((MidiController *)item1)->type==((MidiController *)item2)->type)
      &&(((MidiController *)item1)->ch==((MidiController *)item2)->ch)
      &&(((MidiController *)item1)->param==((MidiController *)item2)->param)) {
      return(0);
    } else {
      return(1);
    }
}
