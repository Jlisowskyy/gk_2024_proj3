//
// Created by Jlisowskyy on 07/12/24.
//

/* internal includes */
#include "../include/UiObjects/TextSpin.hpp"

/* external includes */
#include <QHBoxLayout>
#include <QLabel>

TextSpin::TextSpin(QWidget *parent, const char *toolTip, const char *title) : QWidget(parent) {
    const auto pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(0);

    auto *pSpinBox = new QDoubleSpinBox(this);
    pSpinBox->setRange(1.0, 5000.0);
    pSpinBox->setSingleStep(1);
    pSpinBox->setDecimals(0.0);
    pSpinBox->setValue(100.0);
    pSpinBox->setToolTip(QString(tr(toolTip)));

    pLayout->addWidget(new QLabel(QString(tr(title)) + QString(tr(":")), this));
    pLayout->addStretch();
    pLayout->addWidget(pSpinBox);

    m_spin = pSpinBox;
}
