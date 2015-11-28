#ifndef KAMIXUNIQUE_H
#define KAMIXUNIQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <kuniqueapplication.h>
#include <qwidget.h>

class KAMixUnique : public KUniqueApplication 
{
  public:
    KAMixUnique(bool allowStyles=true, bool GUIenabled=true, bool configUnique=false);
    ~KAMixUnique();
  virtual int newInstance();
  void setMixerWindow(QWidget *widget);
  
  private:
    bool running;
    QWidget *mixerWindow;
};
  
#endif
