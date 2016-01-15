#ifndef WIDGETSTREAMER_H
#define WIDGETSTREAMER_H

#include <QObject>
#include "alconductor/alconductor.h"

class WidgetStreamer : public QObject
{
    Q_OBJECT
public:
    WidgetStreamer(QWidget *pRenderWidget, rtc::scoped_refptr<AlConductor> pStreamWidget);
    ~WidgetStreamer();
private Q_SLOTS:
    void onTimeout();
public Q_SLOTS:
    void startSlot();
private:
    QWidget *m_pRenderWidget;
    rtc::scoped_refptr<AlConductor> m_pStreamWidget;

};

#endif // WIDGETSTREAMER_H
