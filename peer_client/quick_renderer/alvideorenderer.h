#ifndef ALVIDEORENDERER_H
#define ALVIDEORENDERER_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>

#include <QTime>
#include <QVector>
#include <QImage>
#include <QOpenGLTexture>

class AlVideoRenderer : protected QOpenGLFunctions
{
public:
    AlVideoRenderer();
    ~AlVideoRenderer();

    void render();
    void initialize();

private:

//    qreal   m_fAngle;
//    qreal   m_fScale;

//    QVector<QVector3D> vertices;
//    QVector<QVector3D> normals;
//    QOpenGLShaderProgram program1;
//    int vertexAttr1;
//    int normalAttr1;
//    int matrixUniform1;

//    DEBUG
    QImage img;
    QOpenGLTexture* tex;
    GLuint gl_depth_tex;
    GLuint gl_rgb_tex;

};

#endif // ALVIDEORENDERER_H
