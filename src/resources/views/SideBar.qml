import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: "transparent"
    width: 280
    clip: true
    property bool expanded: true
    state: expanded? "Expanded":"Folded"

    PushButton {
        id: titleButton
        height: 32
        anchors.left: parent.left
        anchors.right: parent.right
        noOverlay: true
        anchors.top: parent.top
        showIcon: true
        icon: "qrc:/assets/logo.svg"
        text: qsTr("Bit Wave")
        flat: true
        enabled: false
    }

    SearchBox {
        id: globalSearchBox
        height: 28
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleButton.bottom
        anchors.margins: 12
        placeholder: qsTr("How are you today?")
    }

    ActiveTab {
        id: playerPageButton
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: globalSearchBox.bottom
        anchors.topMargin: 12
        flat: true
        border.color: "transparent"
        showIcon: true
        icon: "qrc:/assets/play-large.svg"
        text: qsTr("Player Page")
        isTabActive: display.activeTabIndex === -1
        onClicked: {
            display.activeTabIndex = -1

            // test area
            // isTabActive = !isTabActive
            player.playUrl("file:///home/reverier/Music/musics/01. Prover.flac");
            player.currentMediaTitle = "Prover";
            player.currentMediaCover = "file:///home/reverier/Music/covers/Tell me.jpg";
            player.currentMediaArtist = "milet";
            player.currentMediaAlbum = "Prover/Tell Me";
            player.setLyrics("[00:00.000] 作词 : milet\n[00:00.745] 作曲 : TomoLow/milet\n[00:01.490]\n[00:06.490]踏み込んだshadowland 楽園なんてない\n[00:11.490]錆びる old remedies 確信なんてない\n[00:17.490]それでも go up the river\n[00:20.490]しがみつくだけのladder\n[00:23.490]涙にさえ灯る日が\n[00:26.490]ひとつのsign ひとつのlight\n[00:30.490]明けない夜も 歌が途切れないように\n[00:36.490]当てなく迷う夢が縺れないように\n[00:42.490]すれ違った足跡達 振り返らぬように\n[00:48.490]踏み外したあなたでさえ 手放さないように\n[00:54.490]I'm the prover I am the prover\n[01:00.490]終らない世界にだって立ち向かうように\n[01:07.490]I'm the prover\n[01:09.490]命の声がまた響きだす頃に\n[01:16.490]あなたと沈まずに行く船を\n[01:27.490]降り出したiron rain\n[01:32.490]ゼロにするためのpray\n[01:34.490]同じ孤独見てたら\n[01:38.490]あなたなら分かるでしょう\n[01:40.490]それでも go up the river\n[01:43.490]いつかまた出逢えるなら\n[01:46.490]この愛に灯る日が\n[01:50.490]ひとつのsign ひとつのlight\n[01:53.490]明けない夜も 歌が途切れないように\n[01:59.490]当てなく迷う夢が縺れないように\n[02:05.490]すれ違った足跡たち 振り返らぬように\n[02:11.490]踏み外したあなたでさえ 手放さないように\n[02:18.490]If you can't find your way\n[02:22.490]I will never abandon you\n[02:25.490]Everything is not lost\n[02:29.490]'Cause I'm standing with you\n[02:31.490]二度と戻りはしない世界だとして\n[02:37.490]I'll live for you\n[03:11.490]I'm the prover I am the prover\n[03:18.490]終らない世界にだって立ち向かうように\n[03:25.490]I am the prover\n[03:28.490]命の声がまた響きだす頃にあなたと\n[03:35.490]I'll be there\n[03:38.490]沈まずに行く船を\n[03:43.490]何処までも行く船を\n", "[by:陳四月一日]\n[00:06.490]深陷的虚境中 不存在什么乐园\n[00:11.490]老套的补救方法 没有任何确信\n[00:17.490]即使如此 只能溯源而上\n[00:20.490]只有紧紧抓住梯子\n[00:23.490]连泪水也如灯光的日子\n[00:26.490]仿佛一个信号 一寸光芒\n[00:30.490]愿这歌声能在无尽的黑夜里源远流长\n[00:36.490]愿彷徨流浪的迷梦 不再纠葛缠绵\n[00:42.490]愿擦肩而过的那些足迹不会回首相望\n[00:48.490]愿你即使行差踏错 我的手也不放\n[00:54.490]我会证明 我是那见证者\n[01:00.490]即使世界没有尽头 我也会锐不可当\n[01:07.490]我会证明\n[01:09.490]当生命之声再次响彻天际\n[01:16.490]我便与你共乘在那永不沉没的船上\n[01:27.490]铁腥之雨从天而降\n[01:32.490]为坚持到底而祈祷\n[01:34.490]若你能看到同样的孤独\n[01:38.490]一定会理解的吧\n[01:40.490]即使如此 只能溯源而上\n[01:43.490]若有一天能再次邂逅你\n[01:46.490]为爱点亮灯光的那一天\n[01:50.490]会是一个信号 一道光芒\n[01:53.490]愿这歌声能在无尽的黑夜里源远流长\n[01:59.490]愿彷徨流浪的迷梦 不再纠葛缠绵\n[02:05.490]愿擦肩而过的那些足迹不会回首相望\n[02:11.490]愿你即使行差踏错 我的手也不放\n[02:18.490]就算你迷失方向\n[02:22.490]我也会对你不离不弃\n[02:25.490]你不会失去一切\n[02:29.490]因为我依旧在你身边\n[02:31.490]即使再也回不到这个世界\n[02:37.490]我也会为你而活\n[03:11.490]我会证明 我是那见证者\n[03:18.490]即使世界没有尽头 我也会锐不可当\n[03:25.490]我会证明\n[03:28.490]当生命之声再次响彻天际\n[03:35.490]我会站在那里\n[03:38.490]与你共乘在那永不沉没的船上\n[03:43.490]能去往任何地方\n");
        }
    }

    ListView {
        id: libraryList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: playerPageButton.bottom
        anchors.topMargin: 12
        anchors.bottom: settingsButton.top
        anchors.bottomMargin: 12

        delegate: ActiveTab {
            width: ListView.view.width
            height: 36
            anchors.topMargin: 3
            flat: true
            border.color: "transparent"
            showIcon: true
            icon: "qrc:/assets/music.svg"
            isTabActive: index === libraryList.currentIndex
        }
    }

    ActiveTab {
        id: settingsButton
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        flat: true
        border.color: "transparent"
        showIcon: true
        icon: "qrc:/assets/settings.svg"
        text: qsTr("Settings")
        isTabActive: display.activeTabIndex === -2
        onClicked: {
            display.activeTabIndex = -2
        }
    }

    states: [
        State {
            name: "Expanded"
            PropertyChanges {
                target: root
                width: 280
            }
        },
        State {
            name: "Folded"
            PropertyChanges {
                target: root
                width: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "Expanded"
            NumberAnimation {
                target: root
                property: "width"
                duration: 300
                easing.type: Easing.OutExpo
            }
        },
        Transition {
            from: "*"; to: "Folded"
            NumberAnimation {
                target: root
                property: "width"
                duration: 300
                easing.type: Easing.OutExpo
            }
        }
    ]
}
