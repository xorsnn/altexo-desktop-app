#ifndef THREADRENDERER_H
#define THREADRENDERER_H

#include <QQuickItem>
class RenderThread;

class ThreadRenderer : public QQuickItem
{
    Q_OBJECT
public:
    ThreadRenderer();

    static QList<QThread *> threads;

public Q_SLOTS:
    void ready();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    RenderThread *m_renderThread;

};

#endif // THREADRENDERER_H
