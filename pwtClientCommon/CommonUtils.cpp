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
#include <QFile>
#include <QTextStream>

#include "CommonUtils.h"
#include "../version.h"
#ifdef __linux__
#include "StartupUtil/StartupUtilLinux.h"
#elif defined(_WIN32)
#include "StartupUtil/StartupUtilWindows.h"
#endif

namespace PWT {
    int getCommonMajorVersion() {
        return CLCOMMON_VER_MAJOR;
    }

    int getCommonMinorVersion() {
        return CLCOMMON_VER_MINOR;
    }

    [[nodiscard]]
    static QString getGPUIDsDB(const PWTS::GPUVendor vendor) {
        switch (vendor) {
            case PWTS::GPUVendor::AMD:
                return ":/gpu/gpuIDs/amdgpu.ids";
            case PWTS::GPUVendor::Intel:
                return ":/gpu/gpuIDs/intelgpu.ids";
            case PWTS::GPUVendor::NVIDIA:
                return ":/gpu/gpuIDs/nvidiagpu.ids";
            default:
                break;
        }

        return "";
    }

    QString getGPUMarketName(const PWTS::GPUVendor vendor, const QString &deviceID, const QString &revisionID) {
        if (deviceID.isEmpty() || revisionID.isEmpty())
            return "";

        const QString idStr = QString("%1,\t%2").arg(deviceID, revisionID);
        QFile ids {getGPUIDsDB(vendor)};
        QTextStream ts(&ids);
        QString tsLine;

        if (!ids.open(QFile::ReadOnly | QFile::Text))
            return "";

        while (ts.readLineInto(&tsLine)) {
            if (tsLine.startsWith(idStr, Qt::CaseInsensitive))
                return tsLine.split(u',').at(2).trimmed();
        }

        return "";
    }

    std::unique_ptr<StartupUtil> getStartupUtil(const QString &name) {
#ifdef __linux__
        return std::make_unique<StartupUtilLinux>(name);
#elif defined(_WIN32)
        return std::make_unique<StartupUtilWindows>(name);
#endif
    }
}
