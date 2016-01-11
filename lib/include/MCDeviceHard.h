#ifndef __MCDeviceHard_h__
#define __MCDeviceHard_h__

#include <MCDevice.h>

namespace MC
{

    class DeviceHard : public Device
    {
        private:
            int      fd;

            bool     getInfo();

        public:
                     DeviceHard(string node);
                   ~ DeviceHard();
            bool     enable();
            void     disable();
            int      recvReport(void * data, int size);
            int      sendReport(void * data, int size);
            int      recvFeatureReport(void * data, int size);
            int      sendFeatureReport(void * data, int size);

    }; // class DeviceHard

} // namespace MC

#endif // __MCDeviceHard_h__
