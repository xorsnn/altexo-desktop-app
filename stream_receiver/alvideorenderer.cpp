#include "alvideorenderer.h"
#include <QDebug>
//#include <QtOpenGL/QGLWidget>

AlVideoRenderer::AlVideoRenderer()
{
    this->img.load("/home/xors/ss0.bmp");
//    this->img = QGLWidget::convertToGLFormat(this->img);
//    this->tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
//    this->tex->setSize(this->img.width(), this->img.height());
//    this->tex->setData(this->img.mirrored());
//    this->img.save("/home/xors/ss0.png");
//    this->tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//    this->tex->setMagnificationFilter(QOpenGLTexture::Linear);
}

AlVideoRenderer::~AlVideoRenderer()
{

}

void AlVideoRenderer::initialize()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &gl_depth_tex);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &gl_rgb_tex);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glViewport(0,0,1280, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, 1280, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
//

    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->img.width(), this->img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, this->img.mirrored().bits());

//    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
//    glEnable(GL_BLEND);
//    QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &program1);
//    const char *vsrc1 =
//        "attribute highp vec4 vertex;\n"
//        "attribute mediump vec3 normal;\n"
//        "uniform mediump mat4 matrix;\n"
//        "varying mediump vec4 color;\n"
//        "void main(void)\n"
//        "{\n"
//        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
//        "    float angle = max(dot(normal, toLight), 0.0);\n"
//        "    vec3 col = vec3(0.40, 1.0, 0.0);\n"
//        "    color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);\n"
//        "    color = clamp(color, 0.0, 1.0);\n"
//        "    gl_Position = matrix * vertex;\n"
//        "}\n";
//    vshader1->compileSourceCode(vsrc1);

//    QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &program1);
//    const char *fsrc1 =
//        "varying mediump vec4 color;\n"
//        "void main(void)\n"
//        "{\n"
//        "    gl_FragColor = color;\n"
//        "}\n";
//    fshader1->compileSourceCode(fsrc1);

//    program1.addShader(vshader1);
//    program1.addShader(fshader1);
//    program1.link();

//    vertexAttr1 = program1.attributeLocation("vertex");
//    normalAttr1 = program1.attributeLocation("normal");
//    matrixUniform1 = program1.uniformLocation("matrix");

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

//    m_fAngle = 0;
//    m_fScale = 1;

////    createGeometry();
}

void AlVideoRenderer::render()
{
//    static std::vector<uint8_t> depth(640*480*4);
//    static std::vector<uint8_t> rgb(640*480*4);

//    // using getTiltDegs() in a closed loop is unstable
//    /*if(device->getState().m_code == TILT_STATUS_STOPPED){
//        freenect_angle = device->getState().getTiltDegs();
//    }*/
//    device->updateState();
//    printf("\r demanded tilt angle: %+4.2f device tilt angle: %+4.2f", freenect_angle, device->getState().getTiltDegs());
//    fflush(stdout);

//    device->getDepth(depth);
//    device->getRGB(rgb);

//    got_frames = 0;

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();

//    glEnable(GL_TEXTURE_2D);

//    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);

//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(0,0,0);
//    glTexCoord2f(1, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 1); glVertex3f(640,480,0);
//    glTexCoord2f(0, 1); glVertex3f(0,480,0);
//    glEnd();

//    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
//    if (device->getVideoFormat() == FREENECT_VIDEO_RGB || device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB)
//        glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
//    else
//        glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0]);

//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
//    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
//    glTexCoord2f(0, 1); glVertex3f(640,480,0);
//    glEnd();

//    glutSwapBuffers();
//    qDebug() << "<< 2";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

//    glDepthMask(true);

//    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

//    glFrontFace(GL_CW);
//    glCullFace(GL_FRONT);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);

//    glEnable(GL_TEXTURE_2D);

//    tex->setData(this->img); //first call.ok
//    this->tex->setData(this->img.mirrored());
//    this->tex->bind();

//    this->img.load("/home/xors/ss0.bmp");
//    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->img.width(), this->img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, this->img.mirrored().bits());

//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 1); glVertex3f(640,480,0);
    glTexCoord2f(0, 1); glVertex3f(0,480,0);
    glEnd();
//    this->tex->setData(this->img);

//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(0,0,0);
//    glTexCoord2f(1, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 1); glVertex3f(640,480,0);
//    glTexCoord2f(0, 1); glVertex3f(0,480,0);
//    glEnd();
//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 11.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(0,0,0);
//    glTexCoord2f(1, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 1); glVertex3f(640,480,0);
//    glTexCoord2f(0, 1); glVertex3f(0,480,0);
//    glEnd();



//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
//    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
//    glTexCoord2f(0, 1); glVertex3f(640,480,0);
//    glEnd();

//    QMatrix4x4 modelview;
//    modelview.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
//    modelview.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
//    modelview.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
//    modelview.scale(m_fScale);
//    modelview.translate(0.0f, -0.2f, 0.0f);

//    program1.bind();
//    program1.setUniformValue(matrixUniform1, modelview);
//    paintQtLogo();
//    program1.release();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

//    m_fAngle += 0.1f;
}

