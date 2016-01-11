//#include <MCApplication.h>
//#include <MCPrerequisites.h>
#include <MCDeviceManager.h>
#include <MCDevice.h>
#include <MCHubUsb.h>

#include <set>
#include <linux/uhid.h>

static unsigned char rdesc[] = {
         0x05, 0x01,     /* USAGE_PAGE (Generic Desktop) */
         0x09, 0x02,     /* USAGE (Mouse) */
         0xa1, 0x01,     /* COLLECTION (Application) */
         0x09, 0x01,             /* USAGE (Pointer) */
         0xa1, 0x00,             /* COLLECTION (Physical) */
         0x85, 0x01,                     /* REPORT_ID (1) */
         0x05, 0x09,                     /* USAGE_PAGE (Button) */
         0x19, 0x01,                     /* USAGE_MINIMUM (Button 1) */
         0x29, 0x03,                     /* USAGE_MAXIMUM (Button 3) */
         0x15, 0x00,                     /* LOGICAL_MINIMUM (0) */
         0x25, 0x01,                     /* LOGICAL_MAXIMUM (1) */
         0x95, 0x03,                     /* REPORT_COUNT (3) */
         0x75, 0x01,                     /* REPORT_SIZE (1) */
         0x81, 0x02,                     /* INPUT (Data,Var,Abs) */
         0x95, 0x01,                     /* REPORT_COUNT (1) */
         0x75, 0x05,                     /* REPORT_SIZE (5) */
         0x81, 0x01,                     /* INPUT (Cnst,Var,Abs) */
         0x05, 0x01,                     /* USAGE_PAGE (Generic Desktop) */
         0x09, 0x30,                     /* USAGE (X) */
         0x09, 0x31,                     /* USAGE (Y) */
         0x09, 0x38,                     /* USAGE (WHEEL) */
         0x15, 0x81,                     /* LOGICAL_MINIMUM (-127) */
         0x25, 0x7f,                     /* LOGICAL_MAXIMUM (127) */
         0x75, 0x08,                     /* REPORT_SIZE (8) */
         0x95, 0x03,                     /* REPORT_COUNT (3) */
         0x81, 0x06,                     /* INPUT (Data,Var,Rel) */
         0xc0,                   /* END_COLLECTION */
         0xc0,           /* END_COLLECTION */
         0x05, 0x01,     /* USAGE_PAGE (Generic Desktop) */
         0x09, 0x06,     /* USAGE (Keyboard) */
         0xa1, 0x01,     /* COLLECTION (Application) */
         0x85, 0x02,             /* REPORT_ID (2) */
         0x05, 0x08,             /* USAGE_PAGE (Led) */
         0x19, 0x01,             /* USAGE_MINIMUM (1) */
         0x29, 0x03,             /* USAGE_MAXIMUM (3) */
         0x15, 0x00,             /* LOGICAL_MINIMUM (0) */
         0x25, 0x01,             /* LOGICAL_MAXIMUM (1) */
         0x95, 0x03,             /* REPORT_COUNT (3) */
         0x75, 0x01,             /* REPORT_SIZE (1) */
         0x91, 0x02,             /* Output (Data,Var,Abs) */
         0x95, 0x01,             /* REPORT_COUNT (1) */
         0x75, 0x05,             /* REPORT_SIZE (5) */
         0x91, 0x01,             /* Output (Cnst,Var,Abs) */
         0xc0,           /* END_COLLECTION */
 };


int uhid_write(int fd, const struct uhid_event *ev)
{
         ssize_t ret;
 
         ret = write(fd, ev, sizeof(*ev));
         if (ret < 0) {
                 fprintf(stderr, "Cannot write to uhid: %m\n");
                 return -errno;
         } else if (ret != sizeof(*ev)) {
                 fprintf(stderr, "Wrong size written to uhid: %ld != %lu\n",
                         ret, sizeof(ev));
                 return -EFAULT;
         } else {
                 return 0;
         }
 } 
 
int create(int fd)
{
        struct uhid_event ev;

         memset(&ev, 0, sizeof(ev));
         ev.type = UHID_CREATE;
         strcpy((char*)ev.u.create.name, "test-uhid-device");
         ev.u.create.rd_data = rdesc;
         ev.u.create.rd_size = sizeof(rdesc);
         ev.u.create.bus = BUS_USB;
         ev.u.create.vendor = 0x15d9;
         ev.u.create.product = 0x0a37;
         ev.u.create.version = 0;
         ev.u.create.country = 0;
 
         return uhid_write(fd, &ev);
 } 
 
void destroy(int fd)
{
         struct uhid_event ev;
 
         memset(&ev, 0, sizeof(ev));
         ev.type = UHID_DESTROY;
 
         uhid_write(fd, &ev);
	 close(fd);
}

void hotplug(MC::Device & device, bool action)
{
 // printf("%s: %s\n", a == true ? "Added" : "Removed", n.c_str());
  printf("%s: %s\n", action == true ? "Added" : "Removed", "");
}

void report(const char* c, int)
{
  printf("report\n");
}

int main(int argc, char * argv[])
{
  
/* int fd = open("/dev/uhid", O_RDWR | O_CLOEXEC);
         if (fd < 0) {
                 fprintf(stderr, "Cannot open uhid-cdev %s: %m\n", "/dev/uhid");
                 return EXIT_FAILURE;
         }
 
         fprintf(stderr, "Create uhid device\n");
         int ret = create(fd);
         if (ret) {
                 close(fd);
                 return EXIT_FAILURE;
         }

 sleep(10);     
    destroy(fd);
    return 0;*/
    

    using namespace MC;

    DeviceManager * dev = new DeviceManager();
    dev->registerHotplugCallback(hotplug);
    dev->registerReportCallback(report);
    
    HubUsb * usb = new HubUsb("/dev/gadgetfs");
    //usb->enable();
    usb->connectDevice(dev->getDevice());
    
    while(true)
    {
      dev->processEvents();
      usb->processEvents();
    }
    
    delete usb;
    delete dev;

//    Application app(argc, argv);
//    app.setApplicationName("Master Control");

/*    while (true)
    {
      app.processEvents(QEventLoop::AllEvents | QEventLoop::WaitForMoreEvents);
    }*/
//    return app.exec();
}

// export PATH=$PATH:/home/ghetto/pandora-dev/arm-2011.09/bin/
// export PATH=$PATH:/home/ghetto/pandora-dev/arm-2014.05/bin/

