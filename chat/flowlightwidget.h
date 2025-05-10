#ifndef FLOWLIGHTWIDGET_H
#define FLOWLIGHTWIDGET_H

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLFunctions_4_1_Core>

class FlowLightWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

public:
    FlowLightWidget(QWidget *parent = nullptr);
    ~FlowLightWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram *shaderProgram;
    QTimer timer_;
    QElapsedTimer  time_;
    GLuint vao, vbo;
};

#endif // FLOWLIGHTWIDGET_H
