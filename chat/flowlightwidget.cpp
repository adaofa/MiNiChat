#include "flowlightwidget.h"
#include <QOpenGLShader>
#include <QFile>

FlowLightWidget::FlowLightWidget(QWidget *parent)
    : QOpenGLWidget(parent), shaderProgram(nullptr)
{
    connect(&timer_, &QTimer::timeout, this, QOverload<>::of(&FlowLightWidget::update));
    timer_.start(16); // 大约60fps
    time_.start();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

FlowLightWidget::~FlowLightWidget()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    delete shaderProgram;
    doneCurrent();
}

void FlowLightWidget::initializeGL()
{
    initializeOpenGLFunctions();

    float vertices[] = {
        -1, -1,
        1, -1,
        -1,  1,
        1, -1,
        1,  1,
        -1,  1
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    shaderProgram = new QOpenGLShaderProgram;
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/flowlight.vert");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/flowlight.frag");
    shaderProgram->link();
}

void FlowLightWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    // 投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 正交投影
    glOrtho(0, w, h, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void FlowLightWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();

    float currentTime = time_.elapsed() / 1000.0f;
    shaderProgram->setUniformValue("iTime", currentTime);
    QSize size = this->size();
    shaderProgram->setUniformValue("iResolution", QVector2D(size.width(), size.height()));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //qDebug() << "Current size:" << this->size();

    shaderProgram->release();
}
