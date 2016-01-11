#include <MCDevice.h>
#include <linux/hidraw.h>

namespace MC
{

    Device::Device()
    {
        mIsEnabled = false;
        node = "";
        idVendor = 0;
        idProduct = 0;
        strVendor = "";
        strProduct = "";
        reportDescriptor = "";
    }


    Device::~Device()
    {
    }


    bool Device::isEnabled() const
    {
        return mIsEnabled;
    }


    string Device::getNode() const
    {
        return node;
    }


    int Device::getVendorId() const
    {
        return idVendor;
    }


    int Device::getProductId() const
    {
        return idProduct;
    }


    string Device::getReportDescriptor() const
    {
        return reportDescriptor;
    }

}
 
