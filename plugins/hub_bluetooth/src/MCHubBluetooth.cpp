#include <MCHubBluetooth.h>
#include <MCMain.h>

#include <poll.h>

namespace MC
{

    ClientBluetooth::ClientBluetooth()
    {
        socketCtrl  = -1;
        socketIntr  = -1;
        isConnected = false;
        bootMode = false;
    }
    
    
    ClientBluetooth::ClientBluetooth(const int & serverSocketCtrl, const int & serverSocketIntr)
    {
        SockAddrL2 addr;
        socklen_t len = sizeof(addr);

        socketCtrl = accept(serverSocketCtrl, (SockAddr* ) &addr, &len);
        socketIntr = accept(serverSocketIntr, (SockAddr* ) &addr, &len);

        char addrStr[18];
        memset(addrStr, 0, 18);
        ba2str(&addr.l2_bdaddr, addrStr);

        serverAddress = addrStr;

        isConnected = true;

        //MESSAGE("Bluetooth", "Connected with " + serverAddress);

        Application::getSingletonPtr()->setBluetoothStatus(serverAddress, true);
        Application::getSingletonPtr()->setConnectionStatus("Bluetooth", 2);
    }


    ClientBluetooth::~ClientBluetooth()
    {
        disconnect();
    }


    bool ClientBluetooth::connect(const String & serverAddress)
    {
        if (serverAddress == "")
        {
            //MESSAGE("Bluetooth", "Can't connect to " + serverAddress);

            return false;
        }

        if (isConnected)
        {
            disconnect();
        }

        this->serverAddress = serverAddress;

        //MESSAGE("Bluetooth", "Trying to connect to " + serverAddress + "...");

        // Create the sockets
        socketCtrl = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        if (socketCtrl < 0)
        {
            LOG("Bluetooth: Failed to create control socket! (%s)\n", strerror(errno));

            return false;
        }

        socketIntr = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

        if (socketIntr < 0)
        {
            close(socketCtrl);

            LOG("Bluetooth: Failed to create interrupt socket! (%s)\n", strerror(errno));

            return false;
        }

        SockAddrL2 l2Addr;
        memset(&l2Addr, 0, sizeof(l2Addr));
        l2Addr.l2_family = AF_BLUETOOTH;

        // Bind the sockets. Is this really necessary?
        /*if ((bind(socketCtrl, (struct sockaddr *) &l2Addr, sizeof(l2Addr)) < 0) || (bind(socketIntr, (struct sockaddr *) &l2Addr, sizeof(l2Addr)) < 0))
        {
            close(socketCtrl);
            close(socketIntr);

            return false;
        }*/

        // Set socket options
        struct l2cap_options opts;
        memset(&opts, 0, sizeof(opts));
        opts.imtu = 48; // HIDP_DEFAULT_MTU
        opts.omtu = 48; // HIDP_DEFAULT_MTU
        opts.flush_to = 0xffff;

        setsockopt(socketCtrl, SOL_L2CAP, L2CAP_OPTIONS, &opts, sizeof(opts));
        setsockopt(socketIntr, SOL_L2CAP, L2CAP_OPTIONS, &opts, sizeof(opts));

        // Connect the sockets
        bdaddr_t tmp;
        str2ba(serverAddress.c_str(), &tmp);
        bacpy(&l2Addr.l2_bdaddr, &tmp);
        l2Addr.l2_psm = htobs(CFG_BT_PSM_CTRL);

        if (::connect(socketCtrl, (SockAddr*) &l2Addr, sizeof(l2Addr)) < 0)
        {
            close(socketIntr);
            close(socketCtrl);

            LOG("Bluetooth: Failed to connect to control psm %i! (%s)\n", CFG_BT_PSM_CTRL, strerror(errno));
            //MESSAGE("Bluetooth", String("Connection failed! ") + strerror(errno));

            return false;
        }

        l2Addr.l2_psm = htobs(CFG_BT_PSM_INTR);

        if (::connect(socketIntr, (SockAddr*) &l2Addr, sizeof(l2Addr)) < 0)    // errno 115
        {
            close(socketIntr);
            close(socketCtrl);

            LOG("Bluetooth: Failed to connect to interrupt psm %i! (%s)\n", CFG_BT_PSM_INTR, strerror(errno));
            //MESSAGE("Bluetooth", String("Connection failed! ") + strerror(errno));

            return false;
        }
                
        //MESSAGE("Bluetooth", "Connected with " +  serverAddress);

        Application::getSingletonPtr()->setBluetoothStatus(serverAddress, true);
        Application::getSingletonPtr()->setConnectionStatus("Bluetooth", 2);

        return isConnected = true;
    }
    
    
    bool ClientBluetooth::reconnect()
    {

        return connect(serverAddress);
    }


    void ClientBluetooth::disconnect()
    {
        if (!isConnected)
        {
            return;
        }

        close(socketIntr);
        close(socketCtrl);

        isConnected = false;

        //MESSAGE("Bluetooth", "Disonnected from " +  serverAddress);

        Application::getSingletonPtr()->setBluetoothStatus("", false);
        Application::getSingletonPtr()->setConnectionStatus("Bluetooth", 1);
    }


    void ClientBluetooth::processEvents()
    {
        if (!isConnected)
        {
            return;
        }

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(socketCtrl, &fds);
        FD_SET(socketIntr, &fds);

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int s = select(socketIntr + 1, &fds, NULL, NULL, &timeout);

        if (s == 0)
        {
            return;
        }

        char buffer[100];
    
        if (FD_ISSET(socketCtrl, &fds))
        {
            int r = recv(socketCtrl, buffer, sizeof(buffer), MSG_NOSIGNAL);

            if (r == 0)
            {
                disconnect();

                return;
            }

            if (r > 0)
            {
                DBG("HID: Got command ");

                for (int i = 0; i < r; i++)
                {
                    DBG("%02x ", buffer[i]);
                }

                DBG("\n");

                // Acknowledge
                buffer[0] = 0x00;

                send(socketCtrl, buffer, 1, MSG_NOSIGNAL);
            }
        }

        if (FD_ISSET(socketIntr, &fds))
        {
            int r = recv(socketIntr, buffer, sizeof(buffer), MSG_NOSIGNAL);

            if (r == 0)
            {
                disconnect();

                return;
            }

            if (r > 0)
            {
                DBG("HID: Got report ");

                for (int i = 0; i < r; i++)
                {
                    DBG("%02x ", buffer[i]);
                }

                DBG("\n");
            }
        }
    }


    bool ClientBluetooth::sendReport(const Hid::Report * report)
    {

        if (!isConnected)
        {
            return false;
        }

        char buffer[255];
        buffer[0] = 0xa1; // data packet id
        int w = 0;
        void * data;
        int size;

        data = bootMode == true ? report->dataBoot : report->data;
        size = bootMode == true ? report->sizeBoot : report->size;

        if ((size > 0) && (data != nullptr))
        {
            memcpy(buffer + 1, data, size);

            w = send(socketIntr, buffer, size + 1, MSG_NOSIGNAL);
        }

        if (w == -1)
        {
            disconnect();
        }

        return w > 0 ? true : false;
    }

} // namespace MC

