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

#include <QFileInfo>

#include "UILogger.h"

namespace PWT::UI {
    QSharedPointer<UILogger> UILogger::getInstance() {
        if (instance.isNull())
            instance.reset(new UILogger);

        return instance;
    }

    void UILogger::writeLog(const QString &msg, const std::source_location source) {
        const QFileInfo srcInfo {source.file_name()};

        emit logWritten(QString("%1[%2:%3] %4").arg(srcInfo.fileName()).arg(source.line()).arg(source.column()).arg(msg));
    }
}
