#ifndef __MCHubUsb_h__
#define __MCHubUsb_h__

#include <MCHub.h>
#include <MCSingleton.h>

// Forward declaration
struct usb_ctrlrequest;

namespace MC
{

    class HubUsb : public Hub, public Singleton <HubUsb>
    {
        private:
             struct Endpoint : aiocb
             {
                 int            direction;
             };

            string              path;
            vector <Endpoint>   endpoints;
            Device            * device;

            void                processRequest(const usb_ctrlrequest & request);
            void                processUsbEvents();

        public:
                                HubUsb(string path);
                              ~ HubUsb();
            bool                enable();
            void                disable();
            bool                connectDevice(Device * device);
            void                processEvents();

    }; // class HubUsb

} // namespace MC

#endif // __MCHubUsb_h__
