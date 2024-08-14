import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import BufferIssueDemo 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Connections {
        target: playButton
        onClicked: audioPlayer.play()
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#2d2d2d"
    }

    AudioPlayer {
        id: audioPlayer
        audioFile: AudioFile {
            id: audioFile
        }
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        onAccepted: audioFile.file = fileDialog.selectedFile.toString().replace("file://", "")
    }

    ColumnLayout {
        id: layout
        anchors.centerIn: parent
        spacing: 10

        Label {
            id: fileDisplay
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: audioFile.file === "" ? "No file selected" : audioFile.file
        }

        Button {
            id: openDialogButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: "Open File Dialog"
            onClicked: fileDialog.open()
        }

        Button {
            id: playButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: "Play"
        }

    }
}
