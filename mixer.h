#ifndef MIXER_H
#define MIXER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <qstring.h>
#include <qlabel.h>
#include <qslider.h>
#include <qsocketnotifier.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qptrlist.h>
#include <qmessagebox.h>
#include <qlist.h>
#include <qxml.h>
#include <alsa/asoundlib.h>
#include "hctldata.h"
#include "hctl_element.h"
#include "gui.h"

#ifdef WITHKDE
#define QAMIX_SHARE_DIR "/usr/share/kamix"
#else
#define QAMIX_SHARE_DIR "/usr/share/qamix"
#endif

class Mixer : public QVBox
{
  Q_OBJECT

  private:
    QSocketNotifier *hctlNotifier;
 
  public:
    HctlData *hctlData;
    Gui *gui;

  private:
    static int hctl_callback(snd_hctl_t *hctl, unsigned int mask, snd_hctl_elem_t *elem);
    int initHctlNotifier();  

  public:
    Mixer(QString ctl_name, QString xml_name, int mode, QWidget* parent=0, const char *name=0);
    ~Mixer();

  public slots:
    void open_mixer(QString ctl_name);
    void close_mixer();
    void initControls();
    void listControls();
    void printDefaultGui();
    void midiAction(int fd);
    void hctlAction(int fd);
};
  
#endif
