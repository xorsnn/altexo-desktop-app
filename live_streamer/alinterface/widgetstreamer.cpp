#include "widgetstreamer.h"
#include <QTextEdit>
#include <QTimer>
#include <iostream>

using namespace std;

WidgetStreamer::WidgetStreamer(QWidget *pRenderWidget, rtc::scoped_refptr<AlConductor> pStreamWidget)
{
    m_pRenderWidget = pRenderWidget;
    m_pStreamWidget = pStreamWidget;


}

WidgetStreamer::~WidgetStreamer()
{
}

void WidgetStreamer::startSlot() {
    QTimer *pTimer = new QTimer(this);
    QObject::connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    pTimer->start(40);
}

void WidgetStreamer::onTimeout()
{
    int width = m_pRenderWidget->width();
    int height = m_pRenderWidget->height();

//    cerr << width << " " << height << endl;

    if (width <= 0 || height <= 0)
        return;

    QPixmap pixmap(width, height);

    m_pRenderWidget->render(&pixmap);
    QImage img = pixmap.toImage();

    m_pStreamWidget->slotSetNewFrame(img);
}
