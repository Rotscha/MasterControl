#include <MCHub.h>

namespace MC
{

    Hub::Hub()
    {
        mIsEnabled = false;
        mIsDeviceConnected = false;
        device = nullptr;
    }


    Hub::~Hub()
    {
    }


    bool Hub::isEnabled()
    {
        return mIsEnabled;
    }

}
