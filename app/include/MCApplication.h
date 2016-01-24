#ifndef __MCApplication_h__
#define __MCApplication_h__

extern "C++" int at_quick_exit(void (*func)(void)) noexcept;
extern "C++" int quick_exit(void (*func)(void)) noexcept;

#include <QtGui/QApplication>
#include <MCWindowMain.h>
#include <MCMain.h>

//#define App Application::getSingletonPtr()
//#define MESSAGE(source, message) Application::getSingletonPtr()->showMessage(source, message)

namespace MC
{

    class Application : public QApplication
    {
        private:

            WindowMain         * windowMain;
            Main               * main;
/*            InputDeviceManager * inputDeviceManager;
            Display            * display;
            bool                 inputGrabbed;*/

            void                 timerEvent(QTimerEvent * timerEvent);

            /*ServerBluetooth    * serverBluetooth;
            ClientBluetooth    * clientBluetooth;
            ClientUSB          * clientUSB;*/

            void                 loadPlugins();

        public:
//            static FILE        * logFile;

                                 Application(int & argc, char** argv);
                               ~ Application();
            void                 grabInput(bool grab);
            void                 showMessage(const string & source, const string & message, const bool & log = true);

    }; // class Application

} // namespace MC

#endif // __MCApplication_h__