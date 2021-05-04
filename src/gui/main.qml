/************************************************************************
 * This file is part of the minilockcpp distribution
 * (https://github.com/mrom1/minilockcpp).
 * Copyright (c) 2021 mrom1.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/



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

