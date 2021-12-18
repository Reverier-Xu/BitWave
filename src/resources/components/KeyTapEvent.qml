import QtQuick 2.15
 
Item {
    id: keyItem
    property var customKey: "Space"
    property int isPressed: 0
    anchors.fill: parent
    focus: true

    signal doubleClicked();
    signal clicked();
 
    Timer {
        id: keyTime
        interval: 200
        onTriggered: {
            keyItem.isPressed = 0
        }
    }
 
    Shortcut {
        sequence: customKey
        autoRepeat: false
        context: Qt.ApplicationShortcut
         onActivated: {
            // console.log(customKey+" activate")
            dClick()
        }
    }
 
    function dClick(){
        if(keyTime.running){
            if (1 === keyItem.isPressed) {
                keyItem.isPressed = 0;
                keyItem.doubleClicked();
                keyTime.stop();
            }
        } else {
            keyItem.isPressed = 1;
            keyItem.clicked();
            keyTime.start();
        }
    }
}