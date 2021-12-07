/**
 * @file video_player.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <mpv/client.h>
#include <mpv/render_gl.h>

#include <QtQuick/QQuickFramebufferObject>
#include <QtWidgets/QOpenGLWidget>
#include <utility>

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
