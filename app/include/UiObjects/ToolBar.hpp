//
// Created by Jlisowskyy on 08/10/24.
//

#ifndef APP_TOOLBAR_H
#define APP_TOOLBAR_H

/* internal includes */
#include "../Intf.hpp"

/* external includes */
#include <QObject>
#include <QToolBar>
#include <QGraphicsItem>
#include <QDoubleSpinBox>
#include <QButtonGroup>

/* Forward declaration */

class ToolBar : public QObject {
    Q_OBJECT

    // ------------------------------
    // Class creation
    // ------------------------------

public:
    explicit ToolBar(QObject *parent);

    ~ToolBar() override;

    // ------------------------------
    // Class interaction
    // ------------------------------

    void setupToolBar(QToolBar *toolBar);

    // ------------------------------
    // Class signals
    // ------------------------------
signals:
    void AlgorithmChanged(ALGORITHMS::TYPE type);

    // ------------------------------
    // Class private methods
    // ------------------------------

protected:
    void _addSeparator();

    void _addToolbarLiteral(const char *strLiteral);

    QAction *_addButtonToToolbar(const char *name, const char *imgPath, const char *toolTip);

    QDoubleSpinBox *_addSpinBoxToToolbar(const char *text, const char *toolTip);

protected slots:
    void _onRadioButtonToggled();

    // ------------------------------
    // Public fields
    // ------------------------------
public:
    QToolBar *m_toolBar{};

    /* actions */
    QAction *m_refresh{};
    QAction *m_load{};

    /* Radio buttons */
    QButtonGroup *m_buttonGroup{};

    /* Param inputs */
    QDoubleSpinBox *m_krSpin{};
    QDoubleSpinBox *m_kgSpin{};
    QDoubleSpinBox *m_kbSpin{};
    QDoubleSpinBox *m_kSpin{};
};


#endif //APP_TOOLBAR_H
