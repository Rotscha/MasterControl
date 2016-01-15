#include <MCDeviceManager.h>
#include <MCDevice.h>
#include <MCHubUsb.h>

static bool running = true;

void onExit(int s)
{
    running = false;
}


// Experimental program
int main(int argc, char * argv[])
{
    signal(SIGINT, onExit);    

    using namespace MC;

    DeviceManager * dev = new DeviceManager();
    HubUsb * usb = new HubUsb("/dev/gadgetfs");

    usb->connectDevice(dev->getDevice());
    
    while(running)
    {
      dev->processEvents();
      usb->processEvents();
    }
    
    printf("Terminating!\n");

    delete usb;
    delete dev;

//    Application app(argc, argv);
//    app.setApplicationName("Master Control");

/*    while (true)
    {
      app.processEvents(QEventLoop::AllEvents | QEventLoop::WaitForMoreEvents);
    }*/
//    return app.exec();
}

// export PATH=$PATH:~/pandora-dev/arm-2014.05/bin/

