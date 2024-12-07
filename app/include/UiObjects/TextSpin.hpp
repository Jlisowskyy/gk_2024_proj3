//
// Created by Jlisowskyy on 07/12/24.
//

#ifndef APP_TEXTSPIN_H
#define APP_TEXTSPIN_H

#include <QWidget>
#include <QDoubleSpinBox>

class TextSpin : public QWidget {
Q_OBJECT

public:
    // ------------------------------
    // Class creation
    // ------------------------------

    explicit TextSpin(QWidget *parent, const char *toolTip, const char *title);

    ~TextSpin() override = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    [[nodiscard]] QDoubleSpinBox *getSpin() const { return m_spin; }

    // ------------------------------
    // Class fields
    // ------------------------------
protected:
    QDoubleSpinBox *m_spin{};
};

#endif //APP_TEXTSPIN_H
