#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLPixelTransferOptions>
#include "ScreenWidget.h"

const GLfloat ScreenWidget::SCREEN[] = { -1, -1, 0, 0, 1,
                                         -1, 1, 0, 0, 0,
                                         1, 1, 0, 1, 0,
                                         -1, -1, 0, 0, 1,
                                         1, 1, 0, 1, 0,
                                         1, -1, 0, 1, 1 };


ScreenWidget::ScreenWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , _program(nullptr)
    , _texture(nullptr)
    , _uniformMvp(0)
    , _uniformTexture(0)
{
}

ScreenWidget::~ScreenWidget()
{
    makeCurrent();

    _vbo.destroy();
    delete _texture;
    delete _program;

    doneCurrent();
}

void ScreenWidget::renderFrame(int width, int height, const uint32_t* frameBuffer)
{
    static const auto pxFormat = QOpenGLTexture::RGBA;
    static const auto pxType = QOpenGLTexture::UInt8;

    if (!_texture)
    {
        _texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        _texture->create();
        _texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        _texture->setSize(width, height);
        _texture->setFormat(QOpenGLTexture::RGBAFormat);
        _texture->allocateStorage(pxFormat, pxType);
    }

    _texture->setData(pxFormat, pxType, frameBuffer);
    update();
}

void ScreenWidget::initializeGL()
{
    initializeOpenGLFunctions();
    createShaderProgram();
    createVertexBuffer();

    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void ScreenWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    auto min = qMin<float>(width , height);
    auto h = width / min;
    auto v = height / min;
    _mvp.setToIdentity();
    _mvp.ortho(-h, h, -v, v, -100, 100);
}

void ScreenWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (!_texture)
    {
        return;
    }

    QOpenGLVertexArrayObject::Binder binder(&_vao);
    _program->bind();
    _program->setUniformValue(_uniformMvp, _mvp);
    _program->setUniformValue(_uniformTexture, 0);
    _texture->bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    _program->release();
}

void ScreenWidget::createShaderProgram()
{
    _program = new QOpenGLShaderProgram();
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ppu.vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ppu.frag");
    _program->bindAttributeLocation("vertex", 0);
    _program->bindAttributeLocation("texcoord", 1);
    _program->link();

    _program->bind();
    _uniformMvp = _program->uniformLocation("mvp");
    _uniformTexture = _program->uniformLocation("texture");
    _program->release();
}

void ScreenWidget::createVertexBuffer()
{
    _program->bind();

    _vao.create();
    QOpenGLVertexArrayObject::Binder binder(&_vao);

    _vbo.create();
    _vbo.bind();
    _vbo.allocate(SCREEN, sizeof(GLfloat) * 36);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    _vbo.release();
    _program->release();
}
