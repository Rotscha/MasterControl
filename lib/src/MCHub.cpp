#include <MCHub.h>
#include <MCDevice.h>

namespace MC
{

    Hub::Hub(string name)
    {
        this->name = name;
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


    string Hub::getName() const
    {
        return name;
    }


    const Device * Hub::getConnectedDevice() const
    {
        return device;
    }
}
