#include "midicontroller.h"

MidiController::MidiController(){

  type = -1;
  ch = -1;
  param = -1;
  sign = 1;
}

MidiController::MidiController(int p_type, int p_ch, int p_param, int p_sign) {

  type = p_type;
  ch = p_ch;
  param = p_param;
  sign = p_sign;
}

MidiController::~MidiController(){
}

void MidiController::sendMidiValue(int value) {

  if (sign > 0) {
    emit midiValueChanged(value);
  } else {
    emit midiValueChanged(10000 + value);
  }
}
