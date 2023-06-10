import QtQuick
import QtQuick.Templates as T

T.StackView {
    id: control

    popEnter: Transition {
        YAnimator {
            duration: 400
            easing.type: Easing.OutExpo
            from: control.height
            to: 0
        }
    }
    popExit: Transition {
        YAnimator {
            duration: 400
            easing.type: Easing.OutExpo
            from: control.height
            to: 0
        }
    }
    pushEnter: Transition {
        YAnimator {
            duration: 400
            easing.type: Easing.OutExpo
            from: 0
            to: -control.height
        }
    }
    pushExit: Transition {
        YAnimator {
            duration: 400
            easing.type: Easing.OutExpo
            from: 0
            to: -control.height
        }
    }
}
