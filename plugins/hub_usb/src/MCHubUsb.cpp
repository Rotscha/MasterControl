#include <MCHubUsb.h>
#include <MCDevice.h>

#include <linux/usb/gadgetfs.h>
#include <linux/usb/ch9.h>
#include <linux/hid.h>
#include <asm/byteorder.h>

#define NAME_EPXIN            "ep%iin"
#define NAME_EPXOUT           "ep%iout"
#define MAX_PACKET_SIZE       0x40

#define NUM_CONFIGURATIONS    0x01
#define CONFIG_ID_DEFAULT     0x01

#define ENDPOINT_INTR_IN      0x81
#define ENDPOINT_INTR_OUT     0x02

#define STRING_ID_VENDOR      0x01
#define STRING_ID_PRODUCT     0x02
#define STRING_ID_SERIAL      0x00
#define STRING_ID_CONFIG      0x04
#define STRING_ID_KEYBOARD    0x05
#define STRING_ID_MOUSE       0x06
#define STRING_ID_GAMEPAD     0x07

#define VENDOR_ID             0x0525	/* XXX NetChip */
#define PRODUCT_ID            0xa4ac	/* Linux-USB HID gadget */


namespace MC
{

    // Redeclaration of some HID structures, because "linux/hid.h"
    // provides these only to the kernel (in version 3.2)
    struct hid_class_descriptor
    {
        __u8 bDescriptorType;
        __le16 wDescriptorLength;
    } __attribute__ ((packed));


    struct hid_descriptor
    {
        __u8 bLength;
        __u8 bDescriptorType;
        __le16 bcdHID;
        __u8 bCountryCode;
        __u8 bNumDescriptors;
        struct hid_class_descriptor desc[1];
    } __attribute__ ((packed));


    static const char16_t * strings[]
    {
        u"\u0409",
        u"Pandora",
        u"Pandora HID",
        u"pandora_hid_ver_001_by_crionic",
        u"Default configuration",
        u"Pandora HID Keyboard",
        u"Pandora HID Mouse",
        u"Pandora HID Gamepad"
    };


    typedef usb_device_descriptor    DeviceDescriptor;
    typedef usb_config_descriptor    ConfigDescriptor;
    typedef usb_interface_descriptor InterfaceDescriptor;
    struct                           EndpointDescriptor
    {
        __u8   bLength;
        __u8   bDescriptorType;
        __u8   bEndpointAddress;
        __u8   bmAttributes;
        __le16 wMaxPacketSize;
        __u8   bInterval;
    } __attribute__((packed));
    typedef hid_descriptor           HIDDescriptor;


    struct DeviceConfig
    {
        struct Interface
        {
            InterfaceDescriptor interface;
            HIDDescriptor       hid;
            EndpointDescriptor  endpoints[2];
            //EndpointDescriptor  endpointOut;
        } __attribute__((packed));

        struct Config
        {
            ConfigDescriptor    config;
            Interface           interface;
        } __attribute__((packed));
    
        __u32                   id;
        Config                  fullSpeed;
        Config                  highSpeed;
        DeviceDescriptor        device;
    } __attribute__((packed));


    static DeviceConfig deviceConfig
    {                                                               // struct DeviceConfig
        0,                                                          // __u32  id
        {                                                           // struct SpeedConfig full
            {                                                       // ConfigDescriptor config
                sizeof(ConfigDescriptor),                           // __u8   bLength
                USB_DT_CONFIG,                                      // __u8   bDescriptorType
                sizeof(DeviceConfig::Config),                       // __le16 wTotalLength
                1,                                                  // __u8   bNumInterfaces
                CONFIG_ID_DEFAULT,                                  // __u8   bConfigurationValue
                STRING_ID_CONFIG,                                   // __u8   iConfiguration
                USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,      // __u8   bmAttributes
                0x32                                                // __u8   bMaxPower
            },
            {
                {                                                   // InterfaceDescriptor interfaces[0]
                    sizeof(InterfaceDescriptor),                    // __u8   bLength;
                    USB_DT_INTERFACE,                               // __u8   bDescriptorType
                    0,                                              // __u8   bInterfaceNumber
                    0,                                              // __u8   bAlternateSetting
                    2,                                              // __u8   bNumEndpoints
                    USB_CLASS_HID,                                  // __u8   bInterfaceClass
                    USB_INTERFACE_SUBCLASS_BOOT,                    // __u8   bInterfaceSubClass: 0x00 None; 0x01 Boot Interface
                    USB_INTERFACE_PROTOCOL_KEYBOARD,                // __u8   bInterfaceProtocol: 0x00 None; 0x01 Keyboard, 0x02 Mouse
                    STRING_ID_KEYBOARD                              // __u8   iInterface
                },
                {                                                   // HIDDescriptor hid
                    sizeof(HIDDescriptor),                          // __u8   bLength
                    HID_DT_HID,                                     // __u8   bDescriptorType
                    __constant_cpu_to_le16(0x0111),                 // __le16 bcdHID
                    0,                                              // __u8   bCountryCode
                    1,                                              // __u8   bNumDescriptors
                    {                                               // struct hid_class_descriptor desc[1]
                        HID_DT_REPORT,                              // __u8   bDescriptorType
                        0,                                          // __le16 wDescriptorLength (needs to be calculated)
                    }
                },
                {                                                   // struct EndpointDescriptor[]
                    {                                               // EndpointDescriptor[0]
                        sizeof(EndpointDescriptor),                 // __u8   bLength
                        USB_DT_ENDPOINT,                            // __u8   bDescriptorType
                        ENDPOINT_INTR_IN,                           // __u8   bEndpointAddress
                        USB_ENDPOINT_XFER_INT,                      // __u8   bmAttributes
                        __constant_cpu_to_le16(MAX_PACKET_SIZE),    // __le16 wMaxPacketSize
                        0x10                                        // __u8   bInterval
                    },
                    {                                               // EndpointDescriptor[1]
                        sizeof(EndpointDescriptor),                 // __u8   bLength
                        USB_DT_ENDPOINT,                            // __u8   bDescriptorType
                        ENDPOINT_INTR_OUT,                          // __u8   bEndpointAddress
                        USB_ENDPOINT_XFER_INT,                      // __u8   bmAttributes
                        __constant_cpu_to_le16(MAX_PACKET_SIZE),    // __le16 wMaxPacketSize
                        0x10                                        // __u8   bInterval
                    }
                }
            }
        },
        {
            deviceConfig.fullSpeed.config,
            deviceConfig.fullSpeed.interface.interface,
            deviceConfig.fullSpeed.interface.hid,
            deviceConfig.fullSpeed.interface.endpoints[0].bLength,
            deviceConfig.fullSpeed.interface.endpoints[0].bDescriptorType,
            deviceConfig.fullSpeed.interface.endpoints[0].bEndpointAddress,
            deviceConfig.fullSpeed.interface.endpoints[0].bmAttributes,
            deviceConfig.fullSpeed.interface.endpoints[0].wMaxPacketSize,
            0x04,
            deviceConfig.fullSpeed.interface.endpoints[1].bLength,
            deviceConfig.fullSpeed.interface.endpoints[1].bDescriptorType,
            deviceConfig.fullSpeed.interface.endpoints[1].bEndpointAddress,
            deviceConfig.fullSpeed.interface.endpoints[1].bmAttributes,
            deviceConfig.fullSpeed.interface.endpoints[1].wMaxPacketSize,
            0x04
        },
        //deviceConfig.fullSpeed,                                         // High speed configuration is the same as full speed
        {                                                               // DeviceDescriptor device
            sizeof(DeviceDescriptor),                                   // __u8   bLength
            USB_DT_DEVICE        ,                                      // __u8   bDescriptorType
            __constant_cpu_to_le16(0x0200),                             // __le16 bcdUSB                0100h - USB Specification 1.0, 0110h - USB Specification 1.1, 0200h - USB Specification 2.0
            USB_CLASS_PER_INTERFACE,                                    // __u8   bDeviceClass
            0,                                                          // __u8   bDeviceSubClass
            0,                                                          // __u8   bDeviceProtocol
            MAX_PACKET_SIZE,                                            // __u8   bMaxPacketSize0       8, 16, 32, 64
            __constant_cpu_to_le16(0x054c),                          // __le16 idVendor
            __constant_cpu_to_le16(0x0268),                         // __le16 idProduct
//            __constant_cpu_to_le16(VENDOR_ID),                          // __le16 idVendor
//            __constant_cpu_to_le16(PRODUCT_ID),                         // __le16 idProduct
            __constant_cpu_to_le16(0x0100),                             // __le16 bcdDevice
            STRING_ID_VENDOR,                                           // __u8   iManufacturer
            STRING_ID_PRODUCT,                                          // __u8   iProduct
            STRING_ID_SERIAL,                                           // __u8   iSerialNumber
            NUM_CONFIGURATIONS                                          // __u8   bNumConfigurations
        }
    };


    HubUsb::HubUsb(string path) : Hub("USB")
    {
        this->path = path;
        device = nullptr;

        endpoints[0].aio_fildes = -1;
        endpoints[1].aio_fildes = -1;
        endpoints[2].aio_fildes = -1;
    }


    HubUsb::~HubUsb()
    {
        disable();
    }


    bool HubUsb::enable()
    {
        if (mIsEnabled)
        {
            return true;
        }

        string gadgetFS = path + '/' + "musb-hdrc";

        if ((endpoints[0].aio_fildes = open(gadgetFS.c_str(), O_RDWR, 0666)) < 0)
        {
            LOG("USB: Failed to open file '%s'! (%s)\n", gadgetFS.c_str(), strerror(errno));

            return false;
        }

        return mIsEnabled = true;
    }


    void HubUsb::disable()
    {
        closeEndpoints();

        if (mIsEnabled)
        {
            close(endpoints[0].aio_fildes);

            mIsEnabled = false;
        }
    }


    bool HubUsb::connectDevice(Device * device)
    {
        if (device == nullptr)
        {
            return false;
        }

        if (this->device == device)
        {
            return true;
        }

        if ((!enable()) || (!device->enable()))
        {
            return false;
        }

        if (this->device != nullptr)
        {
            this->device->disable();
        }

        this->device = device;

        //******//

        deviceConfig.fullSpeed.interface.hid.desc[0].wDescriptorLength = deviceConfig.highSpeed.interface.hid.desc[0].wDescriptorLength = __constant_cpu_to_le16(device->getReportDescriptor().length());

        int w = write(endpoints[0].aio_fildes, &deviceConfig, sizeof(deviceConfig));

        if (w != sizeof(deviceConfig))
        {
            LOG("USB: Failed to initialize ep0! (%s)\n", strerror(errno));

//            close(endpoints[0].aio_fildes);

            disconnectDevice();

            return false;
        }

        LOG("USB: Ep0 initialized. Device connected.\n");

//        Application::getSingletonPtr()->setConnectionStatus("USB", 1);

        return true;
    }


    void HubUsb::disconnectDevice()
    {
        closeEndpoints();

        if (device != nullptr)
        {
            device->disable();
        }

        device = nullptr;
    }


    void HubUsb::closeEndpoints()
    {
        for (int i = 1; i < 3; i++)
        {
            if (endpoints[i].aio_fildes >= 0)
            {
                aio_cancel(endpoints[i].aio_fildes, &endpoints[i]);
                close(endpoints[i].aio_fildes);
                endpoints[i].aio_fildes = -1;
            }
        }
    }


    void HubUsb::processRequest(const usb_ctrlrequest & request)
    {
        __u8 requestDirection = request.bRequestType & USB_DIR_IN;         // Bit 7
        __u8 requestType      = request.bRequestType & USB_TYPE_MASK;      // Bits 6-5
        __u8 requestRecipient = request.bRequestType & USB_RECIP_MASK;     // Bits 4-0

        DBG("USB: Got request! (bRequestType=0x%02x, bRequest=0x%02x, wValue=0x%04x, wIndex=0x%04x, wLength=0x%04x)\n", request.bRequestType, request.bRequest, request.wValue, request.wIndex, request.wLength);

        // Bitmap 10000000
        if ((requestDirection == USB_DIR_IN) && (requestType == USB_TYPE_STANDARD) && (requestRecipient == USB_RECIP_DEVICE))
        {
            switch (request.bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR:
                {
                    DBG("USB: Request id: USB_REQ_GET_DESCRIPTOR\n");

                    // Descriptor type is high byte, descriptor index is low byte
                    const __u8 & type  = ((__u8 *) & request.wValue)[1];
                    const __u8 & index = ((__u8 *) & request.wValue)[0];

                    switch (type)
                    {
                        case USB_DT_STRING:
                        {
                            DBG("USB: Descriptor id: USB_DT_STRING\n");
                            DBG("USB: Parameters: index=%i, language=0x%04x\n", index, request.wIndex);

                            if (index > (sizeof(strings) / sizeof(char16_t*)) - 1)
                            {
                                DBG("USB: Index is invalid! ");

                                break;
                            }

                            if ((index != 0) && (request.wIndex != 0x0409))
                            {
                                DBG("USB: Language is not supported! ");

                                break;
                            }

                            __u8 buffer[request.wLength];
                            memset(buffer, 0, request.wLength);

                            int strLen = std::char_traits<char16_t>::length(strings[index]) * 2;
                            int pktLen = strLen + 2;

                            if (pktLen > request.wLength)
                            {
                                pktLen = request.wLength;
                            }

                            buffer[0] = pktLen;
	                    buffer[1] = USB_DT_STRING;
                            memcpy(buffer + 2, strings[index],  pktLen - 2);

                            int w = write(endpoints[0].aio_fildes, buffer, pktLen);

                            DBG("USB: Sending string: %s!\n", w != pktLen ? "Failed" : "Ok");

                            return;
                        }
                        
                        default:
                        {
                            DBG("USB: Unsupported descriptor (0x%02x)! ", type);
                        }
                    }

                    break;
                }

                default:
                {
                    DBG("USB: Unsupported request (0x%02x)! ", request.bRequest);
                }
            }

            DBG("Stalling...\n");

            read(endpoints[0].aio_fildes, 0, 0);

            return;
        }
        

        // Bitmap 00000000
        if ((requestDirection == USB_DIR_OUT) && (requestType == USB_TYPE_STANDARD) && (requestRecipient == USB_RECIP_DEVICE))
        {
            switch (request.bRequest)
            {
                case USB_REQ_SET_CONFIGURATION:
                {
                    // Desired configuration is low byte, hight byte is reserved
                    const __u8 & config   = ((__u8 *) & request.wValue)[0];
                    const __u8 & reserved = ((__u8 *) & request.wValue)[1];

                    DBG("USB: Request id: USB_REQ_SET_CONFIGURATION\n");
                    DBG("USB: Parameters: config=%i\n", config);
                    DBG("USB: Setting up...\n");

                    // Handle undefined request behavior
                    if ((reserved != 0) || (request.wIndex != 0) || (request.wLength != 0))
                    {
                        DBG("USB: Undefined request parameters! ");

                        break;
                    }

                    bool errors = false;

                    switch (config)
                    {
                        case 0:
                        {
                            DBG("USB: Nothing to do! ");

                            break;
                        }

                        case CONFIG_ID_DEFAULT:
                        {

//                            for (int i = 0; i < deviceConfig.highSpeed.interface.interface.bNumEndpoints; i++)
                            for (int i = 0; i < 2; i++)
                            {
                                EndpointDescriptor & epd = deviceConfig.fullSpeed.interface.endpoints[i];
                                Endpoint & ep = endpoints[i + 1];
                                memset(&ep, 0, sizeof(ep));
                                const char * epDir = (epd.bEndpointAddress & 0x80) == 0 ? "out" : "in";

                                char epxDir[16];
                                memset(epxDir, 0, 16);
                                sprintf(epxDir, "ep%i%s", epd.bEndpointAddress & 0x7F, epDir);

                                string epName = path + '/';
                                epName += epxDir;

                                if ((ep.aio_fildes = open(epName.c_str(), O_RDWR, 0666)) < 0)
                                {
                                    DBG("USB: Failed to open '%s'! (%s)\n", epName.c_str(), strerror(errno));

                                    errors = true;
                                }

                                char buffer[4 + sizeof(EndpointDescriptor) * 2];
                                *(__u32 *) buffer = 0x01;
                                ((EndpointDescriptor*) (buffer + 4))[0] = deviceConfig.fullSpeed.interface.endpoints[i];
                                ((EndpointDescriptor*) (buffer + 4))[1] = deviceConfig.highSpeed.interface.endpoints[i];

                                if (write(ep.aio_fildes, buffer, sizeof(buffer)) != sizeof(buffer))
                                {
                                    DBG("USB: Failed to initialize endpoint 0x%02x! (%s)\n", epd.bEndpointAddress, strerror(errno));

                                    close(ep.aio_fildes);

                                    ep.aio_fildes = -1;
                                    errors = true;
                                }
                                else
                                {
                                    DBG("USB: Endpoint 0x%02x initialized.\n", epd.bEndpointAddress, strerror(errno));

                                    ep.aio_buf = malloc(MAX_PACKET_SIZE);
                                    ep.aio_nbytes = MAX_PACKET_SIZE;
                                    ep.direction = (epd.bEndpointAddress & 0x80) == 0 ? USB_DIR_OUT : USB_DIR_IN;
                                }

                            }


/*                            int epNum = ENDPOINT_INTR_IN & 0x7F;
                            char epxIn[255];
                            memset(epxIn, 0, 255);
                            sprintf(epxIn, NAME_EPXIN, epNum);

                            string epName = path + '/';
                            epName += epxIn;

                            if ((endpoints[1].aio_fildes = open(epName.c_str(), O_RDWR, 0666)) < 0)
                            {
                                DBG("USB: Failed to open '%s'! (%s)\n", epName.c_str(), strerror(errno));

                                errors = true;
                            }

                            char buffer[4 + sizeof(EndpointDescriptor) * 2];
                            *(__u32 *) buffer = 0x01;
                            ((EndpointDescriptor*) (buffer + 4))[0] = deviceConfig.fullSpeed.interface.endpointIn;
                            ((EndpointDescriptor*) (buffer + 4))[1] = deviceConfig.highSpeed.interface.endpointIn;

                            if (write(endpoints[1].aio_fildes, buffer, sizeof(buffer)) != sizeof(buffer))
                            {
                                DBG("USB: Failed to initialize endpoint %i! (%s)\n", epNum, strerror(errno));

                                close(endpoints[1].aio_fildes);

                                endpoints[1].aio_fildes = -1;

                                errors = true;
                            }

//**************
                            epNum = ENDPOINT_INTR_OUT;
                            char epxOut[255];
                            memset(epxOut, 0, 255);
                            sprintf(epxOut, NAME_EPXOUT, epNum);

                            epName = path + '/';
                            epName += epxOut;

                            if ((endpoints[2].aio_fildes = open(epName.c_str(), O_RDWR, 0666)) < 0)
                            {
                                DBG("USB: Failed to open '%s'! (%s)\n", epName.c_str(), strerror(errno));

                                errors = true;
                            }

                            *(__u32 *) buffer = 0x01;
                            ((EndpointDescriptor*) (buffer + 4))[0] = deviceConfig.fullSpeed.interface.endpointOut;
                            ((EndpointDescriptor*) (buffer + 4))[1] = deviceConfig.highSpeed.interface.endpointOut;

                            if (write(endpoints[2].aio_fildes, buffer, sizeof(buffer)) != sizeof(buffer))
                            {
                                DBG("USB: Failed to initialize endpoint %i! (%s)\n", epNum, strerror(errno));

                                close(endpoints[2].aio_fildes);

                                endpoints[2].aio_fildes = -1;

                                errors = true;
                            }
//**************
*/
                            int r = read(endpoints[0].aio_fildes, 0, 0);

                            DBG("USB: Setup complete!%s\n", errors == true ? " (with errors)" : "");
                            DBG("USB: Acknowledging: %s!\n", r == 0 ? "Ok" : "Failed");

//                            mIsDeviceConnected = true;

                            return;
                        }

                        default:
                        {
                            DBG("USB: Unsupported configuration! ");
                        }
                    }

                    break;
                }

                default:
                {
                    DBG("USB: Unsupported request (0x%02x)! ", request.bRequest);
                }
            }

            DBG("Stalling...\n");

            read(endpoints[0].aio_fildes, 0, 0);

            return;
        }

        
        // Bitmap 00100001, HID-Stuff
        if ((requestDirection == USB_DIR_OUT) && (requestType == USB_TYPE_CLASS) && (requestRecipient == USB_RECIP_INTERFACE))
        {
            switch (request.bRequest)
            {
                case HID_REQ_SET_IDLE:
                {
                    int r = read(endpoints[0].aio_fildes, 0, 0);

                    DBG("USB: Request id: HID_SET_IDLE\n");
                    DBG("USB: Parameters: interface=%i\n", request.wIndex);
                    DBG("USB: Acknowledging: %s!\n", r == 0 ? "Ok" : "Failed");

                    return;
                }

                // Required only for boot devices
                // Activates boot or user defined protocol for a given interface
                case HID_REQ_SET_PROTOCOL:
                {
                    int r = read(endpoints[0].aio_fildes, 0, 0);

                    DBG("USB: Request id: HID_SET_PROTOCOL\n");
                    DBG("USB: Parameters: interface=%i, protocol='%s'\n", request.wIndex, request.wValue == 0 ? "boot" : "user");
                    DBG("USB: Acknowledging: %s!\n", r == 0 ? "Ok" : "Failed");

                    return;
                }

                // HID report transfer host -> device
                case HID_REQ_SET_REPORT:
                {
                    DBG("USB: Request id: HID_SET_REPORT\n");

                    char reportType = request.wValue >> 0x08;
                    char reportId = request.wValue & 0xFF;
                    const char * reportTypeStr = reportType == 1 ? "IN" : reportType == 2 ? "OUT" : "FEATURE";

                    DBG("USB: Parameters: reporttype=%i (%s), reportid=%i, reportlength=%i\n", reportType, reportTypeStr, reportId, request.wLength);

                    char buf[request.wLength + 1];
                    buf[0] = reportId;

                    int r = read(endpoints[0].aio_fildes, reportId == 0 ? buf : &buf[1], request.wLength);

                    DBG("USB: Reading report from host: %s!\n", r == request.wLength ? "Ok" : "Failed");

                    if (r != request.wLength)
                    {
                        return;
                    }

                    if ((reportType == 3 /* FEATURE report */) || (reportType == 2 /* OUT report */))
                    {
                        int size = reportId == 0 ? request.wLength : request.wLength + 1;
                        int s;

                        if (reportType == 3)
                        {
                            s = device->sendFeatureReport(buf, size);
                        }
                        else
                        {
                            s = device->sendReport(buf, size);
                        }

                        DBG("USB: Sending report to device: %s!\n", s == size ? "Ok" : "Failed");
                    }
                    else
                    {
                        DBG("USB: Unsupported report type!\n");
                    }

                    return;
                }

                default:
                {
                    DBG("USB: Unsupported request (0x%02x)! ", request.bRequest);
                }
            }

            DBG("Stalling...\n");

            read(endpoints[0].aio_fildes, 0, 0);

            return;
        }


        // Bitmap 10000001
        if ((requestDirection == USB_DIR_IN) && (requestType == USB_TYPE_STANDARD) && (requestRecipient == USB_RECIP_INTERFACE))
        {
            switch (request.bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR:
                {
                    DBG("USB: Request id: USB_REQ_GET_DESCRIPTOR\n");

                    // Descriptor type is high byte, descriptor index is low byte
                    const __u8 & type  = ((__u8 *) & request.wValue)[1];
                    const __u8 & index = ((__u8 *) & request.wValue)[0];

                    switch (type)
                    {
                        case HID_DT_REPORT:
                        {
                            DBG("USB: Descriptor id: HID_DT_REPORT\n");
                            DBG("USB: Parameters: interface=%i, index=%i\n", request.wIndex, index);

                            string descriptor = device->getReportDescriptor();

                            int w = write(endpoints[0].aio_fildes, descriptor.data(), descriptor.length());

                            DBG("USB: Sending report: %s!\n", w != descriptor.length() ? "Failed" : "Ok");

                            if (w == descriptor.length())
                            {
//                                bootMode = false;
//mIsDeviceConnected = true;

                            }

                            return;
                        }
                        
                        default:
                        {
                            DBG("USB: Unsupported descriptor (0x%02x)! ", type);
                        }
                    }

                    break;
                }

                default:
                {
                    DBG("USB: Unsupported request (0x%02x)! ", request.bRequest);
                }
            }

            DBG("Stalling...\n");

            read(endpoints[0].aio_fildes, 0, 0);

            return;
        }


        // Bitmap 10100001, HID-Stuff
        if ((requestDirection == USB_DIR_IN) && (requestType == USB_TYPE_CLASS) && (requestRecipient == USB_RECIP_INTERFACE))
        {
            switch (request.bRequest)
            {
                case HID_REQ_GET_REPORT:
                {
                    DBG("USB: Request id: HID_GET_REPORT\n");

                    char reportType = request.wValue >> 0x08;
                    char reportId = request.wValue & 0xFF;
                    const char * reportTypeStr = reportType == 1 ? "IN" : reportType == 2 ? "OUT" : "FEATURE";

                    DBG("USB: Parameters: reporttype=%i (%s), reportid=%i, reportlength=%i\n", reportType, reportTypeStr, reportId, request.wLength);

                    char buf[request.wLength + 1];
                    memset(buf, 0, request.wLength + 1);
                    buf[0] = reportId;
                    int w;

                    if ((reportType == 3) /* FEATURE report */ || (reportType == 1 /* IN report */))
                    {
                        int size = reportId == 0 ? request.wLength : request.wLength + 1;
                        int r;

                        if (reportType == 3)
                        {
                            r = device->recvFeatureReport(buf, size);

                            DBG("USB: Reading report from device: %s!\n", r == size ? "Ok" : "Failed");
                        }
                        else
                        {
                            DBG("USB: Reading specific report from device IN ep not supported!\n");
                        }
                    }
                    else
                    {
                        DBG("USB: Unsupported report type!\n");
                    }

                    w = write(endpoints[0].aio_fildes, reportId == 0 ? buf : &buf[1], request.wLength);

                    DBG("USB: Sending report to host: %s!\n", w == request.wLength ? "Ok" : "Failed");

                    return;
                }

                default:
                {
                    DBG("USB: Unsupported request (0x%02x)! ", request.bRequest);
                }
            }

            DBG("Stalling...\n");

            read(endpoints[0].aio_fildes, 0, 0);

            return;
        }

        DBG("USB: Unsupported request type '0x%02x'!\n", request.bRequestType);

        read(endpoints[0].aio_fildes, 0, 0);
    }


    void HubUsb::processUsbEvents()
    {
        if (!mIsEnabled)
        {
            return;
        }

        fd_set fds;
        FD_ZERO(&fds);

        FD_SET(endpoints[0].aio_fildes, &fds);

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int s = select(endpoints[0].aio_fildes + 1, &fds, NULL, NULL, &timeout);

        if (s == 0)
        {
            return;
        }

        usb_gadgetfs_event event;

        int r = read(endpoints[0].aio_fildes, &event, sizeof(event));

        if (r != sizeof(event))
        {
            DBG("USB: Error handling event!\n");

            return;
        }

        switch(event.type)
        {
            case GADGETFS_CONNECT:
            {
                DBG("USB: Connected! (Speed: %i)\n", event.u.speed);

//                Application::getSingletonPtr()->setConnectionStatus("USB", 2);

                break;
            }

            case GADGETFS_SETUP:
            {
                processRequest(event.u.setup);

                break;
            }

            case GADGETFS_SUSPEND:
            {
                DBG("USB: Suspended!\n");
//                bootMode = true;
                closeEndpoints();

//                Application::getSingletonPtr()->setConnectionStatus("USB", 1);

                break;
            }

            case GADGETFS_DISCONNECT:
            {
                DBG("USB: Disconnected!\n");
//                bootMode = true;
                closeEndpoints();

//                Application::getSingletonPtr()->setConnectionStatus("USB", 1);

                break;
            }

            case GADGETFS_NOP:
            {
                break;
            }

            default:
            {
                DBG("USB Unhandled event (%i)!\n", event.type);
            }
        }
    }


    void HubUsb::processEvents()
    {
        processUsbEvents();

        if (device == nullptr)
        {
            return;
        }

        char buf[255];
        int  size = sizeof(buf);

        for (int i = 1; i < 3; i++)
        {
            Endpoint & ep = endpoints[i];

            if (ep.aio_fildes < 0)
            {
                continue;
            }

            int ret = aio_return(&ep);

            if (ret == EINPROGRESS)
            {
                continue;
            }

            if (ep.direction == USB_DIR_IN)
            {
                int r;

                if ((r = device->recvReport((void *) ep.aio_buf, MAX_PACKET_SIZE)) > 0)
                {
                    ep.aio_nbytes = r;

                    aio_write(&ep);
                }
            }
            else
            {
                if (ret > 0)
                {
                    device->sendReport((char *) ep.aio_buf, ret);
                }

                aio_read(&ep);
            }
        }
    }

}
