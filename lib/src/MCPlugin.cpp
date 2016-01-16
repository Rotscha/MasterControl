#include <MCPlugin.h>

#include <dlfcn.h>

namespace MC
{

    typedef void (* PLUGIN_START)();
    typedef void (* PLUGIN_STOP)();
    typedef void (* PLUGIN_ENABLE)(bool);

    Plugin::Plugin()
    {
        handle = nullptr;
    }


    Plugin::~Plugin()
    {
        unload();
    }


    bool Plugin::load(const string & fileName)
    {
        handle = dlopen(fileName.c_str(), RTLD_LAZY);

        if (!handle)
        {
            LOG("[Plugin::load] Failed to load '%s'! (%s)\n", fileName.c_str(), dlerror());

            return false;
        }
        
        if (!start())
        {

            dlclose(handle);

            handle = nullptr;

            return false;
        }

        return true;
    }


    void Plugin::unload()
    {
        if (!handle)
        {
            return;
        }

        stop();

        dlclose(handle);

        handle = nullptr;
    }


    bool Plugin::start()
    {
        PLUGIN_START func = (PLUGIN_START) getSymbol("startPlugin");

        if (!func)
        {
            return false;
        }

        func();

        return true;
    }


    void Plugin::stop()
    {
        PLUGIN_STOP func = (PLUGIN_STOP) getSymbol("stopPlugin");

        if (!func)
        {
            return;
        }

        func();
    }


    void * Plugin::getSymbol(const string & symbolName)
    {
        return (void *) dlsym(handle, symbolName.c_str());
    }

}
