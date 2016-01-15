#include <MCHub.h>
#include <MCDevice.h>

namespace MC
{

    Hub::Hub()
    {
        mIsEnabled = false;
        device = nullptr;
    }


    Hub::~Hub()
    {
    }


    bool Hub::isEnabled() const
    {
        return mIsEnabled;
    }

}
