#include <MCHubManager.h>
#include <MCHub.h>
#include <MCMain.h>
#include <MCDeviceManager.h>

namespace MC
{

    HubManager::HubManager()
    {
    }


    HubManager::~HubManager()
    {
    }


    bool HubManager::add(Hub * hub)
    {
        if (hub == nullptr)
        {
            return false;
        }

        if (hubs.find(hub->getName()) != hubs.end())
        {
            return false;
        }
        
        hubs[hub->getName()] = hub;

        envokeCallbacks(*hub, ADDED);

        return true;
    }


    bool HubManager::remove(Hub * hub)
    {
        if (hub == nullptr)
        {
            return false;
        }

        if (hubs.find(hub->getName()) == hubs.end())
        {
            return false;
        }
        
        hubs.erase(hub->getName());

        envokeCallbacks(*hub, REMOVED);

        return true;
    }


    bool HubManager::connectDevice(const string & deviceName,  const string & hubName)
    {
        if (hubs.find(hubName) == hubs.end())
        {
            return false;
        }

        const Device * device;

        if ((device = MgrDevice.getDeviceByName(deviceName)) == nullptr)
        {
            return false;
        }

        if (!hubs[hubName]->connectDevice((Device *) device))
        {
            return false;
        }

        envokeCallbacks(*hubs[hubName], CONNECTED);

        return true;
    }


    void HubManager::envokeCallbacks(const Hub & hub, const Event & event)
    {
        for (set <Callback>::const_iterator it = callbacks.begin(); it != callbacks.end(); it++)
        {
            (*it)(hub, event);
        }
    }


    void HubManager::registerCallback(Callback callback)
    {
        callbacks.insert(callback);
    }


    void HubManager::unregisterCallback(Callback callback)
    {
        callbacks.erase(callback);
    }


    void HubManager::processEvents()
    {
        map <string, Hub *>::iterator it;

        for (it = hubs.begin(); it != hubs.end(); it++)
        {
            it->second->processEvents();
        }
    }

}
