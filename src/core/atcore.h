/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016, 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2016, 2019 Lays Rodrigues <lays.rodrigues@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
*/

#pragma once
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <memory>

#include "atcore_export.h"
#include "beddeform.h"
#include "ifirmware.h"
#include "temperature.h"

class SerialLayer;
class IFirmware;
class QTime;

/**
 * @brief The AtCore class
 * aims to provides a high level interface for serial based gcode devices<br />
 *
 * #### General Workflow
 * - Connect to a serial port with newConnection()
 * - Send commands to the device (pushCommand(), print(), ...)
 * - AtCore::close() when you are all done.

 * #### How AtCore Finds Plugins.
 * AtCore will check each directory below for plugins.
 *  1. QApplication::applicationDirPath/plugins (runtime)
 *  2. QApplication::applicationDirPath/AtCore (runtime)
 *  3. QApplication::applicationDirPath/../PlugIns/AtCore (runtime)
 *  4. Fullpath of KDE_PLUGIN_DIR (buildtime)
 *  5. Qt Plugin path/AtCore (runtime)
 *  6. ECM set KDE PLUGIN DIR  (buildtime)
 *  7. Build Dir/plugins (buildtime)
 */
class ATCORE_EXPORT AtCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QStringList availableFirmwarePlugins READ availableFirmwarePlugins NOTIFY availableFirmwarePluginsChanged)
    Q_PROPERTY(int extruderCount READ extruderCount WRITE setExtruderCount NOTIFY extruderCountChanged)
    Q_PROPERTY(int temperatureTimerInterval READ temperatureTimerInterval WRITE setTemperatureTimerInterval NOTIFY temperatureTimerIntervalChanged);
    Q_PROPERTY(int serialTimerInterval READ serialTimerInterval WRITE setSerialTimerInterval NOTIFY serialTimerIntervalChanged)
    Q_PROPERTY(QStringList serialPorts READ serialPorts NOTIFY portsChanged)
    Q_PROPERTY(float percentagePrinted READ percentagePrinted NOTIFY printProgressChanged)
    Q_PROPERTY(QStringList portSpeeds READ portSpeeds CONSTANT)
    Q_PROPERTY(QString connectedPort READ connectedPort CONSTANT)
    Q_PROPERTY(AtCore::STATES state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool sdMount READ isSdMounted WRITE setSdMounted NOTIFY sdMountChanged)
    Q_PROPERTY(QStringList sdFileList READ sdFileList NOTIFY sdCardFileListChanged)
    Q_PROPERTY(bool autoTemperatureReport READ autoTemperatureReport WRITE setAutoTemperatureReport NOTIFY autoTemperatureReportChanged)
    Q_PROPERTY(Temperature *temperature READ temperature CONSTANT)

    friend class AtCoreTests;
    // Add friends as Sd Card support is extended to more plugins.
    friend class RepetierPlugin;
    friend class MarlinPlugin;
    // friend class SmoothiePlugin;
    // friend class TeacupPlugin;
    // friend class AprinterPlugin;
    // friend class SprinterPlugin;

public:
    /**
     * @brief STATES enum Possible states the printer can be in
     */
    enum STATES {
        DISCONNECTED,  //!< Not Connected to a printer, initial state
        CONNECTING,    //!< Attempting to connect, Fw not probed
        IDLE,          //!< Connected to printer and ready for commands
        BUSY,          //!< Printer is Printing or working
        PAUSE,         //!< Printer is paused
        ERRORSTATE,    //!< Printer Returned Error
        STOP,          //!< Stop Printing and Clean Queue
        STARTPRINT,    //!< Just Starting a print job
        FINISHEDPRINT, //!< Just Finished print job
    };
    Q_ENUM(STATES)
    /**
     * @brief The AXES enum - Printer Axes.
     */
    enum AXES {
        X = 1 << 0, //!< X Axis: X Motor
        Y = 1 << 1, //!< Y Axis Y Motor
        Z = 1 << 2, //!< Z Axis Z Motor
        E = 1 << 3, //!< E Axis: Extruder Motor 0
    };
    Q_ENUM(AXES)
    /**
     * @brief The UNITS enum - Possible Mesurment Units
     */
    enum UNITS {
        METRIC,  //!< Metric Units (Meters)
        IMPERIAL //!< Imperial Units (Feet)
    };
    Q_ENUM(UNITS)
    /**
     * @brief AtCore create a new instance of AtCore
     * @param parent: parent of the object
     */
    explicit AtCore(QObject *parent = nullptr);
    ~AtCore();

    /**
     * @brief version
     * @return Version number
     */
    QString version() const;

    /**
     * @brief Returns a List of detected serial ports
     * @return List of detected ports
     * @sa newConnection(), serial(), closeConnection()
     */
    QStringList serialPorts() const;

    /**
     * @brief connectedPort
     * @return the port atcore is connected to or empty string if none
     */
    QString connectedPort() const;

    /**
     * @brief Connect to a device.
     * @param port: the port to initialize
     * @param baud: the baud of the port
     * @param fwName: Firmware name of plugin to use (will try to autodetect if fwName is unknown)
     * @param disableROC: atcore will attempt to disable reset on connect for this device.
     * @return True is connection was successful
     * @sa serialPorts(), serial(), closeConnection()
     */
    Q_INVOKABLE bool newConnection(const QString &port, int baud, const QString &fwName, bool disableROC = false);

    /**
     * @brief Returns a list of valid baud speeds
     */
    QStringList portSpeeds() const;

    /**
     * @brief Close the current serial connection
     * @sa newConnection(), serial(), serialPorts(), close()
     */
    Q_INVOKABLE void closeConnection();

    /**
     * @brief Main access to the loaded firmware plugin
     * @return IFirmware * to currently loaded plugin
     * @sa availableFirmwarePlugins(), loadFirmwarePlugin()
     */
    Q_INVOKABLE IFirmware *firmwarePlugin() const;

    /**
     * @brief List of available firmware plugins
     * @sa loadFirmwarePlugin(), firmwarePlugin()
     */
    QStringList availableFirmwarePlugins() const;

    /**
     * @brief Get Printer state
     * @return State of the printer
     * @sa setState(), stateChanged(), AtCore::STATES
     */
    AtCore::STATES state();

    /**
     * @brief extruderCount
     * @return The number of detected Extruders Default is 1
     * @sa setExtruderCount(int newCount), extruderCountChanged(int newCount)
     */
    int extruderCount() const;

    /**
     * @brief Return printed percentage
     * @sa printProgressChanged()
     */
    float percentagePrinted() const;

    /**
     * @brief The Bed Deform data as told by the Firmware.
     */
    std::shared_ptr<BedDeform> bedDeform();

    /**
     * @brief The temperature of the current hotend as told by the Firmware.
     */
    Temperature *temperature();

    /**
     * @brief Return the amount of miliseconds the serialTimer is set to. 0 = Disabled
     */
    int serialTimerInterval() const;

    /**
     * @brief Return the amount of miliseconds the temperatureTimer is set to. 0 = Disabled
     */
    int temperatureTimerInterval() const;

    /**
     * @brief Attempt to Mount an sd card
     * @param slot: Sd card Slot on machine (0 is default)
     */
    Q_INVOKABLE void mountSd(uint slot = 0);

    /**
     * @brief Attempt to Unmount an sd card
     * @param slot: Sd card Slot on machine (0 is default)
     */
    Q_INVOKABLE void umountSd(uint slot = 0);

    /**
     * @brief sdFileList
     * @return List of files on the sd card.
     */
    QStringList sdFileList();

    /**
     * @brief Check if an sd card is mounted on the printer
     * @return True if card mounted
     */
    bool isSdMounted() const;

    /**
     * @brief Check if using automatic Temperature reporting to monitor temperatures
     * @return True if using automatic temperature reporting
     */
    bool autoTemperatureReport() const;

signals:

    /**
     * @brief Message emit from atcore these should be displayed to the user for debug.
     *
     * Possable Messages Are:
     *   - Waiting for firmware detect.
     *   - No Plugin found for (detected FW)
     *   - Failed to open device in Read / Write mode.
     *   - Device Errors.
     * @param msg: the message.
     */
    void atcoreMessage(const QString &msg);

    /**
     * @brief New number of extruders
     * @sa extruderCount(), setExtruderCount(int newCount)
     */
    void extruderCountChanged(const int newCount);

    /**
     * @brief Print job's precentage changed.
     * @param newProgress : Message
     * @sa percentagePrinted()
     */
    void printProgressChanged(const float newProgress);

    /**
     * @brief New message was received from the printer
     * @param message: Message that was received
     */
    void receivedMessage(const QByteArray &message);

    /**
     * @brief New interval for serial timer
     * @sa setSerialTimerInterval()
     */
    void serialTimerIntervalChanged(const int newTime);

    /**
     * @brief New interval for temperature timer
     * @sa setTemperatureTimerInterval()
     */
    void temperatureTimerIntervalChanged(const int newTime);

    /**
     * @brief use of automatic temperature reporting has changed
     * @param autoReport: True if using automatic Reporting mode.
     */
    void autoTemperatureReportChanged(bool autoReport);

    /**
     * @brief New interval for automatic temperature report
     * @sa setautoTemperatureReport()
     */
    void autoCheckTemperatureIntervalChanged(const int newTime);

    /**
     * @brief The Printer's State Changed
     * @param newState : the new state of the printer
     * @sa setState(), state(), AtCore::STATES
     */
    void stateChanged(AtCore::STATES newState);

    /**
     * @brief Available serialports Changed
     */
    void portsChanged(const QStringList &portList);

    /**
     * @brief Sd Card Mount Changed
     */
    void sdMountChanged(bool newState);

    /**
     * @brief The files on the sd card have changed.
     */
    void sdCardFileListChanged(const QStringList &fileList);

    /**
     * @brief pushedCommand via serialLayer connect this to your log to see send commands
     * @param comm: the command sent.
     */
    void pushedCommand(const QByteArray &comm);

    /**
     * @brief availableFirmwarePluginsChanged notify about the new plugins available
     */
    void availableFirmwarePluginsChanged();

public slots:

    /**
     * @brief Set the printers state
     * @param state : printer state.
     * @sa state(), stateChanged(), AtCore::STATES
     */
    void setState(AtCore::STATES state);

    /**
     * @brief updateFWPlugins Check for new Firmware plugins.
     */
    Q_INVOKABLE void updateFWPlugins();

    /**
     * @brief Push a command into the command queue
     *
     * @param comm : Command
     */
    Q_INVOKABLE void pushCommand(const QString &comm);

    /**
     * @brief Public Interface for printing a file
     * @param fileName: the gcode file to print.
     * @param sdPrint: set true to print fileName from Sd card
     */
    Q_INVOKABLE void print(const QString &fileName, bool sdPrint = false);

    /**
     * @brief Stop the Printer by empting the queue and aborting the print job (if running)
     * @sa emergencyStop(), pause(), resume()
     */
    Q_INVOKABLE void stop();

    /**
     * @brief stop the printer via the emergency stop Command (M112)
     * @sa stop(), pause(), resume()
     */
    Q_INVOKABLE void emergencyStop();

    /**
     * @brief pause an in process print job
     *
     * Sends M114 on pause to store the location where the head stoped.
     * This is known to cause problems on fake printers
     * @param pauseActions: Gcode to run after pausing commands are ',' separated
     * @sa resume(), stop(), emergencyStop()
     */
    void pause(const QString &pauseActions);

    /**
     * @brief resume a paused print job.
     * After returning to location pause was triggered.
     * @sa pause(), stop(), emergencyStop()
     */
    Q_INVOKABLE void resume();

    /**
     * @brief Send home \p axis command
     * @param axis: the axis(es) to home (use X Y Z or any combo of)
     * @sa home(), move()
     */
    Q_INVOKABLE void home(uchar axis);

    /**
     * @brief Send home all command
     * @sa home(uchar axis), move()
     */
    Q_INVOKABLE void home();

    /**
     * @brief Set extruder temperature
     * @param temp : new temperature
     * @param extruder : extruder number
     * @param andWait: True for heat and ignore commands until temperature is reached
     */
    Q_INVOKABLE void setExtruderTemp(int temp = 0, int extruder = 0, bool andWait = false);

    /**
     * @brief move an axis of the printer
     * @param axis the axis to move AXES (X Y Z E )
     * @param arg the distance to move the axis or the place to move to depending on printer mode
     * @sa home(), home(uchar axis), move(QLatin1Char axis, int arg)
     */
    Q_INVOKABLE void move(AtCore::AXES axis, double arg);

    /**
     * @brief move an axis of the printer
     * @param axis the axis to move AXES (X Y Z E )
     * @param arg the distance to move the axis or the place to move to depending on printer mode
     * @sa home(), home(uchar axis), move(AtCore::AXES, int arg)
     */
    Q_INVOKABLE void move(QLatin1Char axis, double arg);

    /**
     * @brief Set the bed temperature
     * @param temp : new temperature
     * @param andWait: True for heat and ignore commands until temperature is reached
     * @sa setExtruderTemp()
     */
    Q_INVOKABLE void setBedTemp(int temp = 0, bool andWait = false);

    /**
     * @brief setFanSpeed set the fan speed
     * @param fanNumber: fan number
     * @param speed: new speed of the fan 0-100
     */
    Q_INVOKABLE void setFanSpeed(int speed = 0, int fanNumber = 0);

    /**
     * @brief Set printer to absolute position mode
     * @sa setRelativePosition()
     */
    Q_INVOKABLE void setAbsolutePosition();

    /**
     * @brief Set printer to relative position mode
     * @sa setAbsolutePosition()
     */
    Q_INVOKABLE void setRelativePosition();

    /**
     * @brief Disable motors after a delay
     * @param delay: Seconds until motors are disabled. 0= No delay
     */
    Q_INVOKABLE void disableMotors(uint delay = 0);

    /**
     * @brief set the Printers speed
     * @param speed: speed in % (default is 100);
     */
    Q_INVOKABLE void setPrinterSpeed(int speed = 100);

    /**
     * @brief set extruder Flow rate
     * @param rate: flow rate in % (default is 100)
     */
    Q_INVOKABLE void setFlowRate(int rate = 100);

    /**
     * @brief close any open items.
     * You should call this on close events to force any stuck jobs to close
     * @sa closeConnection()
     */
    Q_INVOKABLE void close();

    /**
     * @brief showMessage push a message to the printers LCD
     * @param message: message to show on the LCD
     */
    Q_INVOKABLE void showMessage(const QString &message);

    /**
     * @brief setUnits sets the measurement units do be used
     * @param units : the measurement units to use(METRIC / IMPERIAL)
     * @sa AtCore::UNITS
     */
    Q_INVOKABLE void setUnits(AtCore::UNITS units);

    /**
     * @brief Set the time between checks for new serialPorts (0 is default)
     * @param newTime: Milliseconds between checks. values <= 0 will Disable Checks.
     */
    void setSerialTimerInterval(int newTime);

    /**
     * @brief Set the time between checks for new Temperature (5000 is default on new connections)
     * @param newTime: Milliseconds between checks. values <= 0 will Disable Checks.
     */
    void setTemperatureTimerInterval(int newTime);

    /** @brief Set if atcore should Enable auto temperature reporting. Temperature timer will also be stopped.
     *  @param autoReport: True to enable automatic reporting of temperatures.
     *  @sa setAutoCheckTemperature
     */
    void setAutoTemperatureReport(bool autoReport);

    /**
     * @brief Tell the machine to start reporting its temperature automaticly
     * @param newTime: Time in seconds between reports (0: disabled)
     */
    Q_INVOKABLE void setAutoCheckTemperatureInterval(int newTime);

    /**
     * @brief delete file from sd card
     */
    Q_INVOKABLE void sdDelete(const QString &fileName);

    /**
     * @brief Queue the Printer for status of sd card print
     */
    void sdCardPrintStatus();

private slots:
    /**
     * @brief processQueue send commands from the queue.
     */
    void processQueue();

    /**
     * @brief Send M105 to the printer if one is not in the Queue
     */
    void checkTemperature();

    /**
     * @brief Connect to SerialLayer::receivedCommand
     * @param message: new message.
     */
    void newMessage(const QByteArray &message);

    /**
     * @brief Connect to SerialLayer::pushedCommand
     * @param command: newCommand.
     */
    void newCommand(const QByteArray &command);

    /**
     * @brief Search for firmware string in message.
     * A Helper function for Firmware detection
     * @param message
     */
    void findFirmware(const QByteArray &message);

    /**
     * @brief Search for new serial ports
     */
    void locateSerialPort();

    /**
     * @brief Attempts to disableResetOnConnect for the selected port.
     * @param port: the port.
     */
    void disableResetOnConnect(const QString &port);

    /**
     * @brief Send request to the printer for the sd card file list.
     */
    void getSDFileList();

    /**
     * @brief Handle serial Errors.
     */
    void handleSerialError(QSerialPort::SerialPortError error);

private:
    /**
     * @brief Load A firmware plugin
     * @param fwName : name of the firmware
     * @sa firmwarePlugin(), availableFirmwarePlugins()
     */
    Q_INVOKABLE void loadFirmwarePlugin(const QString &fwName);

    /**
     * @brief True if a firmware plugin is loaded
     */
    bool firmwarePluginLoaded() const;

    /**
     * @brief True if a serial port is initialized
     */
    bool serialInitialized() const;

    /**
     * @brief send firmware request to the printer
     */
    void requestFirmware();

    /**
     * @brief returns AtCorePrivate::sdCardReadingFileList
     * @return True if printer is returning sd card file list
     */
    bool isReadingSdCardList() const;

    /**
     * @brief stops print just for sd prints used internally
     * @sa stop(), emergencyStop()
     */
    void stopSdPrint();

    /**
     * @brief New connections need to wait for the machine to be ready if it needs reboot
     * @param message: message from the firmware
     * @param fwName: fwName to load
     */
    void waitForPrinterReboot(const QByteArray &message, const QString &fwName);

    /**
     * @brief Hold private data of AtCore.
     */
    struct AtCorePrivate;
    std::unique_ptr<AtCorePrivate> d;

protected:
    /**
     * @brief Set the number of extruders on the machine.
     * @param newCount
     * @sa extruderCount(), extruderCountChanged(int newCount)
     */
    void setExtruderCount(int newCount);
    /**
     * @brief Append a file to AtCorePrivate::sdCardFileList.
     * @param fileName: new FileName
     */
    void appendSdCardFileList(const QString &fileName);

    /**
     * @brief Clear AtCorePrivate::sdCardFileList.
     */
    void clearSdCardFileList();

    /**
     * @brief Set if the sd card is mounted by the printer
     * @param mounted: True is mounted
     */
    void setSdMounted(const bool mounted);

    /**
     * @brief set AtCorePrivate::sdCardReadingFileList
     * @param readingList set true if reading file list
     */
    void setReadingSdCardList(bool readingList);
};
