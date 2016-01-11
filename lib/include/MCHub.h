#ifndef __MCHub_h__
#define __MCHub_h__

#include <MCPrerequisites.h>

namespace MC
{

    class Hub
    {
        protected:
            bool           mIsEnabled;
            bool           mIsDeviceConnected;
            Device       * device;

        public:
                           Hub();
            virtual      ~ Hub();
            virtual bool   enable()                       = 0;
            virtual void   disable()                      = 0;
            virtual bool   isEnabled();
            virtual bool   connectDevice(Device * device) = 0;
            virtual void   processEvents()                = 0;


    }; // class Hub

} // namespace MC

#endif // __MCHub_h__ 
