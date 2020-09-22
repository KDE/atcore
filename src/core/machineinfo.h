/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2019 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#pragma once
#include "atcore_export.h"

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QVariant>

class ATCORE_EXPORT MachineInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList profileNames READ profileNames NOTIFY profilesChanged)
public:
    /**
     * @brief KEYS enum Possible keys for the printer settings
     */
    enum class KEY {
        NAME = 0,       //!< Profile Name
        BAUDRATE,       //!< Machine BAUD Rate
        FIRMWARE,       //!< Firmware name
        MAXBEDTEMP,     //!< Maximum Bed Temperature
        MAXEXTTEMP,     //!< Maximum Extruder Temperature
        POSTPAUSE,      //!< Post Pause Commands
        ISCARTESIAN,    //!< Is the machine Cartesian
        XMAX,           //!< X Size for Cartesian machines, Radius for Deltas.
        YMAX,           //!< Y Size
        ZMAX,           //!< Z Size for Cartesian machines, Height for Deltas.
        AUTOTEMPREPORT, //!< Supports AutoTemp Reporting.
    };
    Q_ENUM(KEY)

    /**
     * @brief Get the MachineInfo Instance.
     * @sa qmlSingletonRegister()
     */
    static MachineInfo *instance();

    /**
     * @brief Register The MachineInfo Singleton for QML
     */
    QObject *qmlSingletonRegister(QQmlEngine *engine, QJSEngine *scriptEngine);

    /**
     * @brief Read a full profile
     * @param profileName: name of the profile you want to read.
     * @return A full map of the profile
     * @sa readKey()
     */
    Q_INVOKABLE QVariantMap readProfile(const QString &profileName) const;

    /**
     * @brief Read a key from a profile
     * @param profileName: name of the profile you want to read from.
     * @param key: the Key you want to read
     * @return The value of the requested Key
     * @sa readProfile()
     */
    Q_INVOKABLE QVariant readKey(const QString &profileName, const MachineInfo::KEY key) const;

    /**
     * @brief Store a new profile, Must be used for new profiles.
     * @param profile: A complete machine profile or one that at very least has MachineInfo::Name and any other valid Key.
     * @sa storeKey()
     */
    void storeProfile(const QMap<MachineInfo::KEY, QVariant> &profile) const;

    /**
     * @brief Store a new profile, Must be used for new profiles or to override a profile.
     * @param profile: A complete machine profile or one that at very least has a profile Name and any other valid Key.
     * @sa storeKey()
     */
    Q_INVOKABLE void storeProfile(const QVariantMap &profile) const;

    /**
     * @brief Store a key to an existing profile, Sending a key of Key::NAME will rename the profile
     * @param profileName: profile to write into
     * @param key: The key you will write to
     * @param value: The value you will store.
     * @return true if successful
     * @sa storeProfile()
     */
    Q_INVOKABLE bool storeKey(const QString &profileName, const MachineInfo::KEY key, const QVariant &value) const;

    /**
     * @brief copyies a profile and optionally deletes the src profile
     * @param srcProfile: profiles Current Name
     * @param destProfile: profiles New Name
     * @param rmSrc: delete srcProfile (defalut false)
     * @return true if successful
     */
    Q_INVOKABLE bool copyProfile(const QString &srcProfile, const QString &destProfile, bool rmSrc = false) const;
    /**
     * @brief Remove a full profile
     * @param profileName: name of the profile you want to remove.
     * @return true if successful.
     */
    Q_INVOKABLE bool removeProfile(const QString &profileName) const;

    /**
     * @brief Get a list of all the profile names
     * @return QStringList containing the stored profile names.
     */
    QStringList profileNames() const;

    /**
     * @brief Translate a key enum to string
     * @param key: name of the key to be translated.
     * @return key string if successful.
     */
    Q_INVOKABLE QString keyName(const MachineInfo::KEY key) const;

signals:
    /**
     * @brief A profile has changed
     */
    void profilesChanged() const;

private:
    MachineInfo *operator=(MachineInfo &other) = delete;
    MachineInfo(const MachineInfo &other) = delete;
    explicit MachineInfo(QObject *parent = nullptr);
    ~MachineInfo() = default;

    /**
     * @brief used to hold MachineInfo::KEY name and defaultValues.
     */
    struct keyInfo {
        QString name;          //!< Key name used in the settings file
        QVariant defaultValue; //!< Defaut Value for the key
    };
    /**
     * @brief Map of MachineInfo::KEY , KeyString and DefaultValue
     */
    static const QMap<MachineInfo::KEY, MachineInfo::keyInfo> decoderMap;

    /**
     * @brief m_settings our settings object.
     */

    QSettings *m_settings = nullptr;
};
