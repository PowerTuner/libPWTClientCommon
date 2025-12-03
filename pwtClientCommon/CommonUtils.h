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
#pragma once

#include <QString>
#include <memory>

#include "StartupUtil/StartupUtil.h"
#include "pwtShared/Include/GPU/GPUVendor.h"

namespace PWT {
    [[nodiscard]] PWTCLIENTCOMMON_EXPORT constexpr int getCommonMajorVersion();
    [[nodiscard]] PWTCLIENTCOMMON_EXPORT constexpr int getCommonMinorVersion();
    [[nodiscard]] PWTCLIENTCOMMON_EXPORT QString getGPUMarketName(PWTS::GPUVendor vendor, const QString &deviceID, const QString &revisionID);
    [[nodiscard]] PWTCLIENTCOMMON_EXPORT std::unique_ptr<StartupUtil> getStartupUtil(const QString &name);
}
