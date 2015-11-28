

#include <qpainter.h>
#include <qtooltip.h>
#include <qvbox.h>

#include <kwin.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <qstring.h>
#include <qtimer.h>

#include "kamix.h"
#include "mixlaunch.h"
#include "mixer.h"
#include "masterbox.h"
#include "main.h"
#include "utils.h"
#include "myconfig.h"


KAMix::KAMix (QWidget* parent, const char* name, QMainWindow* p_top,
	      MixLaunch *p_mixLaunch, QString p_ctl_name, QString p_xml_name, int p_mode)
    : QWidget (parent, name),
      top (p_top)
{
    QString qs;    

    mixLaunch = p_mixLaunch;
    ctl_name = p_ctl_name;
    xml_name = p_xml_name;
    mode = p_mode;
    currentDeviceID = 0;
    currentModeID = 0;

    KWin::setSystemTrayWindowFor (winId (), qt_xrootwin ());
    setBackgroundMode (X11ParentRelative);

    QToolTip::add (this, i18n ("KAMix - Audio Mixer"));

    mini_icon = load_qpixmap ("kamix.png");

    menu = new KPopupMenu (0, "main_menu");
    menu->setCheckable(true);

    menu->insertTitle (*mini_icon, "KAMix - Audio Mixer");

    menu->insertSeparator ();

    deviceCount = 0;
    for (QStringList::Iterator iter = card_list.begin (); iter != card_list.end ();
	 ++iter)
    {
//        mixLaunch->devicePopup->insertItem(*iter, mixLaunch, SLOT(newDevice(int)));
        device_ids[deviceCount] = menu->insertItem(*iter, this, SLOT(setNewDevice(int)));
        qs = (*iter).mid((*iter).findRev("(hw:") + 1, 4);
//        fprintf(stderr, "qs = %s\n", qs.latin1());
	if (qs == ctl_name.stripWhiteSpace()) {
	    menu->setItemChecked(device_ids[deviceCount], true);
//            setNewDevice(device_ids[deviceCount]);
	}
	deviceCount++;
    }

    menu->insertSeparator ();

    mode_ids[0] = menu->insertItem (i18n ("&Full"), this, SLOT(setNewMode(int)));
    mode_ids[1] = menu->insertItem (i18n ("&Master only"), this, SLOT(setNewMode(int)));
    mode_ids[2] = menu->insertItem (i18n ("&Simple"), this, SLOT(setNewMode(int)));

    menu->setItemChecked(mode_ids[mode], true);
    
    menu->insertSeparator ();

    menu->insertItem (i18n ("Show Mixer &Window"), this, SLOT(showWindow(int)));

    menu->insertSeparator ();

    menu->insertItem (i18n("&About KAmix"), mixLaunch, SLOT(displayAbout()));

    menu->insertSeparator ();

    menu->insertItem (SmallIcon ("exit"), i18n("&Quit"), this,
                      SLOT (slot_quit ()));
    masterBox = new MasterBox(0, "masterBox", Qt::WStyle_Customize | Qt::WStyle_Splash);
    masterBox->hide();
    masterMixer = new Mixer(ctl_name, xml_name, 1, masterBox);
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(timerProc()));
}


KAMix::~KAMix ()
{
}


void
KAMix::slot_quit ()
{
#ifndef NDEBUG
    fprintf (stderr, "%s\n", __PRETTY_FUNCTION__);
#endif

    QString tmp1 = i18n ("Should KAMix start automatically\n"
	"when you log in?");
    int tmp2 = KMessageBox::questionYesNoCancel (0, tmp1);

    if (tmp2 == KMessageBox::Cancel)
	return;

    myconfig->writeBool ("General", "Autostart", tmp2 == KMessageBox::Yes);
    myconfig->sync ();

    app->quit ();
}

void
KAMix::mousePressEvent (QMouseEvent* event)
{
#ifndef NDEBUG
    fprintf (stderr, "%s\n", __PRETTY_FUNCTION__);
#endif

    int wx, wy, mx, my, h, w, dx, dy;

    switch (event->button ())
    {
        case RightButton:
	    menu->popup (event->globalPos ());
	    break;

        case LeftButton:
            if (!masterBox->isVisible()) {
              masterBox->show();
              masterBox->setHideOnLeave(false);
              timer->start(2000, true);
              wx = 0; wy = 0;
              h = masterBox->height();
              w = masterBox->width();
              dx = 4;
              dy = 10;
              mx = event->globalX();
              my = event->globalY();
              if (my > 300) wy = my - h - dy; 
              else wy = my + dy;
              if (mx > 400) wx = mx - w - dx; 
              else wx = mx + dx;
              masterBox->move(wx, wy);
              masterBox->raise();
            } else {
              masterBox->hide();
            }
	    break;

	default:
	    break;
    }
}


void
KAMix::paintEvent (QPaintEvent* pevent)
{
#ifndef NDEBUG
    fprintf (stderr, "%s\n", __PRETTY_FUNCTION__);
#endif

    QPainter p (this);
    p.setClipRect (pevent->rect ());
    p.setBackgroundMode (TransparentMode);
    p.drawPixmap (0, 0, *mini_icon);
    p.end ();
}

void KAMix::setNewDevice(int id) {

  int l1, index;
  QString qs;

  currentDeviceID = id;
//  fprintf(stderr, "KAMix::setNewDevice id = %d   menu->count() = %d\n", id, menu->count());
  for (l1 = 0; l1 < deviceCount; l1++) {
    menu->setItemChecked(device_ids[l1], false);
  }
  menu->setItemChecked(currentDeviceID, true);
  if (masterMixer) {
    delete masterMixer;
  }
  if (masterBox) {
    delete masterBox;
  }

  masterBox = new MasterBox(0, "masterBox", Qt::WStyle_Customize | Qt::WStyle_Splash);
  qs = menu->text(id);
//  fprintf(stderr, "id = %d, qs = %s\n", id, qs.latin1());
//  printf("menu->text(%d): %s\n", id, qs.latin1()); 
  index = qs.findRev("(hw:");
  ctl_name = qs.mid(index + 1, 4);
  xml_name = "NO_NAME_SPECIFIED"; // Automatic search for GUI description
  masterMixer = new Mixer(ctl_name, xml_name, 1, masterBox);

  for (l1 = 0; l1 < deviceCount; l1++) {
    if (device_ids[l1] == id) {
      mixLaunch->newDeviceParam(l1);
      break;
    }
  }
}

void KAMix::setNewMode(int id) {

  int l1;

  currentModeID = id;
//  fprintf(stderr, "KAMix::setNewMode id = %d   menu->count() = %d\n", id, menu->count());
  for (l1 = 0; l1 < 3; l1++) {
    menu->setItemChecked(mode_ids[l1], false);
  }
  menu->setItemChecked(currentModeID, true);
  for (l1 = 0; l1 < 3; l1++) {
    if (mode_ids[l1] == id) {
      mixLaunch->newModeParam(l1);
      break;
    }
  }
}

void KAMix::showWindow(int id) {

  top->show();
}

void KAMix::timerProc() {

  masterBox->setHideOnLeave(true);
}
