//
// Created by Reverier-Xu on 2021/6/27.
//

#pragma once

#include <QtWidgets/QOpenGLWidget>
#include <utility>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <QtQuick/QQuickFramebufferObject>
#include "utilities/mpv_helper.h"

class MpvRenderer;

class VideoPlayer : public QQuickFramebufferObject {
Q_OBJECT

public:

    static void on_update(void *ctx);

    explicit VideoPlayer(QQuickItem *parent = nullptr);

    ~VideoPlayer() override;

    [[nodiscard]] Renderer *createRenderer() const override;

public slots:

    void doUpdate();

signals:

    void onUpdate();

private:

    mpv_handle *mpv;

    mpv_render_context *mpv_gl;

    friend class MpvRenderer;

};
