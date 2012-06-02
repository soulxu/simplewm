#include "WmFrame.h"

#include <QX11Info>
#include <QVBoxLayout>

#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <QPushButton>

WmFrame::WmFrame(Window win, QWidget *widget) :
        QWidget(widget),
        m_layout(new QVBoxLayout()),
        m_win(win)
{
    qDebug("wmframe: %x", this->winId());
    Atom atom_wmframe = XInternAtom(QX11Info::display(), "wmframe", FALSE);
    long val = 1;
    XChangeProperty(QX11Info::display(), this->winId(), atom_wmframe, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&val, 1);

    QPushButton *but = new QPushButton("a", this);
    this->connect(but, SIGNAL(clicked()), this, SLOT(clicked()));
    this->setLayout(m_layout);

    QPalette palette = this->m_subWin.palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    //palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0)));
    this->m_subWin.setPalette(palette);

    this->m_layout->addWidget(&(this->m_title));
    this->m_layout->addWidget(but);
    this->m_layout->addWidget(&(this->m_subWin));
    this->m_layout->setStretch(1, 1);
    this->m_title.setFixedHeight(5);

    emit test();
}

void WmFrame::clicked() {
    this->move(this->x() + 10, this->y());
}

WmFrame::~WmFrame() {

}

void WmFrame::resizeEmbed(int x, int y) {
    //this->m_subWin.resize(x, y);
    //this->m_subWin.setFixedSize(x, y);
}

bool WmFrame::embedWin() {
    const QX11Info &x11Info = this->x11Info();

    XWindowAttributes attr;

    XGetWindowAttributes(QX11Info::display(), m_win, &attr);



    this->m_subWin.setFixedSize(500, 500);
    XReparentWindow(x11Info.display(), m_win, this->m_subWin.winId(), 0, 0);

    XResizeWindow(QX11Info::display(), m_win, 499, 499);
    XSelectInput(x11Info.display(), m_win, StructureNotifyMask);
    XMapWindow(QX11Info::display(), m_win);
    qDebug("embedWin %d, %d", attr.width,attr.height);




    return true;
}

void WmFrame::unEmbedWin() {
    const QX11Info &x11Info = this->x11Info();
    XReparentWindow(x11Info.display(), m_win, RootWindow(x11Info.display(), x11Info.appScreen()), 0, 0);
}


void WmFrame::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
}

bool WmFrame::x11Event(XEvent *event) {
    switch (event->type) {
    case UnmapNotify:
        qDebug("unmap notify.");
        if (event->xunmap.window == this->m_win) {
            this->unEmbedWin();
            delete this;
        }
        break;
    }

    return QWidget::x11Event(event);
}

