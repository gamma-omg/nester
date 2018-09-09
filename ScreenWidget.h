#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <cstdint>
#include <qopengl.h>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <ppu/IRenderCallback.h>

class QOpenGLShaderProgram;
class QOpenGLTexture;

class ScreenWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions,
                     public nescore::IRenderCallback
{
private:
    static const GLfloat SCREEN[];

public:
    explicit ScreenWidget(QWidget* parent);
    ~ScreenWidget() override;

    void renderFrame(int width, int height, const uint32_t* frameBuffer) override;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    void createShaderProgram();
    void createVertexBuffer();

private:
    QOpenGLShaderProgram* _program;
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;
    QMatrix4x4 _mvp;
    QOpenGLTexture* _texture;

    int _uniformMvp;
    int _uniformTexture;

};

#endif // RENDERWIDGET_H
