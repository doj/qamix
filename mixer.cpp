#include "gui.h"
#include "hctldata.h"
#include "hctl_element.h"
#include "parser.h"
#include "mixer.h"
#include "midicontroller.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>
#include <qlabel.h>
#include <qslider.h> 
#include <qsocketnotifier.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qmainwindow.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qxml.h>
#include <alsa/asoundlib.h>
#endif

Mixer::Mixer(QString ctl_name, QString xml_name, int mode, QWidget *parent, QString name) : Q3VBox(parent, name) {

  QString driver_name, xmldir, short_name, long_name, mixer_name;
  snd_ctl_card_info_t *card_info;

//  fprintf(stderr, "Mixer::Mixer %s %s %d\n", ctl_name.latin1(), xml_name.latin1(), mode);

  hctlData = new HctlData();
  open_mixer(ctl_name);
  initHctlNotifier();
  initControls();
  gui = new Gui(hctlData, this);

  Parser *parser = new Parser(hctlData, gui, mode);
  if (xml_name.contains("NO_NAME_SPECIFIED")) {
#ifdef WITHKDE
  xmldir = "/.kamix/";
#else
  xmldir = "/.qamix/";  
#endif
    snd_ctl_card_info_alloca(&card_info);
    snd_ctl_card_info(hctlData->ctl_handle, card_info);
    driver_name = QString(snd_ctl_card_info_get_driver(card_info));
    short_name = QString(snd_ctl_card_info_get_name(card_info));
    long_name = QString(snd_ctl_card_info_get_longname(card_info));
    mixer_name = QString(snd_ctl_card_info_get_mixername(card_info));
    driver_name = driver_name.stripWhiteSpace();
    short_name = short_name.stripWhiteSpace();
    long_name = long_name.stripWhiteSpace();
    mixer_name = mixer_name.stripWhiteSpace();
//    fprintf(stderr, "--> Name: %s\n", short_name.latin1());
//    fprintf(stderr, "--> Long Name: %s\n", long_name.latin1());
//    fprintf(stderr, "--> Mixer Name: %s\n", mixer_name.latin1());
    xml_name = QString(getenv("HOME")) + xmldir + driver_name.lower() + ".xml";    
//    printf("%s\n", xml_name.latin1());
    QFile f(xml_name);
    if (!f.exists()) {
      xml_name = QString(getenv("HOME")) + xmldir + "default.xml";
//      printf("%s\n", xml_name.latin1());
      QFile f(xml_name);
      if (!f.exists()) {
        xml_name = QString(QAMIX_SHARE_DIR) + "/" + driver_name.lower() + ".xml";
//        printf("%s\n", xml_name.latin1());
        QFile f(xml_name);
        if (!f.exists()) {
          xml_name = QString(QAMIX_SHARE_DIR) + "/default.xml";               
//          printf("%s\n", xml_name.latin1());
          QFile f(xml_name);
        }
      }
    }
  }
  QFile f(xml_name);
  if (!f.exists()) {
    fprintf(stderr, "File %s does not exist.\n", xml_name.latin1());
    exit(-1);
  }
  QXmlInputSource source(&f);
  QXmlSimpleReader reader;
  reader.setContentHandler(parser);
  reader.parse(source);
}

Mixer::~Mixer() {
  
  close_mixer();
  free(hctlData);
  delete hctlNotifier;
}

void Mixer::open_mixer(QString ctl_name) {

//  printf("Mixer::open_mixer(%s)\n", ctl_name.latin1());
  if (snd_hctl_open(&hctlData->hctl_handle, ctl_name.latin1(), SND_CTL_NONBLOCK) < 0) {
    fprintf(stderr, "Could not open CTL %s.\n", ctl_name.latin1());
    exit(-1);
  }
  if (snd_ctl_open(&hctlData->ctl_handle, ctl_name.latin1(), SND_CTL_NONBLOCK) < 0) {
    fprintf(stderr, "Could not open CTL %s.\n", ctl_name.latin1());
    exit(-1);
  }
  snd_hctl_nonblock(hctlData->hctl_handle, 1);
  if (snd_hctl_load(hctlData->hctl_handle) < 0) {
    fprintf(stderr, "Could not load parameters into HCTL.\n");
    exit(-1);
  }
  hctlData->element_count = snd_hctl_get_count(hctlData->hctl_handle);
  snd_hctl_set_callback_private(hctlData->hctl_handle, (void *)this);
  snd_hctl_set_callback(hctlData->hctl_handle, hctl_callback);
}

void Mixer::close_mixer() {

  QObject::disconnect(hctlNotifier, SIGNAL(activated(int)),
                      this, SLOT(hctlAction(int)));
  sleep(1);
  snd_hctl_close(hctlData->hctl_handle);  
  snd_ctl_close(hctlData->ctl_handle);
}

void Mixer::initControls() {

    int l1;
  QString qs1, qs2, qs3;
  snd_hctl_elem_t *hctl_elem;
  HctlElement *hctlElement;

  if (!hctlData->element_count) {
    return;
  }
  hctl_elem = snd_hctl_first_elem(hctlData->hctl_handle);
  hctlElement = new HctlElement(hctl_elem, 0, 0);
  hctlData->addHctlElement(hctlElement);
  for (l1 = 0; l1 < hctlData->element_count - 1; l1++) {
    hctl_elem = snd_hctl_elem_next(hctl_elem);
    hctlElement = new HctlElement(hctl_elem, 0, 0);
    hctlData->addHctlElement(hctlElement);
  }  
}

void Mixer::listControls() {

  int l1, l2, value_count, item_count; 
  long elem_min, elem_max, elem_step, val;
  QString qs1, qs2, qs3;
  snd_hctl_elem_t *hctl_elem;
  HctlElement *hctlElement;

  if (!hctlData->element_count) {
    return;
  }
  hctl_elem = snd_hctl_first_elem(hctlData->hctl_handle);
  hctlElement = new HctlElement(hctl_elem, 0, 0);
  for (l1 = 0; l1 < hctlData->element_count; l1++) {
    switch (hctlElement->elem_type) {
      case SND_CTL_ELEM_TYPE_NONE:
        qs1 = "None";
        break;
      case SND_CTL_ELEM_TYPE_BOOLEAN:
        val = snd_ctl_elem_value_get_boolean(hctlElement->elem_value, 0);
         qs1.sprintf("Boolean   Value: %ld", val);
        break;
      case SND_CTL_ELEM_TYPE_INTEGER:
        elem_min = snd_ctl_elem_info_get_min(hctlElement->elem_info);
        elem_max = snd_ctl_elem_info_get_max(hctlElement->elem_info);
        elem_step = snd_ctl_elem_info_get_step(hctlElement->elem_info);
        val = snd_ctl_elem_value_get_integer(hctlElement->elem_value, 0);
        qs1.sprintf("Integer   Value: %ld Min: %ld Max: %ld Step: %ld", val, elem_min, elem_max, elem_step);
        break;
      case SND_CTL_ELEM_TYPE_ENUMERATED:
        item_count = snd_ctl_elem_info_get_items(hctlElement->elem_info);
        qs1.sprintf("Enumerated   Items: %d", item_count);
        for (l2 = 0; l2 < item_count; l2++) {
          snd_ctl_elem_info_set_item(hctlElement->elem_info, l2);
          hctlElement->updateInfo();
          val = snd_ctl_elem_value_get_enumerated(hctlElement->elem_value, 0);
          qs3 = QString(snd_ctl_elem_info_get_item_name(hctlElement->elem_info));
          if (l2 == val) {
            printf("--> Item %d: %s\n", l2, qs3.latin1());
          } else {
            printf("    Item %d: %s\n", l2, qs3.latin1());
          }
        }
        break;
      case SND_CTL_ELEM_TYPE_BYTES:
        qs1 = "Bytes";
        break;
      case SND_CTL_ELEM_TYPE_IEC958:
        qs1 = "IEC958";
        break;
      case SND_CTL_ELEM_TYPE_INTEGER64:
        qs1 = "Integer 64";
        break;
    }
    qs2 = QString(snd_ctl_elem_info_get_name(hctlElement->elem_info));
    value_count = snd_ctl_elem_info_get_count(hctlElement->elem_info);
    printf("Name of element %3d: %s   Type: %s Count: %d\n", l1, qs2.latin1(), qs1.latin1(), value_count);
    if (l1 < hctlData->element_count - 1) {
      hctl_elem = snd_hctl_elem_next(hctl_elem);
      hctlElement = new HctlElement(hctl_elem, 0, 0);
    }
  }  
}

void Mixer::printDefaultGui() {

  int l1, index; 
  QString qs1, qs2;
  snd_hctl_elem_t *hctl_elem;
  HctlElement *hctlElement;

  if (!hctlData->element_count) {
    return;
  }
  hctl_elem = snd_hctl_first_elem(hctlData->hctl_handle);
  hctlElement = new HctlElement(hctl_elem, 0, 0);
  printf("<mixer>\n");
  printf("<full>\n");
  for (l1 = 0; l1 < hctlData->element_count; l1++) {
    qs1 = QString(snd_ctl_elem_info_get_name(hctlElement->elem_info));
    if ((index = snd_ctl_elem_info_get_index(hctlElement->elem_info)) > 0) {
      printf("      <element index=\"%d\">\n", index);
      qs2.sprintf("%s %d", qs1.latin1(), index);
    } else {
      printf("      <element>\n");
      qs2 = qs1;
    }
    printf("        <alsaname> %s </alsaname>\n", qs1.latin1());
    printf("        <mixname> %s </mixname>\n", qs2.latin1());
    printf("      </element>\n");
    if (l1 < hctlData->element_count - 1) {
      hctl_elem = snd_hctl_elem_next(hctl_elem);
      hctlElement = new HctlElement(hctl_elem, 0, 0);
    }
  }  
  printf("</full>\n\n");
  printf("<master>\n");
  printf("</master>\n\n");
  printf("<simple>\n");
  printf("</simple>\n");
  printf("</mixer>\n");
}

int Mixer::initHctlNotifier() {

/*
  int l1, pfd_count;

  pfd_count = snd_hctl_poll_descriptors_count(hctlData->hctl_handle);
  struct pollfd pfd[pfd_count];
  snd_hctl_poll_descriptors(hctlData->hctl_handle, pfd, pfd_count);
  for (l1 = 0; l1 < pfd_count; l1++) {
    QSocketNotifier *hctlNotifier = new QSocketNotifier(pfd[l1].fd, QSocketNotifier::Read);
    QObject::connect(hctlNotifier, SIGNAL(activated(int)),
                     this, SLOT(hctlAction(int)));
  }
  return(0);
*/

  int hctlEventFd = 0;

  struct pollfd pfd[1];
  snd_hctl_poll_descriptors(hctlData->hctl_handle, pfd, 1);
  hctlEventFd = pfd[0].fd;
//  fprintf(stderr, "hctlEventFd: %d\n", hctlEventFd);
  hctlNotifier = new QSocketNotifier(hctlEventFd, QSocketNotifier::Read);
  QObject::connect(hctlNotifier, SIGNAL(activated(int)),
                   this, SLOT(hctlAction(int)));
  return(0);
}

void Mixer::midiAction(int fd) {


  do {
  snd_seq_event_t *ev;
    snd_seq_event_input(hctlData->seq_handle, &ev);
    if ((ev->type == SND_SEQ_EVENT_CONTROLLER) 
      ||(ev->type == SND_SEQ_EVENT_PITCHBEND)
      ||(ev->type == SND_SEQ_EVENT_CONTROL14)
      ||(ev->type == SND_SEQ_EVENT_NOTEON)
      ||(ev->type == SND_SEQ_EVENT_NOTEOFF)) {
	//const int type = ev->type;
	const int ch = ev->data.control.channel;
	//const int param = (ev->type==SND_SEQ_EVENT_PITCHBEND) ? 0 : ev->data.control.param;
	const int val = ev->data.control.value;

      for(unsigned l1 = 0; l1 < hctlData->midiControllerList.count(); l1++) {
        auto midiController = hctlData->midiControllerList.at(l1);
	int mch;
        if (hctlData->channel >= 0) {
          mch = hctlData->channel;
        } else {
          mch = midiController->ch;
        }
        if ((midiController->type == ev->type) && (mch == ch)) {
          if (ev->type == SND_SEQ_EVENT_PITCHBEND) {
            midiController->sendMidiValue((val + 8192) / 128);
          }
          if ((ev->type == SND_SEQ_EVENT_CONTROL14)
	      && (midiController->param == static_cast<int>(ev->data.control.param))) {
            midiController->sendMidiValue(val / 128);
          }
          if ((ev->type == SND_SEQ_EVENT_CONTROLLER)
	      && (midiController->param == static_cast<int>(ev->data.control.param))) {
            midiController->sendMidiValue(val);
          }
          if ((ev->type == SND_SEQ_EVENT_NOTEON)
            && (midiController->param == ev->data.note.note)) {
            midiController->sendMidiValue(ev->data.note.velocity);
          }
          if ((ev->type == SND_SEQ_EVENT_NOTEOFF)
           && (midiController->param == ev->data.note.note)) {
             midiController->sendMidiValue(0);
          }
        }  
      }    

    }      
    snd_seq_free_event(ev);
  } while (snd_seq_event_input_pending(hctlData->seq_handle, 0) > 0);
}

void Mixer::hctlAction(int fd) {
 
  snd_hctl_handle_events(hctlData->hctl_handle);
}

int Mixer::hctl_callback(snd_hctl_t *hctl, unsigned int mask, snd_hctl_elem_t *elem) {

  return(0);
}
