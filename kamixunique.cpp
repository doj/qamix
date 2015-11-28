#include <stdio.h>
#include <stdlib.h>
#include <qdesktopwidget.h>
#include "kamixunique.h"

KAMixUnique::KAMixUnique(bool allowStyles, bool GUIenabled, bool configUnique) : KUniqueApplication(allowStyles, GUIenabled, configUnique) {

  running = false;                            
  mixerWindow = NULL;
}

KAMixUnique::~KAMixUnique() {
  
}

int KAMixUnique::newInstance() {

  if (running) {
    if (mixerWindow) {
      mixerWindow->hide(); // Needed to let the window move to the current desktop on show()
      mixerWindow->show();
      mixerWindow->raise();
    }
  }
  running = true;
  return(0);  
}

void KAMixUnique::setMixerWindow(QWidget *widget) {

  mixerWindow = widget;
}
