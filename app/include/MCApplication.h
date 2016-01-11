#ifndef __MCApplication_h__
#define __MCApplication_h__

#include <MCPrerequisites.h>
#include <MCSingleton.h>
#include <QtGui/QApplication>

#define App Application::getSingletonPtr()
//#define MESSAGE(source, message) Application::getSingletonPtr()->showMessage(source, message)

namespace MC
{

    class Application : public QApplication, public Singleton <Application>
    {
        private:

/*            WindowMain         * windowMain;
            InputDeviceManager * inputDeviceManager;
            int                  timerApp;
            Display            * display;
            bool                 inputGrabbed;

            void                 timerEvent(QTimerEvent * timerEvent);

            ServerBluetooth    * serverBluetooth;
            ClientBluetooth    * clientBluetooth;
            ClientUSB          * clientUSB;

            void                 loadPlugins();*/

        public:
//            static FILE        * logFile;

                                 Application(int argc, char* argv[]);
                               ~ Application();
/*            void                 grabInput(bool grab);
            void                 showMessage(const String & source, const String & message, const bool & log = true);
            void                 bluetoothConnect(const String & addr);
            void                 bluetoothDisconnect();
            void                 setBluetoothStatus(const String & addr, bool connected);
            void                 setConnectionStatus(const String & name, int status);*/

    }; // class Application

} // namespace MC

#endif // __MCApplication_h__