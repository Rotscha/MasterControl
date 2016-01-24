#include <MCDeviceManager.h>
#include <MCDeviceHard.h>
#include <libudev.h>

namespace MC
{

    DeviceManager::DeviceManager()
    {
        fdHotplug = -255;
    }


    DeviceManager::~DeviceManager()
    {
/*        while (devices.begin() != devices.end())
        {
            removeDevice(*(devices.begin()));
        }*/

        finiHotplug();
    }


    void DeviceManager::initHotplug()
    {
        udev * uDev;
        udev_enumerate * uDevEnum;
        udev_list_entry * uDevList;
        udev_list_entry * uDevListEntry;
        udev_device * device;

        uDev = udev_new();

        if (!uDev)
        {
            LOG("[DeviceManager] Could not initialize udev! HID passthrough will be disabled.\n");

            return;
        }

        hotplugMonitor = udev_monitor_new_from_netlink(uDev, "udev");
        udev_monitor_filter_add_match_subsystem_devtype(hotplugMonitor, "hidraw", nullptr);
        udev_monitor_enable_receiving(hotplugMonitor);
        fdHotplug = udev_monitor_get_fd(hotplugMonitor);

        uDevEnum = udev_enumerate_new(uDev);
        udev_enumerate_add_match_subsystem(uDevEnum, "hidraw");
        udev_enumerate_scan_devices(uDevEnum);
        uDevList = udev_enumerate_get_list_entry(uDevEnum);


        udev_list_entry_foreach(uDevListEntry, uDevList)
        {
            const char * devicePath;

            devicePath = udev_list_entry_get_name(uDevListEntry);
            device = udev_device_new_from_syspath(uDev, devicePath);

            add(new DeviceHard(udev_device_get_devnode(device)));

            udev_device_unref(device);
        }

        udev_enumerate_unref(uDevEnum);
    }


    void DeviceManager::finiHotplug()
    {
        if (fdHotplug >= 0)
        {
            close(fdHotplug);

            udev_monitor_unref(hotplugMonitor);

            fdHotplug = -255;
        }
    }


    void DeviceManager::processHotplugEvents()
    {
        udev_device * device;

        device = udev_monitor_receive_device(hotplugMonitor);

        if (device)
        {
            if (string(udev_device_get_action(device)) == "remove")
            {
                map <string, Device *>::iterator it;

                for (it = devices.begin(); it != devices.end(); it++)
                {
                    if (it->second->getNode() == udev_device_get_devnode(device))
                    {
                        Device * dev = it->second;

                        remove(dev);

                        delete dev;

                        break;
                    }
                }
            }
            else
            {
                add(new DeviceHard(udev_device_get_devnode(device)));
            }

            udev_device_unref(device);
        }
    }


    bool DeviceManager::add(Device * device)
    {
        if (device == nullptr)
        {
            return false;
        }

        if (devices.find(device->getName()) != devices.end())
        {
            return false;
        }
        
        devices[device->getName()] = device;

        envokeCallbacks(*device, ADDED);

        return true;
    }


    bool DeviceManager::remove(Device * device)
    {
        if (device == nullptr)
        {
            return false;
        }

        if (devices.find(device->getName()) == devices.end())
        {
            return false;
        }
        
        devices.erase(device->getName());

        envokeCallbacks(*device, REMOVED);

        return true;
    }


    const Device * DeviceManager::getDeviceByName(const string & name) const
    {
        if (devices.find(name) != devices.end())
        {
            return devices.find(name)->second;
        }

        return nullptr;
    }

/*    bool DeviceManager::connectDevice(const string & deviceName,  const string & hubName)
    {
        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i]->getName() == deviceName)
            {
            }
        }
    }*/


    void DeviceManager::envokeCallbacks(const Device & device, const Event & event)
    {
        for (set <Callback>::const_iterator it = callbacks.begin(); it != callbacks.end(); it++)
        {
            (*it)(device, event);
        }
    }


    void DeviceManager::registerCallback(Callback callback)
    {
        callbacks.insert(callback);
    }


    void DeviceManager::unregisterCallback(Callback callback)
    {
        callbacks.erase(callback);
    }


    void DeviceManager::processEvents()
    {
        if (fdHotplug == -255)
        {
            initHotplug();
        }
        
        if (fdHotplug < 0)
        {
            return;
        }

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        fd_set fdSet;
        FD_ZERO(&fdSet);
        FD_SET(fdHotplug, &fdSet);

        int s = select(fdHotplug + 1, &fdSet, NULL, NULL, &timeout);

        if (s > 0)
        {
            processHotplugEvents();
        }
    }

}
