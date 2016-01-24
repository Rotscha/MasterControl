#include <MCApplication.h>
#include <MCWindowMain.h>
#include <MCPluginManager.h>
/*#include <MCInputDeviceManager.h>
#include <MCInputDevice.h>
#include <MCConfig.h>
#include <MCHidManager.h>
#include <MCPluginManager.h>
#include <MCWindowManager.h>

#include <MCServerBluetooth.h>
#include <MCClientBluetooth.h>
#include <MCClientUSB.h>
#include <X11/Xlib.h>

#define FILENAME_LOG          "mastercontrol.log"
#define FILENAME_MAIN_CONF    "main.conf"
#define FILENAME_KEYDEFS_CONF "events.conf"*/

namespace MC
{
    //FILE * Application::logFile = stdout;

    Application::Application(int & argc, char** argv) : QApplication(argc, argv, true)
    {
/*        display = nullptr; //XOpenDisplay(NULL);
        inputGrabbed = false;*/

        setApplicationName("Master Control");

        main = new Main();

        windowMain = new WindowMain();
        setActiveWindow(windowMain);

MgrPlugin.load("hub_usb.so");


/*        logFile = fopen(FILENAME_LOG, "w");

        if (!logFile)
        {
            printf("Cannot create logfile '%s'! (%s) Logging to 'stdout'...\n", FILENAME_LOG, strerror(errno));

            LOGFILE = stdout;
        }
        else
        {
//            LOGFILE = stdout;
            LOGFILE = logFile;
        }

        new ConfigManager();
        ConfigMgr->load(FILENAME_MAIN_CONF);
        ConfigMgr->load(FILENAME_KEYDEFS_CONF);

        new WindowManager();
        new HidManager();
HidMgr->openRaw("/dev/hidraw0");
        new PluginManager();*/

/*        inputDeviceManager = new InputDeviceManager();
        inputDeviceManager->openDevices();

        loadPlugins();

        serverBluetooth = new ServerBluetooth();
        serverBluetooth->start();
        serverBluetooth->startListen();

        clientBluetooth = new ClientBluetooth();

        clientUSB = new ClientUSB();
        clientUSB->connect(String(CFG_USB_GADGETFS + "musb-hdrc"));*/

        startTimer(50);
    }


    Application::~Application()
    {
/*        grabInput(false);

        delete clientUSB;
        delete serverBluetooth;
        delete clientBluetooth;
        delete inputDeviceManager;*/
        delete windowMain;
        delete main;
        /*delete PluginMgr;
        delete HidMgr;
        delete WindowMgr;
        //ConfigMgr->save(FILENAME_MAIN_CONF);
        delete ConfigMgr;

        if (logFile)
        {
            fclose(logFile);
        }*/
    }


    void Application::timerEvent(QTimerEvent * timerEvent)
    {
        main->processEvents();

        /*if (inputGrabbed)
        {
            WindowMgr->updateTouchscreen();
        }

        serverBluetooth->processEvents();
        clientBluetooth->processEvents();
        clientUSB->processEvents();

        if (!clientBluetooth->connected())
        {
            if (serverBluetooth->connectionAvailable())
            {
                delete clientBluetooth;

                clientBluetooth = serverBluetooth->accept();
            }
        }

        InputEventList events = inputDeviceManager->getEvents();

        InputEventList::const_iterator it;

        for (it = events.begin(); it != events.end(); it++)
        {
            // Skip unknown/unused event types
            if ((it->type == 0) || (it->type == 4))
            {
                continue;
            }

printf("%02i  %02i  %04i %i\n", it->type, it->code, it->value, it->source);


            // Touchscreen currently not used
            if (it->source == InputDevice::Touchscreen)
            {
                int x = it->value;
                it++;
                int y = it->value;
                it++;
                bool b = it->value > 0;

                continue;
            }

            Vector <const Hid::Report *> reports = HidMgr->getReports((const InputEvent *) &(*it));

            Vector <const Hid::Report *>::const_iterator report;

            for (report = reports.begin(); report != reports.end(); report++)
            {
                clientUSB->sendReport(*report);
                clientBluetooth->sendReport(*report);
            }

        }*/
    }


    void Application::loadPlugins()
    {
/*        StringList plugins = CONFIG_GEN_PLUGINS;

        for (int i = 0; i < plugins.size(); i++)
        {
            PluginMgr->load(plugins[i]);
        }*/
    }


    void Application::grabInput(bool grab)
    {
/*        inputGrabbed = grab;

        if (grab)
        {
            if (display != nullptr)
            {
                return;
            }

            display = XOpenDisplay(NULL);

            ::Window window = DefaultRootWindow(display);

            fprintf(LOGFILE, "Grabbing Mouse: %s!\n", XGrabPointer (display, window, false, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime) == GrabSuccess ? "Ok" : "Failed");
            fprintf(LOGFILE, "Grabbing keyboard: %s!\n", XGrabKeyboard(display, window, false, GrabModeAsync, GrabModeAsync, CurrentTime) == GrabSuccess ? "Ok" : "Failed");
        }
        else
        {
            if (display == nullptr)
            {
                return;
            }

            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);

            XCloseDisplay(display);

            display = nullptr;
        }*/
    }


    void Application::showMessage(const string & source, const string & message, const bool & log)
    {
/*        windowMain->showMessage(source, message);

        if (log)
        {
            fprintf(LOGFILE, "%s: %s\n", source.c_str(), message.c_str());
        }*/
    }

} // namespace MC