#ifndef __MCDeviceManager_h__
#define __MCDeviceManager_h__

#include <MCSingleton.h>

#define deviceMgr DeviceManager::getSingletonPtr()

// Forward declaration
struct udev_monitor;

namespace MC
{

    class DeviceManager : public Singleton < DeviceManager >
    {
        private:
            typedef void            (*HotplugCallback)(Device & device, bool action);
            typedef void            (*ReportCallback)(const char * report, int size);

            vector <Device *>         devices;
            udev_monitor            * hotplugMonitor;
            int                       fdHotplug;

            set <HotplugCallback>     hotplugCallbacks;
            set <ReportCallback>      reportCallbacks;

            void                      initHotplug();
            void                      finiHotplug();
            void                      processHotplugEvents();
            void                      processReportEvents();

        public:
                                      DeviceManager();
                                    ~ DeviceManager();
            void                      addDevice(Device * device);
            void                      removeDevice(Device * device);
            void                      registerHotplugCallback(HotplugCallback callback);
            void                      registerReportCallback(ReportCallback callback);
            void                      unregisterHotplugCallback(HotplugCallback callback);
            void                      unregisterReportCallback(ReportCallback callback);
            void                      processEvents();
Device * getDevice();
//            Device               * getDeviceByID(int deviceID);
//            vector <Device *>      getDevices();

    }; // class DeviceManager

} // namespace MC

#endif // __MCDeviceManager_h__ 
