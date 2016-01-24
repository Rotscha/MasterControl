#include <MCMain.h>
#include <MCConfigManager.h>
#include <MCDeviceManager.h>
#include <MCPluginManager.h>
#include <MCHubManager.h>

namespace MC
{

    Main::Main()
    {
        mgrConfig = new ConfigManager();
        mgrHub    = new HubManager();
        mgrDevice = new DeviceManager();
        mgrPlugin = new PluginManager();

        mgrConfig->loadAll();
//mgrConfig->getValues("general", "xxx");
mgrConfig->getKeys("plugin");
    }


    Main::~Main()
    {
        delete mgrPlugin;
        delete mgrDevice;
        delete mgrHub;
        delete mgrConfig;
    }


    void Main::processEvents()
    {
        mgrDevice->processEvents();
        mgrHub->processEvents();
    }


    ConfigManager & Main::getConfigManager() const
    {
        return * mgrConfig;
    }


    DeviceManager & Main::getDeviceManager() const
    {
        return * mgrDevice;
    }


    PluginManager & Main::getPluginManager() const
    {
        return * mgrPlugin;
    }

    
    HubManager & Main::getHubManager() const
    {
        return * mgrHub;
    }

}
 
