import QtQuick 2.15

Item {

    // ----- Public Properties ----- //
    id: root
    width: radius * 2
    height: radius * 2

    property int radius: 25
    property bool running: false

    property color color: display.contentColor

    // ----- Private Properties ----- //

    property int _innerRadius: radius


    Repeater {
        id: repeater
        model: 4
        delegate: Component {
            Rectangle {
                // ----- Private Properties ----- //
                property int _currentAngle: _getStartAngle()

                id: rect
                width: root.width / 10
                height: width
                radius: width / 2
                color: root.color
                transformOrigin: Item.Center
                x: root._getPosOnCircle(_currentAngle).x - width / 2
                y: root._getPosOnCircle(_currentAngle).y - width / 2
                antialiasing: true

                NumberAnimation {
                    id: anim
                    target: rect
                    property: "_currentAngle"
                    duration: 1000
                    from: rect._getStartAngle()
                    to: 360 + rect._getStartAngle()
                    easing.type: Easing.OutQuad
                }

                // ----- Public Properties ----- //

                function playAnimation() {
                    if (anim.running === false) {
                        anim.start();
                    } else {
                        anim.resume();
                    }
                }

                function stopAnimation() {
                    anim.stop();
                }

                // ----- Private Functions ----- //

                function _getStartAngle() {
                    var ang = 90;

                    return ang;
                }
            }
        }
    }

    Timer {
        // ----- Private Properties ----- //
        id: timer
        interval: 1300
        triggeredOnStart: true
        property int _circleIndex: 0
        repeat: true
        running: root.running
        onTriggered: {
            emitTimer.start();
        }
    }

    Timer {
        id: emitTimer
        interval: 100
        triggeredOnStart: true
        property int _circleIndex: 0
        repeat: true
        onTriggered: {
            var maxIndex = repeater.model;
            if (_circleIndex === maxIndex) {
                _circleIndex = 0;
                emitTimer.stop();
            } else {
                // console.log("_circleIndex: " + _circleIndex);
                repeater.itemAt(_circleIndex).playAnimation();
                _circleIndex++;
            }
        }
    }

    // ----- Private Functions ----- //

    function _toRadian(degree) {
        return (degree * 3.14159265) / 180.0;
    }

    function _getPosOnCircle(angleInDegree) {
        var centerX = root.width / 2, centerY = root.height / 2;
        var posX = 0, posY = 0;

        posX = centerX + root._innerRadius * Math.cos(_toRadian(angleInDegree));
        posY = centerY - root._innerRadius * Math.sin(_toRadian(angleInDegree));
        return Qt.point(posX, posY);
    }
}