#include <MCDevice.h>
#include <MCHub.h>

namespace MC
{

    Device::Device()
    {
        node = "";
        idVendor = 0;
        idProduct = 0;
        strVendor = "";
        strProduct = "";
        reportDescriptor = "";
    }


    Device::~Device()
    {
        //disconnect();
    }


    string Device::getName() const
    {
        return node;
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
 
