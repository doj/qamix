#ifndef MASTERBOX_H
#define MASTERBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <qvbox.h>

class MasterBox : public QVBox
{
  Q_OBJECT

  private:
    bool hideOnLeave;

  public:
    MasterBox(QWidget* parent=0, const char *name=0, WFlags flags = 0);
    ~MasterBox();
    void setHideOnLeave(bool on);

  private:
    void leaveEvent(QEvent *);
    
};
  
#endif
