// Copyright (c) 2011-2014 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VCCOIN_QT_VCCOINADDRESSVALIDATOR_H
#define VCCOIN_QT_VCCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class VccoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit VccoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Vccoin address widget validator, checks for a valid VCcoin address.
 */
class VccoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit VccoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // VCCOIN_QT_VCCOINADDRESSVALIDATOR_H
