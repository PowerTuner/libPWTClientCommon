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
#include <QCryptographicHash>
#include <QFile>

#include "InputRanges.h"

namespace PWT::UI {
    QSharedPointer<InputRanges> InputRanges::getInstance() {
        if (instance.isNull())
            instance.reset(new InputRanges);

        return instance;
    }

    void InputRanges::setAppDataPath(const QString &appDataPath) {
        if (!appDataPath.isEmpty())
            path = QString("%1/inputRanges/").arg(appDataPath);
    }

    QByteArray InputRanges::getHash(const QString &path) const {
        QCryptographicHash crypto {QCryptographicHash::Sha256};
        QFile ranges {path};

        if (!ranges.open(QFile::ReadOnly))
            return {};

        if (crypto.addData(&ranges))
            return crypto.result();

        return {};
    }

    PWTS::MinMax InputRanges::getRange(const QString &key, const int minDefault, const int maxDefault) const {
        PWTS::MinMax range {.min = minDefault, .max = maxDefault};

        if (data.contains(key)) {
            const QJsonObject obj = data[key].toObject();

            if (obj.contains("min"))
                range.min = obj["min"].toInt(minDefault);

            if (obj.contains("max"))
                range.max = obj["max"].toInt(maxDefault);
        }

        if (overrideData.contains(key)) {
            const QJsonObject obj = overrideData[key].toObject();

            if (obj.contains("min"))
                range.min = obj["min"].toInt(minDefault);

            if (obj.contains("max"))
                range.max = obj["max"].toInt(maxDefault);
        }

        return range;
    }

    bool InputRanges::readJson(const QString &jsonPath, QJsonDocument &json) {
        QJsonParseError jerr;
        QFile jsonF {jsonPath};

        if (!jsonF.exists())
            return false;

        if (!jsonF.open(QFile::Text | QFile::ReadOnly)) {
            emit logMessageSent(QString("InputRanges: failed to open %1").arg(path));
            return false;
        }

        json = QJsonDocument::fromJson(jsonF.readAll(), &jerr);
        if (jerr.error != QJsonParseError::NoError) {
            emit logMessageSent(QString("InputRanges: failed to parse %1").arg(path));
            return false;
        }

        return true;
    }

    void InputRanges::tryLoadOverride() {
        if (path.isEmpty())
            return;

        const QString ovrdPath = QString("%1%2.json").arg(path, dmiProd);
        QJsonDocument json;

        if (!overrideData.isEmpty()) {
            if (!QFile::exists(ovrdPath)) {
                overrideData = QJsonObject();
                return;

            } else if (overrideHash == getHash(ovrdPath)) {
                return;
            }
        }

        if (!readJson(ovrdPath, json))
            return;

        for (const QString &cpuStr: json.object().keys()) {
            if (!cpuBrand.contains(cpuStr) || cpuStr.startsWith('#'))
                continue;

            overrideData = json[cpuStr].toObject();
            overrideHash = getHash(ovrdPath);
            emit logMessageSent(QString("InputRanges: loaded ranges override for %1").arg(dmiProd));
            break;
        }
    }

    void InputRanges::load(const QString &dmiProduct, const QString &cpuBrandStr) {
        if (dmiProd == dmiProduct && cpuBrand == cpuBrandStr && !data.isEmpty())
            return;

        QJsonDocument json;

        dmiProd = dmiProduct;
        cpuBrand = cpuBrandStr;
        data = QJsonObject();
        overrideData = QJsonObject();

        if (!readJson(QString("%1%2.json").arg(builtInPath, dmiProduct), json))
            return;

        for (const QString &cpuStr: json.object().keys()) {
            if (!cpuBrand.contains(cpuStr) || cpuStr.startsWith('#'))
                continue;

            data = json[cpuStr].toObject();
            emit logMessageSent(QString("InputRanges: loaded ranges for %1").arg(dmiProd));
            break;
        }
    }
}
