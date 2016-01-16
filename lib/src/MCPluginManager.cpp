#include <MCPluginManager.h>
#include <MCPlugin.h>

#include <dirent.h>

//#define PLUGIN_DIR "pandora/plugins/"
#define PLUGIN_DIR "plugins/"

namespace MC
{

    PluginManager::PluginManager()
    {
    }


    PluginManager::~PluginManager()
    {
        unloadAll();
    }


    bool PluginManager::load(const string & fileName)
    {
        map <string, Plugin *>::iterator it = plugins.find(fileName);

        if (it != plugins.end())
        {
            return false;
        }

        Plugin * plugin = new Plugin();

        bool r = plugin->load(PLUGIN_DIR + fileName);

        if (r == false)
        {
            delete plugin;

            LOG("[PluginManager::load] Failed to load %s!\n", fileName.c_str());

            return false;
        }

        plugins[fileName] = plugin;

        LOG("[PluginManager::load] Loaded %s\n", fileName.c_str());

        return true;
    }


    void PluginManager::unload(const string & fileName)
    {
        map <string, Plugin *>::iterator it = plugins.find(fileName);

        if (it == plugins.end())
        {
            return;
        }

        it->second->unload();

        delete it->second;

        LOG("[PluginManager::unload] Unloaded %s\n", fileName.c_str());

        plugins.erase(it);
    }


    void PluginManager::unloadAll()
    {
        while (plugins.begin() != plugins.end())
        {
            unload(plugins.begin()->first);
        }
    }


    vector < string >  PluginManager::getLoadedPlugins()
    {
        vector < string > result;

        map < string, Plugin * >::const_iterator it;

        for (it = plugins.begin(); it != plugins.end(); it++)
        {
            result.push_back(it->first);
        }

        return result;
    }


    vector < string > PluginManager::getAvailablePlugins()
    {
        vector < string > plugins;

        DIR * dir;
        dirent * entry;

        if ((dir = opendir(PLUGIN_DIR)) == nullptr)
        {
            return plugins;
        }

        while ((entry = readdir(dir)) != nullptr)
        {
            string fileName = entry->d_name;

            if ((fileName == ".") || (fileName == ".."))
            {
                continue;
            }

            plugins.push_back(entry->d_name);
        }

        closedir (dir);

        return plugins;
    }

}
