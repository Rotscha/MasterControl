#ifndef __MCPluginManager_h__
#define __MCPluginManager_h__

#include <MCSingleton.h>

#define pluginMgr PluginManager::getSingletonPtr()

namespace MC
{

    class PluginManager : public Singleton <PluginManager>
    {
        private:
            map <string, Plugin *>  plugins;

        public:
                                     PluginManager();
                                   ~ PluginManager();
            bool                     load(const string & fileName);
            void                     unload(const string & fileName);
            void                     unloadAll();
            vector <string >         getLoadedPlugins();
            vector <string>          getAvailablePlugins();

    }; // class PluginManager

} // namespace MC

#endif // __MCPluginManager_h__