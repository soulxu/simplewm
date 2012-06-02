#include "SimpleWm.h"

#include <QDesktopWidget>
#include <QX11Info>

#include "WmFrame.h"

#include <X11/Xutil.h>
#include <X11/Xatom.h>
SimpleWm::SimpleWm(int &argc, char **argv) : QApplication(argc, argv) {


}

SimpleWm::~SimpleWm() {
}

bool SimpleWm::initX11() {
    QDesktopWidget *desktop = this->desktop();
    const QX11Info &x11info = desktop->x11Info();
    XSelectInput(x11info.display(), x11info.appRootWindow(), SubstructureNotifyMask | SubstructureRedirectMask);
    return true;
}

bool SimpleWm::x11EventFilter(XEvent *event) {
    const QX11Info &x11Info = this->desktop()->x11Info();
    Atom atom_wmframe = XInternAtom(QX11Info::display(), "wmframe", FALSE);
    Atom ret_type;
    int ret_format;
    ulong ret_nitems;
    ulong ret_bytes;
    quint32 *data = NULL;
    int ret;
    XWindowAttributes attr;
    WmFrame *f = NULL;
    switch (event->type) {
    case MapNotify:
        qDebug("MapNotify: %ld", event->xmap.window);


        break;
    case ConfigureRequest:
        qDebug("ConfigureRequest: %ld = %d, %d", event->xconfigurerequest.window,
               event->xconfigurerequest.width, event->xconfigurerequest.height);

//        f = this->m_frameList.value(event->xany.window);

//        if (f == NULL) {
//            return false;
//        }

        qDebug("resizeEmbed %ld", event->xany.window);
//        f->resizeEmbed(attr.width, attr.height);
        XResizeWindow(QX11Info::display(), event->xany.window, event->xconfigurerequest.width, event->xconfigurerequest.height);
        break;

    case MapRequest:
        qDebug("MapRequest: %ld. %ld", event->xany.window, event->xmaprequest.window);
        XMapWindow(QX11Info::display(), event->xmaprequest.window);
        /*
        ret = XGetWindowProperty(QX11Info::display(),
                           event->xany.window,
                           atom_wmframe,
                           0,
                           8,
                           FALSE,
                           AnyPropertyType,
                           &ret_type,
                           &ret_format,
                           &ret_nitems,
                           &ret_bytes,
                           (unsigned char **)&data);

        if (ret != Success) {
            return false;
        }

        if ((data != NULL) && (*data == 1)) {
            return false;
        }

        if (event->xmaprequest.parent == DefaultRootWindow(QX11Info::display()))
        {


            qDebug("reparent window: %ld.", event->xany.window);
            WmFrame *newFrame = new WmFrame(event->xcreatewindow.window);
            this->m_skipList.append(newFrame->winId());
            //newFrame->show();

            this->m_frameList.insert(event->xany.window, newFrame);

            if (!newFrame->embedWin()) {
                delete newFrame;
                return false;
            }

            //newFrame->resize(500, 500);
            //newFrame->resizeEmbed(500, 500);
            //this->m_skipList.append(newFrame->winId());

            newFrame->show();
        }
*/
        break;

    case ConfigureNotify:
        qDebug("ConfigureNotify: %ld = %d, %d", event->xconfigure.window, event->xconfigure.width, event->xconfigure.height);
        f = this->m_frameList.value(event->xany.window);
        if (f == NULL) {
            return false;
        }
        qDebug("configurenotify resize");
        f->resizeEmbed(event->xconfigure.width, event->xconfigure.height);
        XResizeWindow(QX11Info::display(), event->xany.window, event->xconfigure.width, event->xconfigure.height);
        break;

    case CreateNotify:
        qDebug("CreateNotify : %ld.", event->xcreatewindow.window);

        break;

    case DestroyNotify:
        qDebug("DestoryNotify: %ld", event->xdestroywindow.window);
        if (!this->m_skipList.contains(event->xdestroywindow.window)) {
            if (this->m_frameList.contains(event->xcreatewindow.window)) {
                WmFrame *frame = this->m_frameList.value(event->xcreatewindow.window);
                frame->unEmbedWin();
                //this->m_skipList.removeOne(frame->winId());
                delete frame;
                this->m_frameList.remove(event->xcreatewindow.window);

            }
        }
        break;

    case UnmapNotify:
        qDebug("UnmapNotify: %ld.", event->xunmap.window);

        break;
    }

    return false;
}
