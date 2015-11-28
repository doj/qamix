#include "hctldata.h"
#include "hctl_element.h"
#include "midicontroller.h"
#include "midicontrollerlist.h"

HctlData::HctlData() {

  channel = -1;
}

HctlData::~HctlData() {
  
}

void HctlData::addHctlElement(HctlElement *hctlElement) {

  hctlElementList.append(hctlElement);
}

void HctlData::addGuiHctlElement(HctlElement *hctlElement) {

  guiHctlElementList.append(hctlElement);
}

void HctlData::addMidiController(MidiController *midiController) {

  midiControllerList.append(midiController);
}
