// Copyright (c) 2011-2013 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qvaluecombobox.h"

QValueComboBox::QValueComboBox(QWidget* parent) : QComboBox(parent), role(Qt::UserRole)
{
<<<<<<< HEAD
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectionChanged(int)));
=======
    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QValueComboBox::handleSelectionChanged);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

QVariant QValueComboBox::value() const
{
    return itemData(currentIndex(), role);
}

void QValueComboBox::setValue(const QVariant& value)
{
    setCurrentIndex(findData(value, role));
}

void QValueComboBox::setRole(int role)
{
    this->role = role;
}

void QValueComboBox::handleSelectionChanged(int idx)
{
    Q_EMIT valueChanged();
}
