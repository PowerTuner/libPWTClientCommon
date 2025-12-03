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
#include "pwtWin32/win32Reg.h"
#include <QCoreApplication>

#include "StartupUtilWindows.h"
#include "../UILogger.h"

namespace PWT {
    StartupUtilWindows::StartupUtilWindows(const QString &name) {
        appName = name.toStdWString();
    }

    bool StartupUtilWindows::setup() const {
        const QString appPath = QString(R"("%1 -s")").arg(QCoreApplication::applicationFilePath().replace('/', '\\'));
        QString errStr;
        const bool res = PWTW32::regWriteSZ(HKEY_CURRENT_USER, runPath, appName.c_str(), REG_SZ, appPath, errStr);

        if (!res)
            UI::UILogger::getInstance()->writeLog(errStr);

        return res;
    }

    bool StartupUtilWindows::unset() const {
        QString errStr;
        const bool res = PWTW32::regDeleteVal(HKEY_CURRENT_USER, runPath, appName.c_str(), errStr);

        if (!res)
            UI::UILogger::getInstance()->writeLog(errStr);

        return res;
    }

    bool StartupUtilWindows::isSet() const {
        bool result = false;
        QString errStr;

        if (!PWTW32::regKeyHasValue(HKEY_CURRENT_USER, runPath, appName.c_str(), result, errStr))
            UI::UILogger::getInstance()->writeLog(errStr);

        return result;
    }
}
