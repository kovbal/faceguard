/*M///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015, Balázs Kovács, Gergő Róth
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the University of Pannonia nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//M*/

#include "database.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>

#include "facerecognizercontainer.h"

const QMap<QString, int>& Database::GetNameLabels() const
{
    return nameLabels;
}

void Database::ExportNameLabels(const QString& fileName)
{
	QJsonDocument nameLabelsDoc;
    QJsonArray nameLablesJson;

    for (QMapIterator<QString, int> it(nameLabels); it.hasNext();)
    {
        it.next();
        QJsonObject val;
        val["name"] = it.key();
        val["label"] = it.value();
        nameLablesJson.append(val);
	}
	nameLabelsDoc.setArray(nameLablesJson);

	const QByteArray bytes = nameLabelsDoc.toJson();
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(bytes);
        file.close();
    }
    else
    {
        qWarning() << "Can't open output file.";
    }
}

void Database::AddImage(const QString& label, const cv::Mat& image)
{
    int tmp = -1;
    QMap<QString, int>::iterator it = nameLabels.find(label);
    if (it == nameLabels.end())
    {
        tmp = nameLabels.size();
        nameLabels[label] = tmp;
    }
    else
    {
        tmp = it.value();
    }

    trainSrc.push_back(image);
    trainLabels.push_back(tmp);
}

void Database::Train()
{
    FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->train(trainSrc, trainLabels);
}

void Database::Save(const QString& filePath)
{
    FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->save(filePath.toStdString());
}

void Database::Load(const QString& filePath)
{
    FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->load(filePath.toStdString());
}

QString Database::FindNameByLabel(int label)
{
    QMapIterator<QString, int> it(nameLabels);

    while (it.hasNext())
    {
        auto item = it.next();
        if (item.value() == label)
        {
            return item.key();
        }
    }

    return QString();
}

void Database::ImportNameLabels(const QString& fileName)
{
    nameLabels.clear();

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        const QByteArray fullJson = file.readAll();
        QJsonParseError err;
        const QJsonDocument doc = QJsonDocument::fromJson(fullJson, &err);
        if (err.error == QJsonParseError::NoError)
        {
            const QJsonArray mainArray = doc.array();
            for (const QJsonValue& i : mainArray)
            {
                const QJsonObject obj = i.toObject();
                nameLabels[obj["name"].toString()] = obj["label"].toInt();
            }
        }
        else
        {
            qWarning() << "Input JSON cannot be parsed.";
        }
    }
}
