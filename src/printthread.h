/* AtCore
   Copyright (C) <2017>

   Authors:
       Chris Rizzitello <sithlord48@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QTextStream>
#include <QFile>
#include "atcore.h"

class PrintThreadPrivate;

class KATCORE_EXPORT PrintThread : public QObject
{
    Q_OBJECT
public:
    PrintThread(AtCore *parent, QString fileName);
    void nextLine();
signals:
    void finished();
    void error(QString err);
    void printProgressChanged(float);
    void nextCommand(const QString &comm);
    void stateChanged(const PrinterState &state);
    void printTimeChanged(const QTime &time);
    void printTimeLeftChanged(const QTime &time);

public slots:
    void start();
    void setState(const PrinterState &state);

private slots:
    void commandReady();
    void endPrint();

private:
    PrintThreadPrivate *d;
    QTime *time;
};
