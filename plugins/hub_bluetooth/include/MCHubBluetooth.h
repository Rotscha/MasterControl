#ifndef __MCHubBluetooth_h__
#define __MCHubBluetooth_h__

#include <MCHub.h>

namespace MC
{

    class HubBluetooth : public Hub
    {
        friend class ServerBluetooth;

        private:
            int                 socketIntr;
            int                 socketCtrl;
            bool                bootMode;

        public:
                                ClientBluetooth();
                                ~ClientBluetooth();

            bool                reconnect();
            bool                connect(const String & serverAddress);
            void                disconnect();
            bool                sendReport(const Hid::Report * report);

            bool                enable();
            void                disable();
            bool                connectDevice(Device * device);
            void                disconnectDevice();
            void                processEvents();

    }; // class HubBluetooth

} // namespace MC

#endif // __MCHubBluetooth_h__
