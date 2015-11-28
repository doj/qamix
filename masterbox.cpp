#include <stdio.h>
#include <stdlib.h>
#include <qvbox.h>
#include "masterbox.h"

MasterBox::MasterBox(QWidget *parent, const char *name, WFlags flags) : QVBox(parent, name, flags) {

  hideOnLeave = false;
}

MasterBox::~MasterBox() {
  
}

void MasterBox::leaveEvent(QEvent *ev) {

  if (hideOnLeave) {
    hide();
  }
}   

void MasterBox::setHideOnLeave(bool on) {

  hideOnLeave = on;
}
