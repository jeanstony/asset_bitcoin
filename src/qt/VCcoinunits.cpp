// Copyright (c) 2011-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/VCcoinunits.h>

#include <QStringList>

QVector<CoinAsset> VccoinUnits::AssetCoins;
QVector<int> VccoinUnits::ActAssetNoStack;
int VccoinUnits::CurrentActAssetNo = 0;

size_t VccoinUnits::AddAssetCoin(const CoinAsset& ca)
{
    for (QVector<CoinAsset>::const_iterator i = AssetCoins.cbegin(); i != AssetCoins.end(); i++) {
        if (i->no == ca.no) {
            return AssetCoins.size();
        }
    }

    AssetCoins.push_back(ca);
    return AssetCoins.size();
}

bool VccoinUnits::SetActiveCoinAsset(const int no)
{

    return false;
}

int VccoinUnits::PushAsset(void)
{
    return tmp;
}

int VccoinUnits::PopAsset(void)
{
    return CurrentActAssetNo;
}

const CoinAsset* VccoinUnits::GetCoinAsset(const int no)
{
    for (QVector<CoinAsset>::const_iterator i = AssetCoins.cbegin(); i != AssetCoins.end(); i++) {
        if (i->no == no) {
            return i;
        }
    }

    return NULL;
}

VccoinUnits::VccoinUnits(QObject* parent, bool ForFee) : QAbstractListModel(parent), OnlyForFee(ForFee)
{
    Reset();
    unitlist = availableUnits();
    unitlistForFee = availableUnits();
}

void VccoinUnits::Reset(void)
{
    AssetCoins.clear();
    CurrentActAssetNo = 0;

    std::vector<CoinAsset> cas;
    CoinAssetManager::Instance().GetAllAssets(cas);
    for (auto ca : cas) {
        AssetCoins.push_back(ca);
    }
}

QList<VccoinUnits::Unit> VccoinUnits::availableUnits()
{
    QList<VccoinUnits::Unit> unitlist;
    if (CurrentActAssetNo == 0) {
        unitlist.append(VC);
        unitlist.append(mVC);
        unitlist.append(uVC);
        unitlist.append(SAT);
    } else {
        unitlist.append(VC);
    }
    return unitlist;
}

bool VccoinUnits::valid(int unit)
{
    if (CurrentActAssetNo == 0) {
        switch (unit) {
        case VC:
        case mVC:
        case uVC:
        case SAT:
            return true;
        default:
            return false;
        }
    } else {
        if (unit == 0) {
            return true;
        } else {
            return false;
		}
	}
}

QString VccoinUnits::longName(int unit)
{
    if (CurrentActAssetNo == 0) {
        switch (unit) {
        case VC:
            return QString("VC");
        case mVC:
            return QString("mVC");
        case uVC:
            return QString::fromUtf8("µVC (VCs)");
        case SAT:
            return QString("Satoshi (VCsat)");
        default:
            return QString("???");
        }
    } else {
        const CoinAsset* pca = GetCoinAsset(CurrentActAssetNo);
        assert(pca != NULL);

        if (unit == 0) {
            return QString(pca->name.c_str());
        } else {
            return QString("???");
		}
    }
}

QString VccoinUnits::shortName(int unit)
{
    if (CurrentActAssetNo == 0) {
        switch (unit) {
        case uVC:
            return QString::fromUtf8("VCs");
        case SAT:
            return QString("VCsat");
        default:
            return longName(unit);
        }
    } else {
        const CoinAsset* pca = GetCoinAsset(CurrentActAssetNo);
        assert(pca != NULL);

        if (unit == 0) {
            return QString(pca->name.c_str());
        } else {
            return QString("???");
        }   
	}
}

QString VccoinUnits::description(int unit)
{
    if (CurrentActAssetNo == 0) {
        switch (unit) {
        case VC:
            return QString("Vccoins");
        case mVC:
            return QString("Milli-Vccoins (1 / 1" THIN_SP_UTF8 "000)");
        case uVC:
            return QString("Micro-Vccoins (VCs) (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        case SAT:
            return QString("Satoshi (VCsat) (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        default:
            return QString("???");
        }
    } else {
        const CoinAsset* pca = GetCoinAsset(CurrentActAssetNo);
        assert(pca != NULL);
        return QString(pca->desc.c_str());
	}
}

qint64 VccoinUnits::factor(int unit)
{
    if (CurrentActAssetNo == 0) {
        switch (unit) {
        case VC:
            return 100000000;
        case mVC:
            return 100000;
        case uVC:
            return 100;
        case SAT:
            return 1;
        default:
            return 100000000;
        }
    } else {
        const CoinAsset* pca = GetCoinAsset(CurrentActAssetNo);
        assert(pca != NULL);
        return pca->coin;   
	}
}

int VccoinUnits::decimals(int unit)
{
    if (CurrentActAssetNo == 0) {
        switch (unit) {
        case VC:
            return 8;
        case mVC:
            return 5;
        case uVC:
            return 2;
        case SAT:
            return 0;
        default:
            return 0;
        }
    } else {
        const CoinAsset* pca = GetCoinAsset(CurrentActAssetNo);
        assert(pca != NULL);
        assert(unit == 0);

		qint64 tmp = 1;
		int i;
        for (i = 0; tmp < pca->coin; i++) {
	        tmp = tmp * 10;
		}

		return i;
	}
}

QString VccoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if (!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    QString quotient_str = QString::number(quotient);

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals > 0) {
        qint64 remainder = n_abs % coin;
        QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');
        return quotient_str + QString(".") + remainder_str;
    } else {
        return quotient_str;
    }
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString VccoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + shortName(unit);
}

QString VccoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool VccoinUnits::parse(int unit, const QString& value, CAmount* val_out)
{
    if (!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if (parts.size() > 2) {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if (parts.size() > 1) {
        decimals = parts[1];
    }
    if (decimals.size() > num_decimals) {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if (str.size() > 18) {
        return false; // Longer numbers will exceed 63 VCs
    }
    CAmount retvalue(str.toLongLong(&ok));
    if (val_out) {
        *val_out = retvalue;
    }
    return ok;
}

QString VccoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (VccoinUnits::valid(unit)) {
        amountTitle += " (" + VccoinUnits::shortName(unit) + ")";
    }
    return amountTitle;
}

int VccoinUnits::rowCount(const QModelIndex& parent) const
{
    const QList<VccoinUnits::Unit> *ptmp_unitlist;
    if(OnlyForFee){
        ptmp_unitlist = &unitlistForFee;
    }else{
        ptmp_unitlist = &unitlist;
    }

    Q_UNUSED(parent);
    if (CurrentActAssetNo == 0) {
        return ptmp_unitlist->size();
    } else {
        return 1;
	}
}

QVariant VccoinUnits::data(const QModelIndex& index, int role) const
{
    const QList<VccoinUnits::Unit> *ptmp_unitlist;
    int tmp = CurrentActAssetNo;

    if(OnlyForFee){
        CurrentActAssetNo = 0;
        ptmp_unitlist = &unitlistForFee;
    }else{
        ptmp_unitlist = &unitlist;
    }

    int row = index.row();
    if (row >= 0 && row < ptmp_unitlist->size()) {
        Unit unit = ptmp_unitlist->at(row);
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(longName(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }

    if(OnlyForFee){
        CurrentActAssetNo = tmp;
    }
    return QVariant();
}

CAmount VccoinUnits::maxMoney()
{
    //return MAX_MONEY;
    const CoinAsset* pca = GetCoinAsset(CurrentActAssetNo);
    assert(pca != NULL);
    return pca->max;
}
