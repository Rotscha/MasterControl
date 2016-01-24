#ifndef __MCDeviceManager_h__
#define __MCDeviceManager_h__

#include <MCPrerequisites.h>

// Forward declaration
struct udev_monitor;

namespace MC
{

    class DeviceManager
    {
        private:
            typedef void            (*Callback)(const Device & device, const Event & event);

            map <string, Device *>    devices;
            udev_monitor            * hotplugMonitor;
            int                       fdHotplug;
            set <Callback>            callbacks;

            void                      envokeCallbacks(const Device & device, const Event & event);
            void                      initHotplug();
            void                      finiHotplug();
            void                      processHotplugEvents();

        public:
                                      DeviceManager();
                                    ~ DeviceManager();

            bool                      add(Device * device);
            bool                      remove(Device * device);
            void                      registerCallback(Callback callback);
            void                      unregisterCallback(Callback callback);
//            bool                      connectDevice(const string & deviceName,  const string & hubName);
            void                      processEvents();

            const Device            * getDeviceByName(const string & name) const;

    }; // class DeviceManager

} // namespace MC

#endif // __MCDeviceManager_h__ 
