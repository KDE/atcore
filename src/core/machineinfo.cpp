/* AtCore
    Copyright (C) <2019>

    Authors:
        Chris Rizzitello <rizzitello@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "machineinfo.h"

#include <QCoreApplication>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(MACHINE_INFO, "org.kde.atelier.core.machineInfo")

const QMap<MachineInfo::KEY, MachineInfo::keyInfo> MachineInfo::decoderMap = {
    {KEY::NAME, {QStringLiteral("Name"), QStringLiteral("ProfileName")}},
    {KEY::BAUDRATE, {QStringLiteral("bps"), 115200}},
    {KEY::FIRMWARE, {QStringLiteral("firmware"), QStringLiteral("Auto-Detect")}},
    {KEY::MAXBEDTEMP, {QStringLiteral("maximumTemperatureBed"), 0}},
    {KEY::MAXEXTTEMP, {QStringLiteral("maximumTemperatureExtruder"), 0}},
    {KEY::POSTPAUSE, {QStringLiteral("postPause"), QString()}},
    {KEY::ISCARTESIAN, {QStringLiteral("isCartesian"), false}},
    {KEY::XMAX, {QStringLiteral("dimensionX"), 200}},
    {KEY::YMAX, {QStringLiteral("dimensionY"), 200}},
    {KEY::ZMAX, {QStringLiteral("dimensionZ"), 180}},
    {KEY::AUTOTEMPREPORT, {QStringLiteral("autoReportTemp"), false}}
};

MachineInfo *MachineInfo::instance()
{
    static MachineInfo m;
    return &m;
}

MachineInfo::MachineInfo(QObject *parent) :
    QObject(parent)
    , m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, QStringLiteral("atcore"), QStringLiteral("profiles"), this))
{
}

QObject *MachineInfo::qmlSingletonRegister(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(scriptEngine)
    engine->setObjectOwnership(instance(), QQmlEngine::CppOwnership);
    return instance();
}

QVariantMap MachineInfo::readProfile(const QString &profileName) const
{
    m_settings->sync();
    m_settings->beginGroup(profileName);
    QVariantMap data{{decoderMap[KEY::NAME].name, m_settings->group()}};
    for (int i = 1 ; i <= 10; i++) {
        data.insert(decoderMap[MachineInfo::KEY(i)].name, m_settings->value(decoderMap[MachineInfo::KEY(i)].name, decoderMap[MachineInfo::KEY(i)].defaultValue));
    }
    m_settings->endGroup();
    return data;
}

QVariant MachineInfo::readKey(const QString &profileName, MachineInfo::KEY key) const
{
    if (profileName.isEmpty()) {
        return decoderMap[key].defaultValue;
    }

    if (key == KEY::NAME) {
        return profileName;
    }

    m_settings->sync();
    return m_settings->value(QStringLiteral("%1/%2").arg(profileName, decoderMap[key].name), decoderMap[key].defaultValue);
}

bool MachineInfo::storeKey(const QString &profileName, const MachineInfo::KEY key, const QVariant &value) const
{
    if (key == KEY::NAME) {
        //copyProfile emits profilesChanged
        return copyProfile(profileName, value.toString(), true);
    }

    if (!profileNames().contains(profileName)) {
        qCWarning(MACHINE_INFO) << "Keysave: Profile does not exsits, create profile before attempting to store keys.";
        return false;
    }

    if (readKey(profileName, key) == value) {
        return false;
    }

    m_settings->beginGroup(profileName);
    m_settings->setValue(decoderMap[key].name, value);
    m_settings->endGroup();
    m_settings->sync();
    emit profilesChanged();
    return true;
}

bool MachineInfo::copyProfile(const QString &srcProfile, const QString &destProfile, bool rmSrc) const
{
    if (srcProfile.isEmpty() || destProfile.isEmpty()) {
        qCWarning(MACHINE_INFO) << "Profile Copy Error: Source or Destination Profile Is Empty.";
        return false;
    }

    if (!profileNames().contains(srcProfile)) {
        qCWarning(MACHINE_INFO) << "Profile Copy Error: Source Not Found.";
        return false;
    }

    if (srcProfile == destProfile) {
        qCWarning(MACHINE_INFO) << "Profile Copy Error: Source is Destination.";
        return false;
    }
    QVariantMap newProfile = readProfile(srcProfile);
    m_settings->beginGroup(destProfile);
    for (int i = 1 ; i <= 10; i++) {
        m_settings->setValue(decoderMap[MachineInfo::KEY(i)].name, newProfile[decoderMap[MachineInfo::KEY(i)].name]);
    }
    m_settings->endGroup();
    m_settings->sync();
    if (rmSrc) {
        removeProfile(srcProfile);
        //removeProfile emits profilesChanged
    } else {
        emit profilesChanged();
    }
    return true;
}

bool MachineInfo::removeProfile(const QString &profileName) const
{
    if (!profileNames().contains(profileName)) {
        qCWarning(MACHINE_INFO) << "Profile Remove Error: Profile not found.";
        return false;
    }
    m_settings->beginGroup(profileName);
    m_settings->remove(QString());
    m_settings->endGroup();
    m_settings->sync();
    emit profilesChanged();
    return  true;
}

void MachineInfo::storeProfile(const QMap<MachineInfo::KEY, QVariant> &profile) const
{
    m_settings->beginGroup(profile[KEY::NAME].toString());
    for (int i = 1 ; i <= 10; i++) {
        m_settings->setValue(decoderMap[MachineInfo::KEY(i)].name, profile[MachineInfo::KEY(i)]);
    }
    m_settings->endGroup();
    m_settings->sync();
    emit profilesChanged();
}

void MachineInfo::storeProfile(const QVariantMap &profile) const
{
    m_settings->beginGroup(profile.first().toString());
    for (auto it = profile.begin(), end = profile.end(); it != end; ++it) {
        m_settings->setValue(it.key(), it.value());
    }
    m_settings->endGroup();
    m_settings->sync();
    emit profilesChanged();
}

QStringList MachineInfo::profileNames() const
{
    m_settings->sync();
    return m_settings->childGroups();
}

QString MachineInfo::keyName(const MachineInfo::KEY key) const
{
    auto it = decoderMap.find(key);
    if (it != decoderMap.end()) {
        return it.value().name;
    }
    return QString();
}
