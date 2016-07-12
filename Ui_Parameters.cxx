#include "Ui_Parameters.h"

void Ui_Parameters::setupUi(QDialog *Parameters)
{
    if (Parameters->objectName().isEmpty())
        Parameters->setObjectName(QStringLiteral("Parameters"));
    Parameters->resize(366, 140);
#ifndef QT_NO_TOOLTIP
    Parameters->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
    Parameters->setStatusTip(QStringLiteral(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
    Parameters->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
    Parameters->setAccessibleName(QStringLiteral("Parameters"));
#endif // QT_NO_ACCESSIBILITY

    buttonBox = new QDialogButtonBox(Parameters);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    buttonBox->setGeometry(QRect(10, 110, 351, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok);

    groupBox = new QGroupBox(Parameters);
    groupBox->setObjectName(QStringLiteral("groupBox"));
    groupBox->setGeometry(QRect(9, 9, 351, 101));

    RaycastSlider = new QSlider(groupBox);
    RaycastSlider->setObjectName(QStringLiteral("RaycastSlider"));
    RaycastSlider->setGeometry(QRect(180, 10, 160, 22));
    RaycastSlider->setMinimum(0);
    RaycastSlider->setMaximum(2);
    RaycastSlider->setPageStep(2);
    RaycastSlider->setValue(*initializer->raycastLevel);
    RaycastSlider->setOrientation(Qt::Horizontal);
    QObject::connect(RaycastSlider,SIGNAL(valueChanged(int)),this,SLOT(RaycastValueChanged()));

    label = new QLabel(groupBox);
    label->setObjectName(QStringLiteral("label"));
    label->setGeometry(QRect(10, 10, 161, 21));
    label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    DetailsCheckBox = new QCheckBox(groupBox);
    DetailsCheckBox->setObjectName(QStringLiteral("DetailsCheckBox"));
    DetailsCheckBox->setGeometry(QRect(180, 40, 21, 21));
    DetailsCheckBox->setChecked(*initializer->eyesAndJaw);
    QObject::connect(DetailsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(DetailsCheckChanged()));

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QStringLiteral("label_2"));
    label_2->setGeometry(QRect(10, 40, 161, 21));
    label_2->setLayoutDirection(Qt::LeftToRight);
    label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    SymCheckBox = new QCheckBox(groupBox);
    SymCheckBox->setObjectName(QStringLiteral("SymCheckBox"));
    SymCheckBox->setGeometry(QRect(180, 70, 21, 21));
    SymCheckBox->setChecked(*initializer->symMode);
    QObject::connect(SymCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SymCheckChanged()));

    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QStringLiteral("label_3"));
    label_3->setGeometry(QRect(10, 70, 161, 21));
    label_3->setLayoutDirection(Qt::LeftToRight);
    label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    retranslateUi(Parameters);
    QObject::connect(buttonBox, SIGNAL(accepted()), Parameters, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), Parameters, SLOT(reject()));

    QMetaObject::connectSlotsByName(Parameters);
} // setupUi

void Ui_Parameters::retranslateUi(QDialog *Parameters)
{
    Parameters->setWindowTitle(QApplication::translate("Parameters", "Parameters", 0));
    groupBox->setTitle(QString());
    label->setText(QApplication::translate("Parameters", "Raycast level (rays per iteration)", 0));
    DetailsCheckBox->setText(QString());
    label_2->setText(QApplication::translate("Parameters", "Eyes and jaw", 0));
    SymCheckBox->setText(QString());
    label_3->setText(QApplication::translate("Parameters", "Symmetrical mode", 0));
} // retranslateUi

void Ui_Parameters::RaycastValueChanged()
{
    *initializer->raycastLevel = RaycastSlider->value();
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->UpdateParameters();
    }
    RefInteractorStyle* refInteractor = dynamic_cast<RefInteractorStyle*>(initializer->content->refInteractor->GetInteractorStyle());
    refInteractor->UpdateParameters();
}

void Ui_Parameters::DetailsCheckChanged()
{
    *initializer->eyesAndJaw = DetailsCheckBox->isChecked();
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->UpdateParameters();
    }
    RefInteractorStyle* refInteractor = dynamic_cast<RefInteractorStyle*>(initializer->content->refInteractor->GetInteractorStyle());
    refInteractor->UpdateParameters();
}

void Ui_Parameters::SymCheckChanged()
{
    *initializer->symMode = SymCheckBox->isChecked();
    if(initializer->newmodelImported)
    {
        InteractorStyle* interactor = dynamic_cast<InteractorStyle*>(initializer->content->style.GetPointer());
        interactor->UpdateParameters();
    }
    RefInteractorStyle* refInteractor = dynamic_cast<RefInteractorStyle*>(initializer->content->refInteractor->GetInteractorStyle());
    refInteractor->UpdateParameters();

}
