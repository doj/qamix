#include "main.h"
#include "utils.h"
#include "parser.h"
#include "mixlaunch.h"

#include <getopt.h>
#include <QtCore/QTranslator>
#include <QtGui/QMainWindow>
#include <Qt3Support/Q3PopupMenu>
#include <QtGui/QMenuBar>

#include <QtCore/QTextCodec>

const char *aboutText =
    "QAMix " QAMIX_VERSION "\nby Matthias Nagorni\n"
    "QAMix is licensed under the GPL2.\n";

MyConfig* myconfig;

QStringList card_list;


void
read_cards ()
{
    snd_ctl_t *ctl_handle;
    snd_ctl_card_info_t *card_info;
    bool foundCard = false;

    QString qs;

    for (int l1 = 0; l1 < 8; l1++) {

	qs.sprintf ("hw:%d", l1);
	if (snd_ctl_open(&ctl_handle, qs.latin1(), SND_CTL_NONBLOCK) < 0) {
	    // fprintf(stderr, "Could not open CTL %s.\n", qs.latin1());
	    continue;
	}
        foundCard = true;
	snd_ctl_card_info_alloca(&card_info);
	snd_ctl_card_info(ctl_handle, card_info);
	QString driver_name = QString(snd_ctl_card_info_get_driver(card_info));

	qs.sprintf("%s (hw:%d)", driver_name.latin1(), l1);
	card_list.append (qs);

	snd_ctl_close(ctl_handle);
    }
    if (!foundCard) {
      fprintf(stderr, "Did not find any soundcard.\n");
      exit(0);
    }
}

static struct option options[] =
        {{"help", 0, 0, 'h'},
         {"device", 1, 0, 'd'},
         {"gui", 1, 0, 'g'},
         {"channel", 1, 0, 'c'},
         {"mode", 1, 0, 'm'},
//         {"verbose", 0, 0, 'v'},
         {"printDefaultGui", 0, 0, 'p'},
         {0, 0, 0, 0}};

int main(int argc, char *argv[])
{
  read_cards ();

  int getopt_return;
  int option_index;
  int channel = -1;
  int mode = 0; // Full Size
  QString ctl_name = "hw:0";
  QString xml_name = "NO_NAME_SPECIFIED";
  bool verbose = false; (void)verbose;
  bool printDefaultGui = false;
  QString qs, qs2, driver_name;

  auto app = new QApplication(argc, argv);

    while((getopt_return = getopt_long(argc, argv, "hvpc:g:m:d:", options, &option_index)) >= 0) {
    switch(getopt_return) {
    case 'd':
        ctl_name = QString(optarg);
        break;
    case 'c':
        channel = atoi(optarg);
        break;
    case 'g':
        xml_name = QString(optarg);
        break;
    case 'm':
        mode = atoi(optarg);
        break;
    case 'h':
        printf("%s\n", aboutText);
        printf("-h, --help                      print this help message]\n");
//        printf("-v, --verbose                   list all CTL elements\n");                            // Enable this later...
        printf("-p, --printDefaultGui           print default GUI description\n");
        printf("-d <name>, --device <name>      device [hw:0]\n");
        printf("-g <name>, --gui <name>         GUI desciption file [deflt_mixer.xml]\n");
        printf("-m <num>,  --mode <num>         GUI mode (0=full, 1=master only, 2=simple\n");
        printf("-c <num>,  --channel <num>      MIDI channel [-1 (channel is set in GUI description)]\n");
        exit(EXIT_SUCCESS);
    case 'v':
        verbose = true;
        break;
    case 'p':
        printDefaultGui = true;
        break;
    }
  }

    myconfig = new MyConfig (new QSettings);

    // i18n stuff

    QTranslator translator (0);
    if (translator.load (QString ("qt_") + QTextCodec::locale (),
                         "/usr/lib/qt3" "/translations/"))
        app->installTranslator (&translator);

    const char* domain = "qamix";
    bindtextdomain (domain, "/usr/share/locale/");
    textdomain (domain);
    bind_textdomain_codeset (domain, "UTF-8");


    QMainWindow *top = new QMainWindow();
    top->setCaption(i18n("QAMix"));

//  Mixer *mixer = new Mixer(ctl_name, xml_name, top);
    MixLaunch *mixLaunch = new MixLaunch(ctl_name, xml_name, mode, top, "mixlaunch");
//  if (verbose) {
//    mixer->listControls();
//  }
  if (printDefaultGui) {
    mixLaunch->printDefaultGui();
  }

  mixLaunch->setChannel(channel);
  Q3PopupMenu *filePopup = new Q3PopupMenu(top);
  Q3PopupMenu *viewPopup = new Q3PopupMenu(top);
  viewPopup->setCheckable(true);
  Q3PopupMenu *devicePopup = new Q3PopupMenu(top);
  devicePopup->setCheckable(true);
  Q3PopupMenu *aboutMenu = new Q3PopupMenu(top);
  mixLaunch->hide_ids[0] = top->menuBar()->insertItem(i18n("&File"), filePopup);
  mixLaunch->hide_ids[3] = top->menuBar()->insertItem(i18n("&View"), viewPopup);
  mixLaunch->viewPopup = viewPopup;
  mixLaunch->hide_ids[1] = top->menuBar()->insertItem(i18n("&Soundcard"), devicePopup);
  mixLaunch->devicePopup = devicePopup;
  top->menuBar()->insertSeparator();
  mixLaunch->hide_ids[2] = top->menuBar()->insertItem(i18n("&About"), aboutMenu);
  filePopup->insertItem(i18n("&Open"), mixLaunch, SLOT(openMixer()));
  filePopup->insertItem(i18n("&Quit"), app, SLOT(quit()));
  viewPopup->insertItem(i18n("&Full"), 0, 0);
  viewPopup->insertItem(i18n("&Master only"), 1, 1);
  viewPopup->insertItem(i18n("&Simple"), 2, 2);
  aboutMenu->insertItem(i18n("About QAMix"), mixLaunch, SLOT(displayAbout()));

  int deviceCount = 0;
  for (QStringList::Iterator iter = card_list.begin (); iter != card_list.end ();
       ++iter)
  {
      devicePopup->insertItem(*iter, deviceCount, deviceCount);
      /*
      if (qs == ctl_name.stripWhiteSpace()) {
        devicePopup->setItemChecked(deviceCount, true);
      }
      */
      deviceCount++;
  }

  viewPopup->setItemChecked(mode, true);
  QObject::connect(devicePopup, SIGNAL(activated(int)), mixLaunch, SLOT(newDevice(int)));
  QObject::connect(viewPopup, SIGNAL(activated(int)), mixLaunch, SLOT(newMode(int)));
  QObject::connect(app, SIGNAL(aboutToQuit()), mixLaunch, SLOT(closeMixer()));
  top->setCentralWidget(mixLaunch);

  app->setMainWidget(top);
  top->show();
  return app->exec();
}
