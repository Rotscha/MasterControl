#ifndef __MCHubManager_h__
#define __MCHubManager_h__

#include <MCPrerequisites.h>

namespace MC
{

    class HubManager
    {
        private:
            typedef void        (*Callback)(const Hub & hub, const Event & event);

            map <string, Hub *>   hubs;
            set <Callback>        callbacks;
            void                  envokeCallbacks(const Hub & hub, const Event & event);

        public:
                                  HubManager();
                                ~ HubManager();

            bool                  add(Hub * hub);
            bool                  remove(Hub * hub);
            bool                  connectDevice(const string & deviceName, const string & hubName);
            void                  registerCallback(Callback callback);
            void                  unregisterCallback(Callback callback);
            void                  processEvents();

    }; // class HubManager

} // namespace MC

#endif // __MCHubManager_h__