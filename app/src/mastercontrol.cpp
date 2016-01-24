#include <MCApplication.h>

int main(int argc, char * argv[])
{
    using namespace MC;

    Application app(argc, argv);
    app.setApplicationName("Master Control");

    return app.exec();
}

// export PATH=$PATH:~/pandora-dev/arm-2014.05/bin/

