//
// Created by Jlisowskyy on 08/10/24.
//

/* external includes */
#include <QDebug>
#include <string>

/* internal includes */
#include "../include/UiObjects/ToolBar.hpp"
#include "../include/UiObjects/TextButton.hpp"

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

}
