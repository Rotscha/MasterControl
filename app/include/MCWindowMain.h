#ifndef __MCWindowMain_h__
#define __MCWindowMain_h__

extern "C++" int at_quick_exit(void (*func)(void)) noexcept;
extern "C++" int quick_exit(void (*func)(void)) noexcept;

#include <QtGui/QtGui>
#include <MCPrerequisites.h>

namespace MC
{

    class WindowMain : public QWidget
    {
        Q_OBJECT

        private:
            static WindowMain * ptr;
            Display           * dsplInputGrabber;
            QLabel            * labelInputGrabber;

            QSystemTrayIcon   * trayIcon;
            QMenu             * menuTrayIcon;
            QMenu             * menuBluetooth;
            QMenu             * menuPlugins;
            QMenu             * menuStatus;
            QAction           * actionStatusBluetooth;
            QAction           * actionStatusUSB;

        private slots:
            void                grabInput(bool grab);
            void                iconActivated(QSystemTrayIcon::ActivationReason reason);
            void                actionTriggered();

            static void         onHubEvent(const Hub & hub, const Event & event);
            static void         onDeviceEvent(const Device & device, const Event & event);

        public:
                                WindowMain();
                              ~ WindowMain();

            void                showMessage(const string & source, const string & message);
            void                setBluetoothStatus(const string & addr, bool connected);
            void                setConnectionStatus(const string & name, int status);

    }; // class WindowMain

} // namespace MC

#endif // __MCWindowMain_h__