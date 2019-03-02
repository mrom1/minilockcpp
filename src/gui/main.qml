import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: "Minilock++"

    PageLogin {
        id: pageLogin
    }

    PageMinilock {
        id: pageMinilock
    }

    Item {
        id: fsm
        state: "PageLogin"
        states: [
            State {
                name: "PageLogin"
                PropertyChanges {target: pageLogin; visible:true }
                PropertyChanges { target: pageMinilock; visible:false }
            },
            State {
                name: "PageMinilock"
                PropertyChanges {target: pageMinilock; visible:true }
                PropertyChanges {target: pageLogin; visible:false }

            }
        ]
    }
}

