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

#include "StartupUtil.h"

namespace PWT {
    class PWTCLIENTCOMMON_EXPORT StartupUtilLinux final: public StartupUtil {
    private:
        QString desktopPath;
        QString appName;

    public:
        explicit StartupUtilLinux(const QString &name);

        [[nodiscard]] bool setup() const override;
        [[nodiscard]] bool unset() const override;
        [[nodiscard]] bool isSet() const override;
    };
}
