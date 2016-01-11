#ifndef __MCPrerequisites_h__
#define __MCPrerequisites_h__

extern "C++" int at_quick_exit(void (*func)(void)) noexcept;
extern "C++" int quick_exit(void (*func)(void)) noexcept;

//#include <errno.h>
#include <fcntl.h>
/*#include <stdio.h>
#include <signal.h>
#include <stdlib.h>*/
#include <string.h>
#include <unistd.h>
/*#include <stropts.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>*/
#include <linux/input.h>
#include <aio.h>
/*#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>*/

#include <vector>
#include <set>
//#include <map>
#include <string>
#include <assert.h>
//#include <fstream>

#define vector std::vector
#define set std::set
#define string std::string
/*#define Map    std::map
#define Pair   std::pair*/

#define nullptr 0

//#define LOGFILE Application::logFile

#define LOG( ... ) fprintf(stdout, __VA_ARGS__)
#define DBG( ... ) fprintf(stdout, __VA_ARGS__)

namespace MC
{
    class Device;
    class DeviceHard;
    class DeviceVirtual;
    class DeviceManager;
    class Hub;
    class HubUsb;


} // namespace MC

#endif // __MCPrerequisites_h__