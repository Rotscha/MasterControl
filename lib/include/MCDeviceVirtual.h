#ifndef __MCDeviceVirtual_h__
#define __MCDeviceVirtual_h__

#include <MCDevice.h>

namespace MC
{

    class DeviceVirtual : public Device
    {
        private:

            bool     enable();
            void     disable();

        public:
                     DeviceVirtual();
                   ~ DeviceVirtual();

            int      recvReport(void * data, int size);
            int      sendReport(void * data, int size);

    }; // class DeviceVirtual

} // namespace MC

#endif // __MCDeviceVirtual_h__
