//
// Created by Jlisowskyy on 08/10/24.
//

/* external includes */
#include <QDebug>
#include <string>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDebug>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"
#include "../include/UiObjects/TextButton.hpp"
#include "../include/UiObjects/TextSpin.hpp"

ToolBar::ToolBar(QObject *parent) : QObject(parent) {
}

ToolBar::~ToolBar() = default;

void ToolBar::_addToolbarLiteral(const char *strLiteral) {
    const auto pliteral = new QAction(tr(strLiteral), m_toolBar);
    pliteral->setDisabled(true);
    m_toolBar->addAction(pliteral);
}

QAction *
ToolBar::_addButtonToToolbar(const char *name, const char *imgPath, const char *toolTip) {
    const auto pButton = new QAction(tr(name), m_toolBar);
    pButton->setIcon(QIcon(imgPath));
    pButton->setToolTip(tr(toolTip));
    m_toolBar->addAction(pButton);

    return pButton;
}

void ToolBar::_addSeparator() {
    const auto pLiteral = new QAction(m_toolBar);
    pLiteral->setSeparator(true);
    m_toolBar->addAction(pLiteral);
}

void ToolBar::setupToolBar(QToolBar *toolBar) {
    Q_ASSERT(toolBar != nullptr);
    Q_ASSERT(m_toolBar == nullptr);
    m_toolBar = toolBar;

    _addSeparator();
    _addToolbarLiteral("Algorithms");

    auto *pButtonGroup = new QButtonGroup(m_toolBar);
    for (uint32_t algoType = 0; algoType < ALGORITHMS::TYPE::LAST; ++algoType) {
        auto *pRadioButton = new QRadioButton(ALGORITHMS::DESCRIPTIONS[algoType]);

        if (algoType == ALGORITHMS::DEFAULT_SELECTION) {
            pRadioButton->setChecked(true);
        }

        pRadioButton->setProperty(UI_CONSTANTS::DEFAULT_BUTTON_PAYLOAD, QVariant::fromValue(algoType));

        connect(pRadioButton, &QRadioButton::toggled, this, &ToolBar::_onRadioButtonToggled);

        pButtonGroup->addButton(pRadioButton);
        m_toolBar->addWidget(pRadioButton);
    }

    _addSeparator();
    _addToolbarLiteral("Dithering Params");

    m_krSpin = _addSpinBoxToToolbar("Kr", "Provide input for Kr param");
    m_krSpin->setValue(4.0);
    m_kgSpin = _addSpinBoxToToolbar("Kg", "Provide input for Kg param");
    m_kgSpin->setValue(4.0);
    m_kbSpin = _addSpinBoxToToolbar("Kb", "Provide input for Kb param");
    m_kbSpin->setValue(4.0);

    _addSeparator();
    _addToolbarLiteral("Popularity algorithm params");

    m_kSpin = _addSpinBoxToToolbar("K", "Provide input for K param");

    _addSeparator();
    _addToolbarLiteral("Actions");

    auto pButton = new TextButton(m_toolBar, "Click here to process the image", "Refresh image", ":icons/refresh.png");
    m_refresh = pButton->getAction();
    m_toolBar->addWidget(pButton);

    pButton = new TextButton(m_toolBar, "Click here to load new image from a disk", "Load image", ":icons/load.png");
    m_load = pButton->getAction();
    m_toolBar->addWidget(pButton);

    pButton = new TextButton(m_toolBar, "Click here to save new image to a disk", "Save image", ":icons/load.png");
    m_save = pButton->getAction();
    m_toolBar->addWidget(pButton);

    pButton = new TextButton(m_toolBar, "Click here to procedurally generate image", "Generate image", ":icons/load.png");
    m_generate = pButton->getAction();
    m_toolBar->addWidget(pButton);

    _addSeparator();
}

void ToolBar::_onRadioButtonToggled() {
    if (sender() == nullptr) {
        return;
    }

    auto *pSelectedButton = qobject_cast<QRadioButton *>(sender());

    if (pSelectedButton && pSelectedButton->isChecked()) {
        uint32_t algoType = pSelectedButton->property(UI_CONSTANTS::DEFAULT_BUTTON_PAYLOAD).toUInt();
        emit AlgorithmChanged(static_cast<ALGORITHMS::TYPE>(algoType));

        qDebug() << "SELECTED: " << ALGORITHMS::DESCRIPTIONS[algoType];
    }
}

QDoubleSpinBox *ToolBar::_addSpinBoxToToolbar(const char *text, const char *toolTip) {
    auto *pSpin = new TextSpin(m_toolBar, toolTip, text);
    m_toolBar->addWidget(pSpin);
    return pSpin->getSpin();
}
