#include <MCHubBluetooth.h>
#include <MCMain.h>
#include <MCHubManager.h>

namespace MC
{

    HubBluetooth * plugin = nullptr;
    
    extern "C" void startPlugin()
    {
        if (plugin)
        {
            return;
        }

        plugin = new HubBluetooth();

        MgrHub.add(plugin);
    }


    extern "C" void stopPlugin()
    {
        if (!plugin)
        {
            return;
        }

        MgrHub.remove(plugin);

        delete plugin;

        plugin = nullptr;
    }


    extern "C" void enablePlugin(bool enable)
    {
        if (!plugin)
        {
            return;
        }

        //plugin->enable(enable);
    }

} // namespace MC
 
