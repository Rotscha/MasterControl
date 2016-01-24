#ifndef __MCMain_h__
#define __MCMain_h__

#include <MCSingleton.h>

#define MgrConfig Main::getSingletonPtr()->getConfigManager()
#define MgrDevice Main::getSingletonPtr()->getDeviceManager()
#define MgrPlugin Main::getSingletonPtr()->getPluginManager()
#define MgrHub    Main::getSingletonPtr()->getHubManager()

namespace MC
{

    class Main : public Singleton <Main>
    {
        private:
            vector <Hub *>   hubs;
            ConfigManager  * mgrConfig;
            DeviceManager  * mgrDevice;
            PluginManager  * mgrPlugin;
            HubManager     * mgrHub;

        public:
                             Main();
                           ~ Main();

            void             processEvents();

            ConfigManager  & getConfigManager() const;
            DeviceManager  & getDeviceManager() const;
            PluginManager  & getPluginManager() const;
            HubManager     & getHubManager() const;

    }; // class Main

} // namespace MC

#endif // __MCMain_h__ 
