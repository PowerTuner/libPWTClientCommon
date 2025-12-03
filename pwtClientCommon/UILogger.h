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

#include <QObject>
#include <QSharedPointer>
#include <source_location>

#include "Include/clientcommonExport.h"

namespace PWT::UI {
    class PWTCLIENTCOMMON_EXPORT UILogger final: public QObject {
        Q_OBJECT

    private:
        static inline QSharedPointer<UILogger> instance;

        UILogger() = default;

    public:
        UILogger(UILogger &) = delete;

        static QSharedPointer<UILogger> getInstance();
        void writeLog(const QString &msg, std::source_location source = std::source_location::current());

    signals:
        void logWritten(const QString &msg);
    };
}
