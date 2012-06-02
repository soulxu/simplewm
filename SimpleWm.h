#ifndef SIMPLEWM_H
#define SIMPLEWM_H

#include <QApplication>
#include <QHash>

#include <X11/X.h>

class WmFrame;

class SimpleWm : public QApplication {
public:
    SimpleWm(int &argc, char **argv);
    virtual ~SimpleWm();

    bool initX11();
    virtual bool x11EventFilter(XEvent *event);

private:
    QHash<Window, WmFrame *> m_frameList;
    QList<Window> m_skipList;
};

#endif // SIMPLEWM_H
