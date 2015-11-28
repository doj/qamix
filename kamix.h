#include <qmainwindow.h>
#include <kpopupmenu.h>
#include <kuniqueapplication.h>
#include "mixlaunch.h"
#include "mixer.h"
#include "masterbox.h"

class KAMix : public QWidget
{
    Q_OBJECT

public:

    KAMix (QWidget* parent, const char* name, QMainWindow*, MixLaunch *, QString p_ctl_name, QString p_xml_name, int p_mode);
    ~KAMix ();
    int device_ids[8], mode_ids[3], deviceCount;

private slots:

    void slot_quit ();

private:

    QMainWindow* top;
    MixLaunch *mixLaunch;
    QString ctl_name, xml_name;
    int currentDeviceID, currentModeID, mode;
    MasterBox *masterBox;
    Mixer *masterMixer;
    QTimer *timer;
    
    KPopupMenu* menu;

    QPixmap* mini_icon;

    void mousePressEvent (QMouseEvent*);        // overridden
    void paintEvent (QPaintEvent*);             // overridden

public slots:
    void setNewDevice(int);
    void setNewMode(int);
    void showWindow(int);
    void timerProc();
};

