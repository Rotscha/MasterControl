#include <MCDeviceManager.h>
#include <MCDeviceHard.h>
#include <libudev.h>

namespace MC
{

    DeviceManager::DeviceManager()
    {
        initHotplug();
    }


    DeviceManager::~DeviceManager()
    {
        while (devices.begin() != devices.end())
        {
            removeDevice(*(devices.begin()));
        }

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
//            DBG("[DeviceManager] Could not initialize udev! Bluetooth support will be disabled.\n");

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

            addDevice(new DeviceHard(udev_device_get_devnode(device)));

            udev_device_unref(device);
        }

        udev_enumerate_unref(uDevEnum);
    }


    void DeviceManager::finiHotplug()
    {
        close(fdHotplug);

        udev_monitor_unref(hotplugMonitor);
    }


    void DeviceManager::processHotplugEvents()
    {
        udev_device * device;

        device = udev_monitor_receive_device(hotplugMonitor);

        if (device)
        {
            if (string(udev_device_get_action(device)) == "remove")
            {
                for (int i = 0; i < devices.size(); i++)
                {
                    if (devices[i]->getNode() == udev_device_get_devnode(device))
                    {
                          removeDevice(devices[i]);

                          break;
                    }
                }

            }
            else
            {
                addDevice(new DeviceHard(udev_device_get_devnode(device)));
            }

            udev_device_unref(device);
        }
    }


    void DeviceManager::processReportEvents()
    {
/*        if (devices.size() == 0)
        {
            return;
        }

        char buf[255];
        int  size = sizeof(buf);

        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i]->recvReport(buf, &size))
            {
                for (set <ReportCallback>::const_iterator it = reportCallbacks.begin(); it != reportCallbacks.end(); it++)
                {
                    (*it)(buf, size);
                }
            }
        }*/
    }


    void DeviceManager::addDevice(Device * device)
    {
        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i] == device)
            {
                return;
            }
        }

        devices.push_back(device);

        for (set <HotplugCallback>::const_iterator it = hotplugCallbacks.begin(); it != hotplugCallbacks.end(); it++)
        {
            (*it)(*device, true);
        }
    }


    void DeviceManager::removeDevice(Device * device)
    {
        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i] == device)
            {
                for (set <HotplugCallback>::const_iterator it = hotplugCallbacks.begin(); it != hotplugCallbacks.end(); it++)
                {
                    (*it)(*device, false);
                }

                delete device;

                devices.erase(devices.begin() + i);

                return;
            }
        }
    }


    void DeviceManager::registerHotplugCallback(HotplugCallback callback)
    {
        hotplugCallbacks.insert(callback);
    }


    void DeviceManager::registerReportCallback(ReportCallback callback)
    {
        reportCallbacks.insert(callback);
    }


    void DeviceManager::unregisterHotplugCallback(HotplugCallback callback)
    {
        hotplugCallbacks.erase(callback);
    }


    void DeviceManager::unregisterReportCallback(ReportCallback callback)
    {
        reportCallbacks.erase(callback);
    }


    void DeviceManager::processEvents()
    {
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

        //processReportEvents();
    }

Device * DeviceManager::getDevice()
{
  return devices[0];
}

}
