//
// Created by Reverier-Xu on 2021/6/27.
//

#ifndef BITWAVE_VIDEO_PLAYER_H
#define BITWAVE_VIDEO_PLAYER_H


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

protected:

    void mouseMoveEvent(QMouseEvent *event) override;

public slots:

    void doUpdate();

signals:

    void mouseMoved();

    void onUpdate();

    void videoPlayingChanged(bool m);

    void styleChanged(bool m);

private:

    mpv_handle *mpv;

    mpv_render_context *mpv_gl;

    friend class MpvRenderer;

};


#endif //BITWAVE_VIDEO_PLAYER_H
