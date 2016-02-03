//
//  OffscreenUi.cpp
//  interface/src/render-utils
//
//  Created by Bradley Austin Davis on 2015-04-04
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#include "FileDialogHelper.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>


QUrl FileDialogHelper::home() {
    return pathToUrl(QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0]);
}

QStringList FileDialogHelper::standardPath(StandardLocation location) {
    return QStandardPaths::standardLocations(static_cast<QStandardPaths::StandardLocation>(location)); 
}

QString FileDialogHelper::urlToPath(const QUrl& url) { 
    return url.toLocalFile(); 
}

bool FileDialogHelper::fileExists(const QString& path) {
    return QFile(path).exists();
}

bool FileDialogHelper::validPath(const QString& path) {
    return QFile(path).exists(); 
}

bool FileDialogHelper::validFolder(const QString& path) { 
    return QDir(path).exists(); 
}

QUrl FileDialogHelper::pathToUrl(const QString& path) { 
    return QUrl::fromLocalFile(path); 
}


QUrl FileDialogHelper::saveHelper(const QString& saveText, const QUrl& currentFolder, const QStringList& selectionFilters) {
    qDebug() << "Calling save helper with " << saveText << " " << currentFolder << " " << selectionFilters;

    QFileInfo fileInfo(saveText);

    // Check if it's a relative path and if it is resolve to the absolute path
    {
        if (fileInfo.isRelative()) {
            fileInfo = QFileInfo(currentFolder.toLocalFile() + "/" + fileInfo.filePath());
        }
    }

    // Check if we need to append an extension, but only if the current resolved path isn't a directory
    if (!fileInfo.isDir()) {
        QString fileName = fileInfo.fileName();
        if (!fileName.contains(".") && selectionFilters.size() == 1) {
            const QRegularExpression extensionRe{ ".*(\\.[a-zA-Z0-9]+)$" };
            QString filter = selectionFilters[0];
            auto match = extensionRe.match(filter);
            if (match.hasMatch()) {
                fileInfo = QFileInfo(fileInfo.filePath() + match.captured(1));
            }
        }
    }

    return QUrl::fromLocalFile(fileInfo.absoluteFilePath());
}

bool FileDialogHelper::urlIsDir(const QUrl& url) {
    return QFileInfo(url.toLocalFile()).isDir();
}

bool FileDialogHelper::urlIsFile(const QUrl& url) {
    return QFileInfo(url.toLocalFile()).isFile();
}

bool FileDialogHelper::urlExists(const QUrl& url) {
    return QFileInfo(url.toLocalFile()).exists();
}

bool FileDialogHelper::urlIsWritable(const QUrl& url) {
    return QFileInfo(url.toLocalFile()).isWritable();
}
