/**
 * QtJson - A simple class for parsing JSON data into a QVariant hierarchies and vice-versa.
 * Copyright (C) 2011  Eeli Reilin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file json.h
 */

#ifndef JSON_H
#define JSON_H

#include <QVariant>
#include <QString>


/**
 * \namespace QtJson
 * \brief A JSON data parser
 *
 * Json parses a JSON data into a QVariant hierarchy.
 */
namespace QtJson {
    typedef QVariantMap JsonObject;
    typedef QVariantList JsonArray;

    /**
     * Parse a JSON string
     *
     * \param json The JSON data
     */
    QVariant parse(const QString &json);

    /**
     * Parse a JSON string
     *
     * \param json The JSON data
     * \param success The success of the parsing
     */
    QVariant parse(const QString &json, bool &success);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     *
     * \return QByteArray Textual JSON representation in UTF-8
     */
    QByteArray serialize(const QVariant &data);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     * \param success The success of the serialization
     *
     * \return QByteArray Textual JSON representation in UTF-8
     */
    QByteArray serialize(const QVariant &data, bool &success);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     *
     * \return QString Textual JSON representation
     */
    QString serializeStr(const QVariant &data);

    /**
     * This method generates a textual JSON representation
     *
     * \param data The JSON data generated by the parser.
     * \param success The success of the serialization
     *
     * \return QString Textual JSON representation
     */
    QString serializeStr(const QVariant &data, bool &success);
}

#endif //JSON_H
