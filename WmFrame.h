#ifndef WMFRAME_H
#define WMFRAME_H

#include <QWidget>

#include <X11/X.h>

class QVBoxLayout;

class WmFrame : public QWidget {
    Q_OBJECT
public:
    WmFrame(Window win, QWidget *widget = NULL);
    virtual ~WmFrame();
    void resizeEmbed(int x, int y);
    bool embedWin();
    void unEmbedWin();

private slots:
    void clicked();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual bool x11Event(XEvent *event);

signals:
    void test();

private:
    QVBoxLayout *m_layout;
    QWidget m_title;
    QWidget m_subWin;
    Window m_win;
};

#endif // WMFRAME_H
