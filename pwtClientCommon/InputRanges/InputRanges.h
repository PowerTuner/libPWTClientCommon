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

#include <QJsonObject>

#include "../Include/clientcommonExport.h"
#include "pwtShared/Include/Types/MinMax.h"

namespace PWT::UI {
    class PWTCLIENTCOMMON_EXPORT InputRanges final: public QObject {
        Q_OBJECT

    private:
        static constexpr char builtInPath[] = ":/rangesDB/inputRanges/";
        inline static QSharedPointer<InputRanges> instance;
        QString path;
        QJsonObject data;
        QJsonObject overrideData;
        QByteArray overrideHash;
        QString dmiProd;
        QString cpuBrand;

        InputRanges() = default;

        [[nodiscard]] QByteArray getHash(const QString &path) const;
        PWTS::MinMax getRange(const QString &key, int minDefault, int maxDefault) const;
        [[nodiscard]] bool readJson(const QString &jsonPath, QJsonDocument &json);
        void tryLoadOverride();

    public:
        InputRanges(InputRanges &) = delete;

        void refresh() { tryLoadOverride(); }

        // amd
        PWTS::MinMax getRADJPl() const { return getRange("radj_pl", 1000, 150 * 1000); }
        PWTS::MinMax getRADJTctl() const { return getRange("radj_tctl", 70, 105); }
        PWTS::MinMax getRADJAPUSlow() const { return getRange("radj_apu_slow", 1000, 150 * 1000); }
        PWTS::MinMax getRADJAPUSkinTemp() const { return getRange("apu_skin_temp", 0, 100); }
        PWTS::MinMax getRADJDGPUSkinTemp() const { return getRange("radj_dgpu_skin_temp", 0, 100); }
        PWTS::MinMax getRADJVrmCurrent() const { return getRange("radj_vrm_current", 8 * 1000, 300 * 1000); }
        PWTS::MinMax getRADJVrmSocCurrent() const { return getRange("radj_vrm_soc_current", 8 * 1000, 300 * 1000); }
        PWTS::MinMax getRADJGfxClock() const { return getRange("radj_gfx_clock", 0, 0); }
        PWTS::MinMax getRADJCO() const { return getRange("radj_co", -50, 50); }

        // intel
        PWTS::MinMax getIntelPl() const { return getRange("intl_pl", 1000, 150 * 1000); }
        PWTS::MinMax getIntelPl4() const { return getRange("intl_pl4", 0, 500 * 1000); }
        PWTS::MinMax getIntelPP1() const { return getRange("intl_pp1", 0, 300 * 1000); }
        PWTS::MinMax getIntelFIVR() const { return getRange("intl_fivr", -200, 0); }
        PWTS::MinMax getIntelTurboPwrCurrentTDP() const { return getRange("intl_turbo_pwr_current_tdp", 1000, 150 * 1000); }
        PWTS::MinMax getIntelTurboPwrCurrentTDC() const { return getRange("intl_turbo_pwr_current_tdc", 1000, 70 * 1000); }

        [[nodiscard]] static QSharedPointer<InputRanges> getInstance();
        void setAppDataPath(const QString &appDataPath);
        void load(const QString &dmiProduct, const QString &cpuBrandStr);

    signals:
        void logMessageSent(const QString &msg);
    };
}
