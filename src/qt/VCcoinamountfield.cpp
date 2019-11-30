// Copyright (c) 2011-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/VCcoinamountfield.h>

#include <qt/VCcoinunits.h>
#include <qt/guiconstants.h>
#include <qt/qvaluecombobox.h>
#include <qt/coinuint_treemodel.h>

#include <QApplication>
#include <QAbstractSpinBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>

static void ParseIdx(const int idx, int &AssetNo, int &CoinNo)
{
    AssetNo = ((idx & 0xFFFFFFFF) >> 16);
    CoinNo = (idx & 0xFFFF);
}

/** QSpinBox that uses fixed-point numbers internally and uses our own
 * formatting/parsing functions.
 */
class AmountSpinBox: public QAbstractSpinBox
{
    Q_OBJECT

public:
    explicit AmountSpinBox(QWidget *parent):
        QAbstractSpinBox(parent)
    {
        setAlignment(Qt::AlignRight);

        connect(lineEdit(), &QLineEdit::textEdited, this, &AmountSpinBox::valueChanged);
    }

    QValidator::State validate(QString &text, int &pos) const
    {
        if(text.isEmpty())
            return QValidator::Intermediate;
        bool valid = false;
        parse(text, &valid);
        /* Make sure we return Intermediate so that fixup() is called on defocus */
        return valid ? QValidator::Intermediate : QValidator::Invalid;
    }

    void fixup(QString &input) const
    {
        bool valid;
        CAmount val;

        if (input.isEmpty() && !m_allow_empty) {
            valid = true;
            val = m_min_amount;
        } else {
            valid = false;
            val = parse(input, &valid);
        }

        if (valid) {
            int AssetNo, CoinNo;
            ParseIdx(currentUnit, AssetNo, CoinNo);
            VccoinUnits::PushAsset();
            VccoinUnits::SetActiveCoinAsset(AssetNo);

            CAmount tmp_m_max_amount = VccoinUnits::maxMoney();
            val = qBound(m_min_amount, val, tmp_m_max_amount);
            input = VccoinUnits::format(CoinNo, val, false, VccoinUnits::separatorAlways);

            VccoinUnits::PopAsset();

            lineEdit()->setText(input);
        }
    }

    CAmount value(bool *valid_out=nullptr) const
    {
        return parse(text(), valid_out);
    }

    void setValue(const CAmount& value)
    {
        int AssetNo, CoinNo;
        ParseIdx(currentUnit, AssetNo, CoinNo);
        VccoinUnits::PushAsset();
        VccoinUnits::SetActiveCoinAsset(AssetNo);
        lineEdit()->setText(VccoinUnits::format(CoinNo, value, false, VccoinUnits::separatorAlways));
        VccoinUnits::PopAsset();

        Q_EMIT valueChanged();
    }

    void SetAllowEmpty(bool allow)
    {
        m_allow_empty = allow;
    }

    void SetMinValue(const CAmount& value)
    {
        m_min_amount = value;
    }

    void SetMaxValue(const CAmount& value)
    {
        m_max_amount = value;
    }

    void stepBy(int steps)
    {
        bool valid = false;
        CAmount val = value(&valid);
        val = val + steps * singleStep;
        val = qBound(m_min_amount, val, m_max_amount);
        setValue(val);
    }

    void setDisplayUnit(int unit)
    {
        bool valid = false;
        CAmount val = value(&valid);

        int oldAssetNo, curAssetno, CoinNo;
        ParseIdx(currentUnit, oldAssetNo, CoinNo);
        ParseIdx(unit, curAssetno, CoinNo);
        if(oldAssetNo != curAssetno){
            valid = false;
        }

        currentUnit = unit;

        if(valid)
            setValue(val);
        else
            clear();
    }

    void setSingleStep(const CAmount& step)
    {
        singleStep = step;
    }

    QSize minimumSizeHint() const
    {
        if(cachedMinimumSizeHint.isEmpty())
        {
            ensurePolished();

            const QFontMetrics fm(fontMetrics());
            int h = lineEdit()->minimumSizeHint().height();
            int w = fm.width(VccoinUnits::format(VccoinUnits::VC, VccoinUnits::maxMoney(), false, VccoinUnits::separatorAlways));
            w += 2; // cursor blinking space

            QStyleOptionSpinBox opt;
            initStyleOption(&opt);
            QSize hint(w, h);
            QSize extra(35, 6);
            opt.rect.setSize(hint + extra);
            extra += hint - style()->subControlRect(QStyle::CC_SpinBox, &opt,
                                                    QStyle::SC_SpinBoxEditField, this).size();
            // get closer to final result by repeating the calculation
            opt.rect.setSize(hint + extra);
            extra += hint - style()->subControlRect(QStyle::CC_SpinBox, &opt,
                                                    QStyle::SC_SpinBoxEditField, this).size();
            hint += extra;
            hint.setHeight(h);

            opt.rect = rect();

            cachedMinimumSizeHint = style()->sizeFromContents(QStyle::CT_SpinBox, &opt, hint, this)
                                    .expandedTo(QApplication::globalStrut());
        }
        return cachedMinimumSizeHint;
    }

private:
    int currentUnit{VccoinUnits::VC};
    CAmount singleStep{CAmount(100000)}; // satoshis
    mutable QSize cachedMinimumSizeHint;
    bool m_allow_empty{true};
    CAmount m_min_amount{CAmount(0)};
    CAmount m_max_amount{VccoinUnits::maxMoney()};

    /**
     * Parse a string into a number of base monetary units and
     * return validity.
     * @note Must return 0 if !valid.
     */
    CAmount parse(const QString &text, bool *valid_out=nullptr) const
    {
        CAmount val = 0;

        int AssetNo, CoinNo;
        ParseIdx(currentUnit, AssetNo, CoinNo);
        VccoinUnits::PushAsset();
        VccoinUnits::SetActiveCoinAsset(AssetNo);
        bool valid = VccoinUnits::parse(CoinNo, text, &val);
        VccoinUnits::PopAsset();

        if(valid)
        {
            if(val < 0 || val > VccoinUnits::maxMoney())
                valid = false;
        }
        if(valid_out)
            *valid_out = valid;
        return valid ? val : 0;
    }

protected:
    bool event(QEvent *event)
    {
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Comma)
            {
                // Translate a comma into a period
                QKeyEvent periodKeyEvent(event->type(), Qt::Key_Period, keyEvent->modifiers(), ".", keyEvent->isAutoRepeat(), keyEvent->count());
                return QAbstractSpinBox::event(&periodKeyEvent);
            }
        }
        return QAbstractSpinBox::event(event);
    }

    StepEnabled stepEnabled() const
    {
        if (isReadOnly()) // Disable steps when AmountSpinBox is read-only
            return StepNone;
        if (text().isEmpty()) // Allow step-up with empty field
            return StepUpEnabled;

        StepEnabled rv = StepNone;
        bool valid = false;
        CAmount val = value(&valid);
        if (valid) {
            if (val > m_min_amount)
                rv |= StepDownEnabled;
            if (val < m_max_amount)
                rv |= StepUpEnabled;
        }
        return rv;
    }

Q_SIGNALS:
    void valueChanged();
};

#include <qt/VCcoinamountfield.moc>

VccoinAmountField::VccoinAmountField(QWidget *parent) :
    QWidget(parent),
    amount(nullptr),
    ptv(nullptr)
{
    amount = new AmountSpinBox(this);
    amount->setLocale(QLocale::c());
    amount->installEventFilter(this);
    amount->setMaximumWidth(240);

    onlyForFee = false;
    QVariant tmp = parent->property("OnlyForFee");
    if(!tmp.isNull()){
        assert(tmp.type() == QVariant::Bool);
        onlyForFee = tmp.toBool();
    }

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(amount);
    unit = new QValueComboBox(this);
    if(onlyForFee){
        unit->setModel(new VccoinUnits(this, true));
    }else{
        QStringList headers;
        headers << tr("Coin") << tr("Balance");

        CoinUintTreeModel *model = new CoinUintTreeModel(headers);
        model->ReadAllBalanceData();

        ptv = new QTreeView(parent);
        ptv->setModel(model);
        unit->setModel(model);
        unit->setView(ptv);
        ptv->setColumnWidth(0, 130);
        ptv->setColumnWidth(1, 220);
        const_cast<QAbstractItemView*>(unit->view())->setMinimumWidth(ptv->columnWidth(0) + ptv->columnWidth(1) + 30);
        ptv->expandAll();
        ptv->setRootIsDecorated(false);
        ptv->setAlternatingRowColors(true);
        ptv->setStyleSheet("QHeaderView::section { \
                           height:25px; \
                           color:black; \
                           padding-left: 3px;  \
                           background:#D7D7D7; \
                           border-left:0px solid gray;  \
                           border-right:1px solid gray;  \
                           border-top:0px solid gray; \
                           border-bottom:1px solid gray; }");
        ptv->setStyleSheet("QTreeView { \
                           show-decoration-selected:1;  \
                       }");
        ptv->setCurrentIndex(model->index(0,0));
        unit->setModelColumn(0);
        unit->setCurrentIndex(0);
    }
    layout->addWidget(unit);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,6,0);

    setLayout(layout);

    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(amount);

    // If one if the widgets changes, the combined content changes as well
    connect(amount, &AmountSpinBox::valueChanged, this, &VccoinAmountField::valueChanged);
    connect(unit, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VccoinAmountField::unitChanged);

    // Set default based on configuration
    unitChanged(unit->currentIndex());
}

VccoinAmountField::~VccoinAmountField(){
    if(ptv){
        delete ptv;
        ptv = nullptr;
    }
}

void VccoinAmountField::clear()
{
    amount->clear();
    //unit->setModelColumn(0);
    unit->setCurrentIndex(1);
    int tmp = unit->currentIndex();
    //unit->presetIndex(unit->model()->index(0, 0));
}

void VccoinAmountField::setEnabled(bool fEnabled)
{
    amount->setEnabled(fEnabled);
    unit->setEnabled(fEnabled);
}

bool VccoinAmountField::validate()
{
    bool valid = false;
    value(&valid);
    setValid(valid);
    return valid;
}

void VccoinAmountField::setValid(bool valid)
{
    if (valid)
        amount->setStyleSheet("");
    else
        amount->setStyleSheet(STYLE_INVALID);
}

bool VccoinAmountField::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        // Clear invalid flag on focus
        setValid(true);
    }
    return QWidget::eventFilter(object, event);
}

QWidget *VccoinAmountField::setupTabChain(QWidget *prev)
{
    QWidget::setTabOrder(prev, amount);
    QWidget::setTabOrder(amount, unit);
    return unit;
}

CAmount VccoinAmountField::value(bool *valid_out) const
{
    return amount->value(valid_out);
}

int VccoinAmountField::getCurrentAssetNo(void) const
{
    int idx = unit->MyCurrentIndex();
    int AssetNo, tmp;
    ParseIdx(idx, AssetNo, tmp);
    return AssetNo;
}

void VccoinAmountField::setValue(const CAmount& value)
{
    amount->setValue(value);
}

void VccoinAmountField::SetAllowEmpty(bool allow)
{
    amount->SetAllowEmpty(allow);
}

void VccoinAmountField::SetMinValue(const CAmount& value)
{
    amount->SetMinValue(value);
}

void VccoinAmountField::SetMaxValue(const CAmount& value)
{
    amount->SetMaxValue(value);
}

void VccoinAmountField::setReadOnly(bool fReadOnly)
{
    amount->setReadOnly(fReadOnly);
}

void VccoinAmountField::unitChanged(int idx)
{
    //const int AssetNo = ((idx & 0xFFFFFFFF) >> 16);
    //const int CoinNo = (idx & 0xFFFF);
    // Use description tooltip for current unit for the combobox
    unit->setToolTip(unit->itemData(idx, Qt::ToolTipRole).toString());

    // Determine new unit ID
    int newUnit = unit->MyCurrentIndex();   //unit->itemData(idx, VccoinUnits::UnitRole).toInt();

    amount->setDisplayUnit(newUnit);
}

void VccoinAmountField::setBalance(const interfaces::WalletBalances& balances)
{
    CoinUintTreeModel *model = (CoinUintTreeModel*)ptv->model();
    if(model){
        model->ReadAllBalanceData();
        ptv->expandAll();
        ptv->setCurrentIndex(model->index(0,0));
        unit->setModelColumn(0);
        unit->setCurrentIndex(0);
    }
}

void VccoinAmountField::setDisplayUnit(int newUnit)
{
    unit->setValue(newUnit);
}

void VccoinAmountField::setSingleStep(const CAmount& step)
{
    amount->setSingleStep(step);
}
