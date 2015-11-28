#include "utils.h"
#include "mixlaunch.h"
#include "mixer.h"

#include <QtGui/QMenuBar>
#include <QtGui/QFileDialog>

MixLaunch::MixLaunch(QString p_ctlname, QString p_xml_name, int p_mode, QWidget *parent, QString name) : Q3VBox(parent, name) {

  mainWindow = (Q3MainWindow *)parent;

  ctl_name = p_ctlname;
  xml_name = p_xml_name;
  mode = p_mode;
  viewPopup = NULL;
  devicePopup = NULL;
  channel = -1;
  if (mode == 1) {
    mainWindow->menuBar()->hide();
  }
  aboutWidget = new QMessageBox(this);
  if ((seqHandle = open_seq()) != NULL) {
    initSeqNotifier();
  }
  mixer = new Mixer(ctl_name, xml_name, mode, this, "mixer");
  if (seqHandle) {
    mixer->hctlData->seq_handle = seqHandle;  
    QObject::connect(seqNotifier, SIGNAL(activated(int)),
                     mixer, SLOT(midiAction(int)));
  }
  updateCaption();
}

MixLaunch::~MixLaunch() {

  if (mixer) {
    delete(mixer);
  }  
}

void MixLaunch::openMixer() {
 
#ifdef WITHKDE
    xml_name = QString(QFileDialog::getOpenFileName("/usr/share/kamix", "KAMix GUI description (*.xml)"));
#else
    xml_name = QString(QFileDialog::getOpenFileName("/usr/share/qamix", "QAMix GUI description (*.xml)"));
#endif
    if (xml_name.isEmpty()) {
	return;
  } else {
    closeMixer();
//    printf("Opening new mixer %s\n", xml_name.latin1());
    mixer = new Mixer(ctl_name, xml_name, mode, this, "mixer");
    if (seqHandle) {
      mixer->hctlData->seq_handle = seqHandle;
      QObject::connect(seqNotifier, SIGNAL(activated(int)),
                       mixer, SLOT(midiAction(int)));
    }
    mixer->show();
  }
}

void MixLaunch::closeMixer() {

//  printf("Closing mixer\n");
  if (mixer) {
    delete(mixer);
  }
}

extern const char *aboutText;
void MixLaunch::displayAbout() {

#ifdef WITHKDE
    auto title = i18n("About KAMix");
#else
    auto title = i18n("About QAMix");
#endif
    aboutWidget->about(this, title, aboutText);
    aboutWidget->raise();
}

void MixLaunch::newDevice(int id) {

  int index;
  QString qs;

  auto deviceMenu = reinterpret_cast<Q3PopupMenu*>(sender());

  for (unsigned l1 = 0; l1 < deviceMenu->count(); l1++) {
    deviceMenu->setItemChecked(l1, false);
  }
  deviceMenu->setItemChecked(id, true);
  qs = deviceMenu->text(id);
//  printf("deviceMenu->text(%d): %s\n", id, qs.latin1()); 
  index = qs.findRev("(hw:");
  ctl_name = qs.mid(index + 1, 4);
  xml_name = "NO_NAME_SPECIFIED"; // Automatic search for GUI description
//  printf("ctl_name: %s\n", ctl_name.latin1());
  closeMixer();
  mixer = new Mixer(ctl_name, xml_name, mode, this, "mixer");
  if (seqHandle) {
    mixer->hctlData->seq_handle = seqHandle;
    QObject::connect(seqNotifier, SIGNAL(activated(int)),
                     mixer, SLOT(midiAction(int)));
  }
  mixer->show();
  updateCaption();
}

void MixLaunch::newDeviceParam(int id) {

  int index;
  QString qs;

#if 0
  Q3PopupMenu *deviceMenu = (Q3PopupMenu *)sender();
//  for (l1 = 0; l1 < deviceMenu->count(); l1++) {
//    deviceMenu->setItemChecked(l1, false);
//  }
//  deviceMenu->setItemChecked(id, true);
#endif
  for (unsigned l1 = 0; l1 < devicePopup->count(); l1++) {
    devicePopup->setItemChecked(l1, false);
  }
  devicePopup->setItemChecked(id, true);
  qs = devicePopup->text(id);
//  fprintf(stderr, "id = %d, qs = %s\n", id, qs.latin1());
//  printf("deviceMenu->text(%d): %s\n", id, qs.latin1()); 
  index = qs.findRev("(hw:");
  ctl_name = qs.mid(index + 1, 4);
  xml_name = "NO_NAME_SPECIFIED"; // Automatic search for GUI description
//  printf("ctl_name: %s\n", ctl_name.latin1());
  closeMixer();
  mixer = new Mixer(ctl_name, xml_name, mode, this, "mixer");
  if (seqHandle) {
    mixer->hctlData->seq_handle = seqHandle;
    QObject::connect(seqNotifier, SIGNAL(activated(int)),
                     mixer, SLOT(midiAction(int)));
  }
  mixer->show();
  updateCaption();
}

void MixLaunch::updateCaption() {

  QString driver_name, qs;
  snd_ctl_card_info_t *card_info;

  snd_ctl_card_info_alloca(&card_info);
  snd_ctl_card_info(mixer->hctlData->ctl_handle, card_info);
  driver_name = QString(snd_ctl_card_info_get_driver(card_info));
#ifdef WITHKDE
  qs.sprintf("KAMix " QAMIX_VERSION "  -  %s", driver_name.latin1());
#else
  qs.sprintf("QAMix " QAMIX_VERSION "  -  %s", driver_name.latin1());
#endif
  mainWindow->setCaption(qs);
}

void MixLaunch::printDefaultGui() {

  mixer->printDefaultGui();
}

void MixLaunch::newMode(int id) {

  int l1;

  Q3PopupMenu *viewMenu = (Q3PopupMenu *)sender();
  mode = id;
#ifndef WITHKDE
  if (mode == 1) {
    for (l1 = 0; l1 < 3; l1++) {
      mainWindow->menuBar()->setItemVisible(hide_ids[l1], false);
    }
  } else {
   for (l1 = 0; l1 < 3; l1++) {
      mainWindow->menuBar()->setItemVisible(hide_ids[l1], true);
    }
  }
  for (unsigned l1 = 0; l1 < viewMenu->count(); l1++) {
    viewMenu->setItemChecked(l1, false);
  }
  viewMenu->setItemChecked(mode, true);
#endif
  closeMixer();
  mixer = new Mixer(ctl_name, xml_name, mode, this, "mixer");
  if (seqHandle) {
    mixer->hctlData->seq_handle = seqHandle;
    QObject::connect(seqNotifier, SIGNAL(activated(int)),
                     mixer, SLOT(midiAction(int)));
  }
  mixer->show();  
}

void MixLaunch::newModeParam(int id) {

  int l1;

//  fprintf(stderr, "id = %d\n", id);
  //Q3PopupMenu *viewMenu = (Q3PopupMenu *)sender();
//  mode = viewMenu->itemParameter(id);
  mode = id;
//  fprintf(stderr, "mode = %d\n", mode);
#ifndef WITHKDE
  if (mode == 1) {
    for (l1 = 0; l1 < 3; l1++) {
      mainWindow->menuBar()->setItemVisible(hide_ids[l1], false);
    }
  } else {
   for (l1 = 0; l1 < 3; l1++) {
      mainWindow->menuBar()->setItemVisible(hide_ids[l1], true);
    }
  }
//  for (l1 = 0; l1 < viewMenu->count(); l1++) {
//    viewMenu->setItemChecked(l1, false);
//  }
  for (unsigned l1 = 0; l1 < viewPopup->count(); l1++) {
    viewPopup->setItemChecked(l1, false);
  }
//  viewMenu->setItemChecked(id, true);
  viewPopup->setItemChecked(id, true);
#endif
  closeMixer();
  mixer = new Mixer(ctl_name, xml_name, mode, this, "mixer");
  if (seqHandle) {
    mixer->hctlData->seq_handle = seqHandle;
    QObject::connect(seqNotifier, SIGNAL(activated(int)),
                     mixer, SLOT(midiAction(int)));
  }
  mixer->show();  
}

snd_seq_t *MixLaunch::open_seq() {

  snd_seq_t *seq_handle;
  int portid;
  int clientid;
//  QString qs;

  if (snd_seq_open(&seq_handle, "hw", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
    fprintf(stderr, "Error opening ALSA sequencer.\n");
    return(NULL);
//    exit(1);
  }
#ifdef WITHKDE
  snd_seq_set_client_name(seq_handle, "KAMix");
#else
  snd_seq_set_client_name(seq_handle, "QAMix");
#endif
  clientid = snd_seq_client_id(seq_handle);
  (void)clientid;
  if ((portid = snd_seq_create_simple_port(seq_handle, "qamix",
            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "Error creating sequencer port.\n");
    exit(1);
  }
//  qs.sprintf("QAMix " QAMIX_VERSION " - %d:%d", clientid, portid);
//  ((QMainWindow *)parent())->setCaption(qs);
  return(seq_handle);
}
 
int MixLaunch::initSeqNotifier() {

  int alsaEventFd = 0;

  struct pollfd pfd[1];
  snd_seq_poll_descriptors(seqHandle, pfd, 1, POLLIN);
  alsaEventFd = pfd[0].fd;
//  fprintf(stderr, "alsaEventFd: %d\n", alsaEventFd);
  seqNotifier = new QSocketNotifier(alsaEventFd, QSocketNotifier::Read);
  return(0);
}

void MixLaunch::setChannel(int p_channel) {

  channel = p_channel;
  mixer->hctlData->channel = channel;  
}
