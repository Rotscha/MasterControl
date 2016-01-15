#include <MCDeviceHard.h>
#include <MCHub.h>
#include <linux/hidraw.h>

// May not be defined in older kernel headers
#ifndef HIDIOCSFEATURE
    #define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
    #define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

namespace MC
{

    DeviceHard::DeviceHard(string node)
    {
        this->node = node;
        fd = -1;

        getInfo();
    }


    DeviceHard::~DeviceHard()
    {
        disable();
    }


    bool DeviceHard::getInfo()
    {
        int fd = open(node.c_str(), O_RDWR | O_NONBLOCK);
 
        if (fd < 0)
        {
            return false;
        }

        hidraw_report_descriptor descriptor;
        hidraw_devinfo info;

        if ((ioctl(fd, HIDIOCGRDESCSIZE, &descriptor.size) < 0) || (ioctl(fd, HIDIOCGRDESC, &descriptor) < 0) || (ioctl(fd, HIDIOCGRAWINFO, &info) < 0))
        {
            close(fd);

            return false;
        }

        reportDescriptor.resize(descriptor.size);
        memcpy((void *) reportDescriptor.data(), descriptor.value, descriptor.size);
        idVendor = info.vendor;
        idProduct = info.product;

        close(fd);
    }


    bool DeviceHard::enable()
    {
        if (fd >= 0)
        {
            return true;
        }

        fd = open(node.c_str(), O_RDWR | O_NONBLOCK);

        return fd >= 0;
    }


    void DeviceHard::disable()
    {
        if (fd >= 0)
        {
            close(fd);

            fd = -1;
        }
    }


    int DeviceHard::recvReport(void * data, int size)
    {
        if (fd < 0)
        {
            return -1;
        }

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        fd_set fdSet;
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        int s = select(fd + 1, &fdSet, NULL, NULL, &timeout);

        if (s <= 0)
        {
            return -1;
        }

        int result = read(fd, data, size);

        if (result < 0)
        {
            return -1;
        }

        return result;
    }


    int DeviceHard::sendReport(void * data, int size)
    {
        if (fd < 0)
        {
            return -1;
        }

        return write(fd, data, size);
    }


    int DeviceHard::recvFeatureReport(void * data, int size)
    {
        if (fd < 0)
        {
            return -1;
        }

        return ioctl(fd, HIDIOCGFEATURE(size), data);
    }


    int DeviceHard::sendFeatureReport(void * data, int size)
    {
        if (fd < 0)
        {
            return -1;
        }

        return ioctl(fd, HIDIOCSFEATURE(size), data);
    }

}
