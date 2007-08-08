/* This file is part of the KDE project
 * Copyright (C) 2006-2007 Thomas Zander <zander@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KOTEXT_STYLES_PRIVATE_H
#define KOTEXT_STYLES_PRIVATE_H

#include <QVariant>
#include <QHash>

class StylePrivate {
public:
    StylePrivate();
    ~StylePrivate();

    void add(int key, const QVariant &value);
    void remove(int key);
    const QVariant value(int key) const;
    bool contains(int key) const;
    void copyMissing(const StylePrivate *other);
    void removeDuplicates(const StylePrivate *other);
    void clearAll() { m_properties.clear(); }

    bool operator==( const StylePrivate &other ) const;
private:
    QHash<int, QVariant> m_properties;
};
#endif
