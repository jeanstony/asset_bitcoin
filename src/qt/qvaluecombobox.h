// Copyright (c) 2011-2015 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VCCOIN_QT_QVALUECOMBOBOX_H
#define VCCOIN_QT_QVALUECOMBOBOX_H

#include <QComboBox>
#include <QVariant>
#include <QTreeView>

/* QComboBox that can be used with QDataWidgetMapper to select ordinal values from a model. */
class QValueComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
    explicit QValueComboBox(QWidget *parent = nullptr);
    virtual ~QValueComboBox();

    QVariant value() const;
    void setValue(const QVariant &value);
    void presetIndex(QModelIndex index);
    /** Specify model role to use as ordinal value (defaults to Qt::UserRole) */
    void setRole(int role);
    int MyCurrentIndex() const;
    QVariant itemData(int index, int role = Qt::UserRole) const;

Q_SIGNALS:
    void valueChanged();

private:
    int role;

private Q_SLOTS:
    void handleSelectionChanged(int idx);
};

#endif // VCCOIN_QT_QVALUECOMBOBOX_H
