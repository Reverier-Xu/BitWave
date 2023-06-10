/**
 * @file video_player.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright 2023 Woo Tech
 */

#include "video_player.h"

#include <QApplication>
#include <QEvent>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QQuickWindow>
#include <QMetaObject>
#include <QOpenGLContext>
#include <QPainterPath>
#include <stdexcept>
#include <QQuickOpenGLUtils>

#include "player.h"


void on_mpv_redraw(void* ctx) { VideoPlayer::on_update(ctx); }

static void* get_proc_address_mpv(void* ctx, const char* name) {
    Q_UNUSED(ctx)

    QOpenGLContext* gl_ctx = QOpenGLContext::currentContext();
    if (!gl_ctx) return nullptr;

    return reinterpret_cast<void*>(gl_ctx->getProcAddress(QByteArray(name)));
}

class MpvRenderer : public QQuickFramebufferObject::Renderer {
    VideoPlayer* obj;

   public:
    explicit MpvRenderer(VideoPlayer* new_obj) : obj{new_obj} { }

    ~MpvRenderer() override = default;

    // This function is called when a new FBO is needed.
    // This happens on the initial frame.
    QOpenGLFramebufferObject* createFramebufferObject(
        const QSize& size) override {
        // init mpv_gl:
        if (!obj->mpv_gl) {
            mpv_opengl_init_params gl_init_params{get_proc_address_mpv, nullptr};
            mpv_render_param params[]{
                {MPV_RENDER_PARAM_API_TYPE,
                 const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL)},
                {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
                {MPV_RENDER_PARAM_INVALID, nullptr}};

            if (mpv_render_context_create(&obj->mpv_gl, obj->mpv, params) < 0)
                throw std::runtime_error("failed to initialize mpv GL context");
            mpv_render_context_set_update_callback(obj->mpv_gl, on_mpv_redraw,
                                                   obj);
        }

        return QQuickFramebufferObject::Renderer::createFramebufferObject(size);
    }

    void render() override {
        QQuickOpenGLUtils::resetOpenGLState();

        QOpenGLFramebufferObject* fbo = framebufferObject();
        mpv_opengl_fbo mpv_fbo{
            .fbo = static_cast<int>(fbo->handle()),
            .w = fbo->width(),
            .h = fbo->height(),
            .internal_format = 0
        };
        int flip_y{0};

        mpv_render_param params[] = {
            // Specify the default framebuffer (0) as target. This will
            // render onto the entire screen. If you want to show the video
            // in a smaller rectangle or apply fancy transformations, you'll
            // need to render into a separate FBO and draw it manually.
            {MPV_RENDER_PARAM_OPENGL_FBO, &mpv_fbo},
            // Flip rendering (needed due to flipped GL coordinate system).
            {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
            {MPV_RENDER_PARAM_INVALID, nullptr}};
        // See render_gl.h on what OpenGL environment mpv expects, and
        // other API details.
        mpv_render_context_render(obj->mpv_gl, params);
    }
};

VideoPlayer::VideoPlayer(QQuickItem* parent)
    : QQuickFramebufferObject(parent), mpv_gl(nullptr) {
    // qDebug() << "VideoPlayer is initializing!";
    mpv = Player::instance()->engine()->getMpvHandle();
    connect(this, &VideoPlayer::onUpdate, this, &VideoPlayer::doUpdate,
            Qt::QueuedConnection);
}

VideoPlayer::~VideoPlayer() {
    if (mpv_gl)  // only initialized if something got drawn
    {
        mpv_render_context_free(mpv_gl);
    }

    mpv_terminate_destroy(mpv);
}

void VideoPlayer::on_update(void* ctx) {
    auto* self = (VideoPlayer*) ctx;
    emit self->onUpdate();
}

// connected to onUpdate(); signal makes sure it runs on the GUI thread
void VideoPlayer::doUpdate() { update(); }

QQuickFramebufferObject::Renderer* VideoPlayer::createRenderer() const {
    window()->setPersistentGraphics(true);
    window()->setPersistentSceneGraph(true);
    return new MpvRenderer(const_cast<VideoPlayer*>(this));
}
