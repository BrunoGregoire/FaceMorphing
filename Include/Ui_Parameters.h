#ifndef UI_PARAMETERS_H
#define UI_PARAMETERS_H

#include "qtIncludes.h"

#include "Initializer.h"

class Ui_Parameters : QObject
{
    Q_OBJECT

public:
    Initializer* initializer;

    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QSlider *RaycastSlider;
    QLabel *label;
    QCheckBox *DetailsCheckBox;
    QLabel *label_2;
    QCheckBox *SymCheckBox;
    QLabel *label_3;

    void setupUi(QDialog *Parameters);
    void retranslateUi(QDialog *Parameters);

public slots:

    void RaycastValueChanged();
    void DetailsCheckChanged();
    void SymCheckChanged();
};

#endif //UI_PARAMETERS_H
