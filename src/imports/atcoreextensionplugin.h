/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#pragma once
#include <QQmlExtensionPlugin>

class AtCoreExtensionPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atcore")

public:
    void registerTypes(const char *uri) override;
};
