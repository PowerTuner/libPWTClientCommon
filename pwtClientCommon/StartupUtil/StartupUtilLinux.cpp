/*
 * This file is part of PWTClientCommon.
 * Copyright (C) 2025 kylon
 *
 * PWTClientCommon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PWTClientCommon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QStandardPaths>
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>

#include "StartupUtilLinux.h"

namespace PWT {
    StartupUtilLinux::StartupUtilLinux(const QString &name) {
        desktopPath = QString("%1/.config/autostart/%2.desktop").arg(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), name);
        appName = name;
    }

    bool StartupUtilLinux::setup() const {
        QFile desktop {desktopPath};
        QTextStream ts {&desktop};

        if (!desktop.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
            return false;

        ts << "[Desktop Entry]\n" <<
            "Categories=System\n" <<
            QString("Name=%1\n").arg(appName) <<
            "StartupNotify=false\n" <<
            "Terminal=false\n" <<
            "Type=Application\n" <<
            QString("Icon=/usr/icon/hicolor/%1.ico\n").arg(appName.contains("console") ? "pwtc" : "pwt") <<
            QString("Exec=%1 -s\n").arg(QCoreApplication::applicationFilePath());

        desktop.close();
        return true;
    }

    bool StartupUtilLinux::unset() const {
        QFile desktop {desktopPath};

        return !desktop.exists() || desktop.remove();
    }

    bool StartupUtilLinux::isSet() const {
        return QFile(desktopPath).exists();
    }
}
