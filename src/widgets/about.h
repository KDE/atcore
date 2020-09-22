/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2017-2018 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QDialog>

#include "atcorewidgets_export.h"

/**
 * @brief An About AtCore dialog.
 */

class ATCOREWIDGETS_EXPORT About : public QDialog
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);
    ~About() = default;
};
