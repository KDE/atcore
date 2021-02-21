/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019-2020 Chris Rizzitello <rizzitello@kde.org>
*/

#include "profilemanager.h"
#include "atcore_default_folders.h"
#include "machineinfo.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
#include <QCoreApplication>
#include <QDir>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSpinBox>
#include <QToolButton>

ProfileManager::ProfileManager(QWidget *parent)
    : QWidget(parent)
{
    auto newLabel = new QLabel(tr("Profile:"));
    cbProfile = new QComboBox();
    cbProfile->setEditable(true);
    cbProfile->addItems(MachineInfo::instance()->profileNames());
    cbProfile->setCompleter(new QCompleter(MachineInfo::instance()->profileNames()));
    connect(MachineInfo::instance(), &MachineInfo::profilesChanged, this, [this] {
        int index = cbProfile->currentIndex();
        cbProfile->clear();
        cbProfile->completer()->deleteLater();
        cbProfile->addItems(MachineInfo::instance()->profileNames());
        cbProfile->setCompleter(new QCompleter(MachineInfo::instance()->profileNames()));
        cbProfile->setCurrentIndex(std::min<int>(index, cbProfile->count() - 1));
    });

    connect(cbProfile, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] {
        if (MachineInfo::instance()->profileNames().contains(cbProfile->currentText())) {
            loadProfile(cbProfile->currentText());
        }
    });

    connect(cbProfile->lineEdit(), &QLineEdit::editingFinished, this, &ProfileManager::onCbProfileEditingFinished);

    auto newHLayout = new QHBoxLayout();
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(cbProfile, 75);

    auto newButton = new QToolButton();
    newButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete"), style()->standardIcon(QStyle::SP_DialogDiscardButton)));
    newButton->setToolTip(tr("Delete Current Profile"));
    newButton->setIconSize(QSize(fontMetrics().height(), fontMetrics().height()));
    connect(newButton, &QToolButton::clicked, this, [this] {
        if (!cbProfile->currentText().isEmpty()) {
            MachineInfo::instance()->removeProfile(cbProfile->currentText());
        }
    });

    auto mainLayout = new QVBoxLayout();
    newHLayout->addWidget(newButton);
    mainLayout->addLayout(newHLayout);
    auto profileLayout = new QVBoxLayout();
    auto boxLayout = new QVBoxLayout();

    newLabel = new QLabel(tr("Name"));
    lineName = new QLineEdit();
    connect(lineName, &QLineEdit::editingFinished, this, [this] {
        QString itemName = lineName->text();
        if (MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::NAME, itemName)) {
            loadProfile(itemName);
        }
    });
    newHLayout = new QHBoxLayout;
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(lineName);
    profileLayout->addLayout(newHLayout);

    newLabel = new QLabel(tr("Printer Type:"));
    radioDelta = new QRadioButton(tr("Delta"));
    radioCartesian = new QRadioButton(tr("Cartesian"));
    radioCartesian->setChecked(true);
    connect(radioCartesian, &QRadioButton::toggled, this, &ProfileManager::onRadioCartesianToggled);

    newHLayout = new QHBoxLayout;
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(radioCartesian);
    newHLayout->addWidget(radioDelta);
    boxLayout->addLayout(newHLayout);

    lblX = new QLabel(tr("Maximum X"));
    sbMaxX = new QSpinBox();
    sbMaxX->setMaximum(std::numeric_limits<int>::max());
    sbMaxX->setSuffix(QStringLiteral("mm"));
    connect(sbMaxX, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::XMAX, value); });

    newHLayout = new QHBoxLayout;
    newHLayout->addWidget(lblX);
    newHLayout->addWidget(sbMaxX);
    boxLayout->addLayout(newHLayout);

    newLabel = new QLabel(tr("Maximum Y"));
    sbMaxY = new QSpinBox();
    sbMaxY->setMaximum(std::numeric_limits<int>::max());
    sbMaxY->setSuffix(QStringLiteral("mm"));
    connect(sbMaxY, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::YMAX, value); });

    newHLayout = new QHBoxLayout;
    newHLayout->setContentsMargins(0, 0, 0, 0);
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(sbMaxY);
    axisY = new QWidget(this);
    axisY->setLayout(newHLayout);
    boxLayout->addWidget(axisY);

    lblZ = new QLabel(tr("Maximum Z"));
    sbMaxZ = new QSpinBox();
    sbMaxZ->setMaximum(std::numeric_limits<int>::max());
    sbMaxZ->setSuffix(QStringLiteral("mm"));
    connect(sbMaxZ, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::ZMAX, value); });

    newHLayout = new QHBoxLayout;
    newHLayout->addWidget(lblZ);
    newHLayout->addWidget(sbMaxZ);
    boxLayout->addLayout(newHLayout);

    auto groupBox = new QGroupBox(tr("Mechanics"));
    groupBox->setLayout(boxLayout);
    profileLayout->addWidget(groupBox);

    boxLayout = new QVBoxLayout;
    newLabel = new QLabel(tr("Bed Maximum"));
    sbMaxBedTemp = new QSpinBox();
    sbMaxBedTemp->setMaximum(999);
    sbMaxBedTemp->setSuffix(QStringLiteral(" ºC"));
    connect(sbMaxBedTemp, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::MAXBEDTEMP, value); });

    newHLayout = new QHBoxLayout;
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(sbMaxBedTemp);
    boxLayout->addLayout(newHLayout);

    sbMaxExtTemp = new QSpinBox();
    sbMaxExtTemp->setMaximum(999);
    newLabel = new QLabel(tr("Extruder Maximum"));
    sbMaxExtTemp->setSuffix(QStringLiteral(" ºC"));
    connect(sbMaxExtTemp, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::MAXEXTTEMP, value); });

    newHLayout = new QHBoxLayout();
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(sbMaxExtTemp);
    boxLayout->addLayout(newHLayout);

    checkAutoTempReport = new QCheckBox(tr("Auto Temperature Report"));
    checkAutoTempReport->setLayoutDirection(Qt::RightToLeft);
    connect(checkAutoTempReport, &QCheckBox::toggled, this, [this](bool checked) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::AUTOTEMPREPORT, checked); });
    boxLayout->addWidget(checkAutoTempReport, 0, Qt::AlignRight);

    groupBox = new QGroupBox(tr("Temperature"));
    groupBox->setLayout(boxLayout);
    profileLayout->addWidget(groupBox);

    boxLayout = new QVBoxLayout();
    cbBaud = new QComboBox();
    cbBaud->addItems(BAUDS);
    cbBaud->setCurrentText(QStringLiteral("115200"));
    connect(cbBaud, &QComboBox::currentTextChanged, this, [this](const QString &newText) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::BAUDRATE, newText); });

    newLabel = new QLabel(tr("Bit Rate"));
    newHLayout = new QHBoxLayout();
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(cbBaud);
    boxLayout->addLayout(newHLayout);

    newLabel = new QLabel(tr("Firmware"));
    cbFirmware = new QComboBox();
    cbFirmware->addItem(QStringLiteral("Auto-Detect"));
    cbFirmware->addItems(detectFWPlugins());
    connect(cbFirmware, &QComboBox::currentTextChanged, this, [this](const QString &newText) { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::FIRMWARE, newText); });

    newHLayout = new QHBoxLayout();
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(cbFirmware);
    boxLayout->addLayout(newHLayout);

    linePostPause = new QLineEdit();
    connect(linePostPause, &QLineEdit::editingFinished, this, [this] { MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::POSTPAUSE, linePostPause->text()); });

    newLabel = new QLabel(tr("PostPause"));
    newHLayout = new QHBoxLayout();
    newHLayout->addWidget(newLabel);
    newHLayout->addWidget(linePostPause);
    boxLayout->addLayout(newHLayout);

    groupBox = new QGroupBox(tr("Advanced"));
    groupBox->setLayout(boxLayout);
    profileLayout->addWidget(groupBox);
    groupBox = new QGroupBox(tr("Profile"));
    groupBox->setLayout(profileLayout);
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);
    loadProfile(cbProfile->currentText());
}

void ProfileManager::onCbProfileEditingFinished()
{
    if (MachineInfo::instance()->profileNames().contains(cbProfile->currentText())) {
        loadProfile(cbProfile->currentText());
        return;
    }
    QMap<MachineInfo::KEY, QVariant> newProfile = {{MachineInfo::KEY::NAME, cbProfile->currentText()},
                                                   {MachineInfo::KEY::FIRMWARE, cbFirmware->currentText()},
                                                   {MachineInfo::KEY::BAUDRATE, cbBaud->currentText()},
                                                   {MachineInfo::KEY::POSTPAUSE, linePostPause->text()},
                                                   {MachineInfo::KEY::ISCARTESIAN, radioCartesian->isChecked()},
                                                   {MachineInfo::KEY::XMAX, sbMaxX->value()},
                                                   {MachineInfo::KEY::YMAX, sbMaxY->value()},
                                                   {MachineInfo::KEY::ZMAX, sbMaxZ->value()},
                                                   {MachineInfo::KEY::AUTOTEMPREPORT, checkAutoTempReport->isChecked()},
                                                   {MachineInfo::KEY::MAXBEDTEMP, sbMaxBedTemp->value()},
                                                   {MachineInfo::KEY::MAXEXTTEMP, sbMaxExtTemp->value()}};
    MachineInfo::instance()->storeProfile(newProfile);
    loadProfile(newProfile[MachineInfo::KEY::NAME].toString());
}

void ProfileManager::onRadioCartesianToggled(bool checked)
{
    axisY->setVisible(checked);
    if (checked) {
        lblX->setText(tr("Maximum X"));
        lblZ->setText(tr("Maximum Z"));
    } else {
        lblX->setText(tr("Radius"));
        lblZ->setText(tr("Height"));
    }
    if (MachineInfo::instance()->profileNames().contains(cbProfile->currentText())) {
        MachineInfo::instance()->storeKey(cbProfile->currentText(), MachineInfo::KEY::ISCARTESIAN, checked);
    }
}

void ProfileManager::loadProfile(const QString &profileName)
{
    if (profileName.isEmpty()) {
        return;
    }
    blockSignals(true);
    lineName->setText(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::NAME).toString());
    radioCartesian->setChecked(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::ISCARTESIAN).toBool());
    radioDelta->setChecked(!MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::ISCARTESIAN).toBool());
    sbMaxX->setValue(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::XMAX).toInt());
    sbMaxY->setValue(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::YMAX).toInt());
    sbMaxZ->setValue(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::ZMAX).toInt());
    checkAutoTempReport->setChecked(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::AUTOTEMPREPORT).toBool());
    sbMaxBedTemp->setValue(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::MAXBEDTEMP).toInt());
    sbMaxExtTemp->setValue(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::MAXEXTTEMP).toInt());
    cbFirmware->setCurrentText(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::FIRMWARE).toString());
    cbBaud->setCurrentText(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::BAUDRATE).toString());
    linePostPause->setText(MachineInfo::instance()->readKey(profileName, MachineInfo::KEY::POSTPAUSE).toString());
    cbProfile->setCurrentText(profileName);
    cbProfile->setCurrentIndex(MachineInfo::instance()->profileNames().indexOf(profileName));
    blockSignals(false);
}

QStringList ProfileManager::detectFWPlugins()
{
    QStringList firmwares;
    for (const QString &path : AtCoreDirectories::pluginDir) {
        const auto pluginList = QDir(path).entryList({AtCoreDirectories::pluginExtFilter}, QDir::Files);
        for (QString file : pluginList) {
            file = file.split(QStringLiteral(".")).at(0).toLower().simplified();
            if (file.startsWith(QStringLiteral("lib")))
                file = file.remove(QStringLiteral("lib"));
            firmwares.append(file);
        }
        if (!firmwares.isEmpty())
            break; //Use first path with valid files
    }
    return firmwares;
}
