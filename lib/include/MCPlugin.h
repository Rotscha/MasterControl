#ifndef __MCPlugin_h__
#define __MCPlugin_h__

#include <MCPrerequisites.h>

namespace MC
{

    class Plugin
    {
        private:
            void * handle;

            bool   start();
            void   stop();
            void * getSymbol(const string & symbolName);

        public:
                   Plugin();
                 ~ Plugin();

            bool   load(const string & fileName);
            void   unload();

    }; // class Plugin

} // namespace MC

#endif // __MCPlugin_h__