#ifndef MIXLAUNCH_H
#define MIXLAUNCH_H

#include "mixer.h"
#include <QtCore/qnamespace.h>
#include <Qt3Support/Q3MainWindow>
#include <Qt3Support/Q3PopupMenu>

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <qobject.h>
#include <qwidget.h>
#include <qmainwindow.h>
#include <qvbox.h>
#include <qstring.h>
#include <qstringlist.h>
#include <alsa/asoundlib.h>
#endif

#ifdef WITHKDE
const QString aboutText = "KAMix 0.0.7\nby Matthias Nagorni\n"
                          "KDE Version by Arvin Schnell\n"
                          "(c)2003 SuSE AG Nuremberg\n\n"
                          "KAMix is licensed under the GPL.\n";
#else
const QString aboutText = "QAMix 0.0.7\nby Matthias Nagorni\n"
                          "KDE Version by Arvin Schnell\n"
                          "(c)2003 SuSE AG Nuremberg\n\n"
                          "QAMix is licensed under the GPL.\n";
#endif

class MixLaunch : public Q3VBox
{
  Q_OBJECT

  private:
    Q3MainWindow *mainWindow;
    QMessageBox *aboutWidget;
    Mixer *mixer;    
    QString ctl_name;
    QString xml_name;
    int mode;
    QSocketNotifier *seqNotifier;
    snd_seq_t *seqHandle;

  public:
    int hide_ids[4], channel;
    Q3PopupMenu *viewPopup, *devicePopup;
  
  public:
    MixLaunch(QString p_ctlname, QString p_xml_name, int p_mode, QWidget* parent, QString name);
    ~MixLaunch();
    snd_seq_t *open_seq();
    int initSeqNotifier();

  public slots: 
    void openMixer();
    void closeMixer();
    void displayAbout();
    void printDefaultGui();
    void newDevice(int id);
    void newDeviceParam(int id);
    void newMode(int id);
    void newModeParam(int id);
    void updateCaption();
    void setChannel(int p_channel);
};
  
#endif
