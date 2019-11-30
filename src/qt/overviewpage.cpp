// Copyright (c) 2011-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/overviewpage.h>
#include <qt/forms/ui_overviewpage.h>

#include <qt/VCcoinunits.h>
#include <qt/clientmodel.h>
#include <qt/guiconstants.h>
#include <qt/guiutil.h>
#include <qt/optionsmodel.h>
#include <qt/platformstyle.h>
#include <qt/transactionfilterproxy.h>
#include <qt/transactiontablemodel.h>
#include <qt/walletmodel.h>

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlError>

#define DECORATION_SIZE 54
#define NUM_ITEMS 5

Q_DECLARE_METATYPE(interfaces::WalletBalances)

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit TxViewDelegate(const PlatformStyle* _platformStyle, QObject* parent = nullptr) : QAbstractItemDelegate(parent), unit(VccoinUnits::VC),
                                                                                              platformStyle(_platformStyle)
    {
    }

    inline void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        painter->save();

        VccoinUnits::AutoAssetStack dummy;
        int AssetNo = index.data(TransactionTableModel::CoinAssetNo).toInt();
        VccoinUnits::SetActiveCoinAsset(AssetNo);

        QIcon icon = qvariant_cast<QIcon>(index.data(TransactionTableModel::RawDecorationRole));
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2*ypad)/2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top()+ypad, mainRect.width() - xspace, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top()+ypad+halfheight, mainRect.width() - xspace, halfheight);
        QRect feeRect(mainRect.left() + xspace, mainRect.top()+ypad+halfheight, mainRect.width() - xspace, halfheight);
        icon = platformStyle->SingleColorIcon(icon);
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        qint64 fee = index.data(TransactionTableModel::FeeRole).toLongLong();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();
        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = option.palette.color(QPalette::Text);
        if (value.canConvert<QBrush>()) {
            QBrush brush = qvariant_cast<QBrush>(value);
            foreground = brush.color();
        }

        painter->setPen(foreground);
        QRect boundingRect;
        painter->drawText(addressRect, Qt::AlignLeft|Qt::AlignVCenter, address, &boundingRect);

        if (index.data(TransactionTableModel::WatchonlyRole).toBool()) {
            QIcon iconWatchonly = qvariant_cast<QIcon>(index.data(TransactionTableModel::WatchonlyDecorationRole));
            QRect watchonlyRect(boundingRect.right() + 5, mainRect.top()+ypad+halfheight, 16, halfheight);
            iconWatchonly.paint(painter, watchonlyRect);
        }

        if (amount < 0) {
            foreground = COLOR_NEGATIVE;
        } else if (!confirmed) {
            foreground = COLOR_UNCONFIRMED;
        } else {
            foreground = option.palette.color(QPalette::Text);
        }
        painter->setPen(foreground);
        QString amountText = VccoinUnits::formatWithUnit(unit, amount, true, VccoinUnits::separatorAlways);
        if (!confirmed) {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight | Qt::AlignVCenter, amountText);

        painter->setPen(option.palette.color(QPalette::Text));
        painter->drawText(amountRect, Qt::AlignLeft | Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        if (AssetNo != 0 && fee != 0) {
            if (fee < 0) {
                foreground = COLOR_NEGATIVE;
            } else if (!confirmed) {
                foreground = COLOR_UNCONFIRMED;
            } else {
                foreground = option.palette.color(QPalette::Text);
            }

            VccoinUnits::PushAsset();
            VccoinUnits::SetActiveCoinAsset();

            painter->setPen(foreground);
            QString feeText = VccoinUnits::formatWithUnit(unit, -fee, true, VccoinUnits::separatorAlways);
            if (!confirmed) {
                feeText = QString("[") + feeText + QString("]");
            }
            painter->drawText(feeRect, Qt::AlignRight | Qt::AlignVCenter, feeText);

            VccoinUnits::PopAsset();
        }

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;
    const PlatformStyle* platformStyle;
};
#include <QtQml/QQmlContext>
#include "assetitem.h"
#include "assetmodel.h"
#include <qt/overviewpage.moc>
#include <qt/utilitydialog.h>

bool IsMainAssetOwner();

OverviewPage::OverviewPage(const PlatformStyle* platformStyle, QWidget* parent) : QWidget(parent),
                                                                                  QmlContainer(nullptr),
                                                                                  pQuickView(nullptr),
                                                                                  ui(new Ui::OverviewPage),
                                                                                  clientModel(nullptr),
                                                                                  walletModel(nullptr),
                                                                                  txdelegate(new TxViewDelegate(platformStyle, this)),
                                                                                  pQmlAssetModal(nullptr)
{
    //qmlRegisterType<AssetModel>("Backend", 1, 0, "AssetModel");
    pQmlAssetModal = new AssetModel();

    ui->setupUi(this);

    m_balances.balance = -1;

    // use a SingleColorIcon for the "out of sync warning" icon
    QIcon icon = platformStyle->SingleColorIcon(":/icons/warning");
    icon.addPixmap(icon.pixmap(QSize(64, 64), QIcon::Normal), QIcon::Disabled); // also set the disabled icon because we are using a disabled QPushButton to work around missing HiDPI support of QLabel (https://bugreports.qt.io/browse/QTBUG-42503)
    ui->labelTransactionsStatus->setIcon(icon);
    ui->labelWalletStatus->setIcon(icon);

    // Recent transactions
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, &QListView::clicked, this, &OverviewPage::handleTransactionClicked);

    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
    connect(ui->labelWalletStatus, &QPushButton::clicked, this, &OverviewPage::handleOutOfSyncWarningClicks);
    connect(ui->labelTransactionsStatus, &QPushButton::clicked, this, &OverviewPage::handleOutOfSyncWarningClicks);

    for (int i = 0; i < ui->gridLayout->count(); i++) {
        QWidget* pw = ui->gridLayout->itemAt(i)->widget();
        if (pw) {
            pw->hide();
        }
    }
    ui->frame->layout()->removeItem(ui->gridLayout);

    pQuickView = new QQuickView();
    QmlContainer = QWidget::createWindowContainer(pQuickView, this);
    QmlContainer->setMinimumSize(250, 280);
    QmlContainer->setMaximumSize(1100, 1100);
    QmlContainer->setFocusPolicy(Qt::TabFocus);
    pQuickView->setResizeMode(QQuickView::SizeRootObjectToView);
    pQuickView->rootContext()->setContextProperty("AssetListModal", pQmlAssetModal);
	pQuickView->setSource(QUrl("qrc:/icons/AssetList.qml"));	
    QString err;
    QList<QQmlError> errs = pQuickView->errors();
    for (int i = 0; i < errs.size(); i++) {
        err = errs[i].description();
        QMessageBox::critical(nullptr, PACKAGE_NAME, err);
    }

    pQuickView->update();

    if (IsMainAssetOwner()) {
        NotifyMainAssetFound();
    }

    ui->frame->layout()->addWidget(QmlContainer);
    if (platformStyle->GetModaloverlayViewActive()) {
        QmlContainer->hide();
    }
}

void OverviewPage::NotifyModalOverlayActived(bool IsActive)
{
    if (IsActive) {
    } else {
        if (QmlContainer) {
            QmlContainer->show();
        }
    }
}

void OverviewPage::handleTransactionClicked(const QModelIndex& index)
{
    if (filter)
        Q_EMIT transactionClicked(filter->mapToSource(index));
}

void OverviewPage::handleOutOfSyncWarningClicks()
{
    if (QmlContainer) {
        QmlContainer->hide();
    }
    Q_EMIT outOfSyncWarningClicked();
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

void OverviewPage::setBalance(const interfaces::WalletBalances& balances)
{
    setAssetBalances();
    return;

    int unit = walletModel->getOptionsModel()->getDisplayUnit();
    m_balances = balances;
    if (walletModel->privateKeysDisabled()) {
        ui->labelBalance->setText(VccoinUnits::formatWithUnit(unit, balances.watch_only_balance, false, VccoinUnits::separatorAlways));
        ui->labelUnconfirmed->setText(VccoinUnits::formatWithUnit(unit, balances.unconfirmed_watch_only_balance, false, VccoinUnits::separatorAlways));
        ui->labelImmature->setText(VccoinUnits::formatWithUnit(unit, balances.immature_watch_only_balance, false, VccoinUnits::separatorAlways));
        ui->labelTotal->setText(VccoinUnits::formatWithUnit(unit, balances.watch_only_balance + balances.unconfirmed_watch_only_balance + balances.immature_watch_only_balance, false, VccoinUnits::separatorAlways));
    } else {
        ui->labelBalance->setText(VccoinUnits::formatWithUnit(unit, balances.balance, false, VccoinUnits::separatorAlways));
        ui->labelUnconfirmed->setText(VccoinUnits::formatWithUnit(unit, balances.unconfirmed_balance, false, VccoinUnits::separatorAlways));
        ui->labelImmature->setText(VccoinUnits::formatWithUnit(unit, balances.immature_balance, false, VccoinUnits::separatorAlways));
        ui->labelTotal->setText(VccoinUnits::formatWithUnit(unit, balances.balance + balances.unconfirmed_balance + balances.immature_balance, false, VccoinUnits::separatorAlways));
        ui->labelWatchAvailable->setText(VccoinUnits::formatWithUnit(unit, balances.watch_only_balance, false, VccoinUnits::separatorAlways));
        ui->labelWatchPending->setText(VccoinUnits::formatWithUnit(unit, balances.unconfirmed_watch_only_balance, false, VccoinUnits::separatorAlways));
        ui->labelWatchImmature->setText(VccoinUnits::formatWithUnit(unit, balances.immature_watch_only_balance, false, VccoinUnits::separatorAlways));
        ui->labelWatchTotal->setText(VccoinUnits::formatWithUnit(unit, balances.watch_only_balance + balances.unconfirmed_watch_only_balance + balances.immature_watch_only_balance, false, VccoinUnits::separatorAlways));
    }
    // only show immature (newly mined) balance if it's non-zero, so as not to complicate things
    // for the non-mining users
    bool showImmature = balances.immature_balance != 0;
    bool showWatchOnlyImmature = balances.immature_watch_only_balance != 0;

    // for symmetry reasons also show immature label when the watch-only one is shown
    ui->labelImmature->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelImmatureText->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelWatchImmature->setVisible(!walletModel->privateKeysDisabled() && showWatchOnlyImmature); // show watch-only immature balance
}

void OverviewPage::NotifyMainAssetFound(void)
{
    do {
        if (pQuickView->status() == QQuickView::Ready || pQuickView->status() == QQuickView::Error) {
            break;
        }

    } while (1);

    QObject* pRoundButton = pQuickView->findChild<QObject*>("createAsset");
    if (pRoundButton) {
        pRoundButton->setProperty("visible", "true");
    }

    pQmlAssetModal->UpdateMainWalletStatus(true);
}

void OverviewPage::NotifyAssetStatusChanged(const int AssetNo)
{
    pQmlAssetModal->UpdateAssetStatus(AssetNo);
}

#include <sstream>
#include <string>
std::shared_ptr<CWallet> GetAssetWallet(const int AssetNo);
void OverviewPage::setAssetBalances(void)
{
    VccoinUnits::Reset();

    while (pQmlAssetModal->rowCount() != 0) {
        pQmlAssetModal->remove(0);
    }

    bool MainWallet = IsMainAssetOwner();
    std::vector<CoinAsset> cas;
    CoinAssetManager::Instance().GetAllAssets(cas);
    for (auto ca : cas) {
        std::shared_ptr<CWallet> pWallet = ::GetAssetWallet(ca.no);
        std::unique_ptr<interfaces::Wallet> pInfWallet = interfaces::MakeWallet(pWallet);
        if (pInfWallet == nullptr) {
            continue;
        }
        interfaces::WalletBalances balances = pInfWallet->getBalances();
        if (ca.no != 0 && !IsMainAssetOwner()) {
            if (balances.balance == 0 && balances.unconfirmed_balance == 0) {
                continue;
            }
        }
        
        VccoinUnits::SetActiveCoinAsset(ca.no);
        std::stringstream strs;
        //strs << ca.name.c_str() << "(" << ca.no << ")";
        strs << ca.name.c_str();
        QString name = strs.str().c_str();

        int unit = walletModel->getOptionsModel()->getDisplayUnit();
        QString Balance = VccoinUnits::formatWithUnit(unit, balances.balance, false, VccoinUnits::separatorAlways);
        QString UnconfirmedBalance = VccoinUnits::formatWithUnit(unit, balances.unconfirmed_balance, false, VccoinUnits::separatorAlways);
        QString TotalBalance = VccoinUnits::formatWithUnit(unit, balances.balance + balances.unconfirmed_balance + balances.immature_balance, false, VccoinUnits::separatorAlways);

        pQmlAssetModal->append(ca.no, name, Balance, UnconfirmedBalance, TotalBalance, ca.IsLock(), MainWallet );
    }

    VccoinUnits::SetActiveCoinAsset();
}

// show/hide watch-only labels
void OverviewPage::updateWatchOnlyLabels(bool showWatchOnly)
{
    ui->labelSpendable->setVisible(showWatchOnly);      // show spendable label (only when watch-only is active)
    ui->labelWatchonly->setVisible(showWatchOnly);      // show watch-only label
    ui->lineWatchBalance->setVisible(showWatchOnly);    // show watch-only balance separator line
    ui->labelWatchAvailable->setVisible(showWatchOnly); // show watch-only available balance
    ui->labelWatchPending->setVisible(showWatchOnly);   // show watch-only pending balance
    ui->labelWatchTotal->setVisible(showWatchOnly);     // show watch-only total balance

    if (!showWatchOnly)
        ui->labelWatchImmature->hide();
}

void OverviewPage::setClientModel(ClientModel* model)
{
    this->clientModel = model;
    if (model) {
        // Show warning if this is a prerelease version
        connect(model, &ClientModel::alertsChanged, this, &OverviewPage::updateAlerts);
        updateAlerts(model->getStatusBarWarnings());
    }
}

void OverviewPage::setWalletModel(WalletModel* model)
{
    this->walletModel = model;
    if (model && model->getOptionsModel()) {
        // Set up transaction list
        filter.reset(new TransactionFilterProxy());
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->setShowInactive(false);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter.get());
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        interfaces::Wallet& wallet = model->wallet();
        interfaces::WalletBalances balances = wallet.getBalances();
        setBalance(balances);
        connect(model, &WalletModel::balanceChanged, this, &OverviewPage::setBalance);

        connect(model->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &OverviewPage::updateDisplayUnit);

        updateWatchOnlyLabels(wallet.haveWatchOnly() && !model->privateKeysDisabled());
        connect(model, &WalletModel::notifyWatchonlyChanged, [this](bool showWatchOnly) {
            updateWatchOnlyLabels(showWatchOnly && !walletModel->privateKeysDisabled());
        });
    }

    // update the display unit, to not use the default ("VC")
    updateDisplayUnit();
}

void OverviewPage::updateDisplayUnit()
{
    if (walletModel && walletModel->getOptionsModel()) {
        if (m_balances.balance != -1) {
            setBalance(m_balances);
        }

        // Update txdelegate->unit with the current unit
        txdelegate->unit = walletModel->getOptionsModel()->getDisplayUnit();

        ui->listTransactions->update();
    }
}

void OverviewPage::updateAlerts(const QString& warnings)
{
    this->ui->labelAlerts->setVisible(false); //!warnings.isEmpty());
    this->ui->labelAlerts->setText("");       //warnings);
}

void OverviewPage::showOutOfSyncWarning(bool fShow)
{
    ui->labelWalletStatus->setVisible(fShow);
    ui->labelTransactionsStatus->setVisible(fShow);
}
