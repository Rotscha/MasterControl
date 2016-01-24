#ifndef __MCHubUsb_h__
#define __MCHubUsb_h__

#include <MCHub.h>

// Forward declaration
struct usb_ctrlrequest;

namespace MC
{

    class HubUsb : public Hub
    {
        private:
             struct Endpoint : aiocb
             {
                 int            direction;
             };

            string              path;

            // [0] = Control, [1] = IN, [2] = OUT
            Endpoint            endpoints[3];

            void                processRequest(const usb_ctrlrequest & request);
            void                processUsbEvents();
            void                closeEndpoints();

        public:
                                HubUsb(string path);
                              ~ HubUsb();

            bool                enable();
            void                disable();
            bool                connectDevice(Device * device);
            void                disconnectDevice();
            void                processEvents();

    }; // class HubUsb

} // namespace MC

#endif // __MCHubUsb_h__
