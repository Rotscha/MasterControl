#include <MCWindowMain.h>
#include <MCMain.h>
#include <MCHubManager.h>
#include <MCDeviceManager.h>
//#include <MCApplication.h>
//#include <MCPluginManager.h>
//#include <MCConfig.h>

namespace MC
{
    WindowMain * WindowMain::ptr = nullptr;

    WindowMain::WindowMain()
    {
        ptr = this;

        hide();
        setWindowTitle("Master Control");
        setWindowFlags(Qt::Dialog);

        dsplInputGrabber = nullptr;

        // Setup GUI stuff
        menuTrayIcon = new QMenu(this);
        menuTrayIcon->setTitle("Master Control");
//        menuTrayIcon->setStyleSheet("QMenu { font-size:14px; color:white; left: 20px; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #333333);}");
        menuTrayIcon->setStyleSheet("QMenu { font-size:14px; left: 20px;}");

        QAction * actionCaption = new QAction(menuTrayIcon);
        actionCaption->setText("Master Control");
        actionCaption->setEnabled(false);

        menuStatus = new QMenu(menuTrayIcon);
        menuStatus->setTitle("&Status");
        //menuStatus->setStyleSheet("QMenu { font-size:14px; color:white; left: 20px; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #333333);}");

        actionStatusBluetooth = new QAction(menuStatus);
        actionStatusBluetooth->setEnabled(false);
        actionStatusBluetooth->setProperty("name", "Bluetooth");
        menuStatus->addAction(actionStatusBluetooth);
        setConnectionStatus("Bluetooth", 0);

        actionStatusUSB = new QAction(menuStatus);
        actionStatusUSB->setEnabled(false);
        actionStatusUSB->setProperty("name", "USB");
        menuStatus->addAction(actionStatusUSB);
        setConnectionStatus("USB", 0);

        menuBluetooth = new QMenu(menuTrayIcon);
        menuBluetooth->setTitle("&Bluetooth");
        //menuBluetooth->setStyleSheet("QMenu { font-size:14px; color:white; left: 20px; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #333333);}");
        menuBluetooth->setProperty("connected", false);

        menuPlugins = new QMenu(menuTrayIcon);
        menuPlugins->setTitle("&Plugins");
        //menuPlugins->setStyleSheet("QMenu { font-size:14px; color:white; left: 20px; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #333333);}");

        QAction * actionGrabInput = new QAction(tr("&Grab input"), menuTrayIcon);
        actionGrabInput->setCheckable(true);
        connect(actionGrabInput, SIGNAL(toggled(bool)), this, SLOT(grabInput(bool)));

        QAction * actionQuit = new QAction(tr("&Quit"), menuTrayIcon);
        connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

        menuTrayIcon->addAction(actionCaption);
        menuTrayIcon->addSeparator();
        menuTrayIcon->addMenu(menuStatus);
        menuTrayIcon->addMenu(menuBluetooth);
        menuTrayIcon->addMenu(menuPlugins);
        menuTrayIcon->addAction(actionGrabInput);
        menuTrayIcon->addSeparator();
        menuTrayIcon->addAction(actionQuit);

        trayIcon = new QSystemTrayIcon(this);
//        trayIcon->setIcon(QIcon("pandora/mastercontrol.png"));
        trayIcon->setIcon(QIcon("mastercontrol.png"));
        trayIcon->setToolTip("Master Control");
        trayIcon->setContextMenu(menuTrayIcon);
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
        trayIcon->show();

        MgrHub.registerCallback(onHubEvent);
        MgrDevice.registerCallback(onDeviceEvent);
    }


    WindowMain::~WindowMain()
    {
        MgrDevice.unregisterCallback(onDeviceEvent);
        MgrHub.unregisterCallback(onHubEvent);
    }
    

    void WindowMain::grabInput(bool grab)
    {
//        App->grabInput(grab);
    }


    void WindowMain::showMessage(const string & source, const string & message)
    {
//        trayIcon->showMessage(source.c_str(), message.c_str());
    }


    void WindowMain::setBluetoothStatus(const string & addr, bool connected)
    {
/*        if (connected)
        {
            menuBluetooth->clear();

            String text = "Connected to " + addr;

            QAction * action = new QAction(menuBluetooth);
            action->setText(text.c_str());
            action->setEnabled(false);

            menuBluetooth->addAction(action);
            menuBluetooth->addSeparator();

            action = new QAction(menuBluetooth);
            action->setText("Disconnect");
            action->setProperty("name", "btDisconnect");
            connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));

            menuBluetooth->addAction(action);
            menuBluetooth->setProperty("connected", true);
        }
        else
        {
            menuBluetooth->clear();

            QAction * action = new QAction(menuBluetooth);
            action->setText("Connect to");
            action->setEnabled(false);

            menuBluetooth->addAction(action);
            menuBluetooth->addSeparator();

            StringMap btNames = ConfigManager::getSingletonPtr()->getBluetoothNames();
            StringMap::const_iterator it;

            for (it = btNames.begin(); it != btNames.end(); it++)
            {
                QAction * action = new QAction(menuBluetooth);
                String name = it->first + " (" + it->second + ')';
                action->setText(name.c_str());
                action->setProperty("name", "btConnect");
                action->setProperty("address", it->first.c_str());
                connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));

                menuBluetooth->addAction(action);
            }

            menuBluetooth->setProperty("connected", false);
        }*/
    }


    void WindowMain::setConnectionStatus(const string & name, int status)
    {
/*        QAction * action;

        if (name == "Bluetooth")
        {
            action = actionStatusBluetooth;
        }
        else
        if (name == "USB")
        {
            action = actionStatusUSB;
        }
        else
        {
            return;
        }

        switch(status)
        {
            case 0:
            {
                action->setText(action->property("name").toString() + ": Not available");

                break;
            }

            case 1:
            {
                action->setText(action->property("name").toString() + ": Not connected");

                break;
            }

            case 2:
            {
                action->setText(action->property("name").toString() + ": Connected");

                break;
            }
        }*/
    }


    void WindowMain::iconActivated(QSystemTrayIcon::ActivationReason reason)
    {
        /*if (menuBluetooth->property("connected") == false)
        {
            setBluetoothStatus("", false);
        }*/

        if (menuPlugins->isEmpty())
        {
  QAction * action = new QAction(menuPlugins);
  action->setText("Available plugins");
  action->setEnabled(true);
  action->setCheckable(true);
  menuPlugins->addAction(action);
  menuPlugins->addSeparator();

QPixmap pixmap(20,20);
pixmap.fill(QColor("red"));
//QIcon redIcon(pixmap);
action->setIcon(QIcon(pixmap));

/*            StringList plugins = PluginManager::getSingletonPtr()->getLoadedPlugins();

            QAction * action = new QAction(menuPlugins);
            action->setText("Available plugins");
            action->setEnabled(false);

            menuPlugins->addAction(action);
            menuPlugins->addSeparator();

            for (int i = 0; i < plugins.size(); i++)
            {
                action = new QAction(menuPlugins);
                action->setText(plugins[i].c_str());
                action->setCheckable(true);
                action->setChecked(true);
                action->setProperty("name", "plugin");
                connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));

                menuPlugins->addAction(action);
            }

            menuPlugins->addSeparator();

            action = new QAction(menuPlugins);
            action->setText("&Enable all");
            action->setCheckable(false);
            action->setProperty("name", "enablePlugins");
            menuPlugins->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));

            action = new QAction(menuPlugins);
            action->setText("&Disable all");
            action->setCheckable(false);
            action->setProperty("name", "disablePlugins");
            menuPlugins->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));*/
        }

        switch(reason)
        {
            case QSystemTrayIcon::Trigger:
            {
                menuTrayIcon->popup(QCursor::pos());

                break;
            }
        }
    }


    void WindowMain::actionTriggered()
    {
/*        QAction * action = (QAction *) sender();

        if (action->property("name") == "btConnect")
        {
            String addr = action->property("address").toString().toStdString();

            Application::getSingletonPtr()->bluetoothConnect(addr);

            return;
        }

        if (action->property("name") == "btDisconnect")
        {
            Application::getSingletonPtr()->bluetoothDisconnect();

            return;
        }

        if (action->property("name") == "plugin")
        {
            if (action->isChecked())
            {
                PluginManager::getSingletonPtr()->enable(action->text().toStdString(), true);
            }
            else
            {
                PluginManager::getSingletonPtr()->enable(action->text().toStdString(), false);
            }

            return;
        }

        if ((action->property("name") == "enablePlugins") || (action->property("name") == "disablePlugins"))
        {
            bool enable = action->property("name") == "enablePlugins" ? true : false;

            QList <QAction*> actions = menuPlugins->actions();

            QList <QAction*>::iterator it;

            for (it = actions.begin(); it != actions.end(); it++)
            {
                if ((*it)->property("name") == "plugin")
                {
                    PluginManager::getSingletonPtr()->enable((*it)->text().toStdString(), enable);

                    (*it)->setChecked(enable);
                }
            }

            return;
        }*/
    }


    void WindowMain::onHubEvent(const Hub & hub, const Event & event)
    {
        printf("%i\n", event);
    }


    void WindowMain::onDeviceEvent(const Device & device, const Event & event)
    {
        printf("%i\n", event);
    }


} // namespace MC