#include "SimpleWm.h"
#include "WmFrame.h"

int main(int argc, char **argv) {
    SimpleWm app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.initX11();

    return app.exec();
}
