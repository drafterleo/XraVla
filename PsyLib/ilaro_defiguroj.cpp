#include "ilaro_defiguroj.h"
#include <QHBoxLayout>

KIlaroDeFiguroj::KIlaroDeFiguroj(QWidget *parent) :
    KIlaroAbstrakta(parent)
{
    m_rektanguloBtn = new QPushButton("+4R");
    connect(m_rektanguloBtn, SIGNAL(clicked()), SIGNAL(necesasAldoniRektangulon()));
    m_kvaranguloBtn = new QPushButton("+4");
    connect(m_kvaranguloBtn, SIGNAL(clicked()), SIGNAL(necesasAldoniKvarangulon()));
    m_kvinanguloBtn = new QPushButton("+5");
    connect(m_kvinanguloBtn, SIGNAL(clicked()), SIGNAL(necesasAldoniKvinangulon()));
    m_sesanguloBtn = new QPushButton("+6");
    connect(m_sesanguloBtn, SIGNAL(clicked()), SIGNAL(necesasAldoniSesangulon()));

    m_eliminiFiguronBtn = new QPushButton("-F");
    connect(m_eliminiFiguronBtn, SIGNAL(clicked()), SIGNAL(necesasEliminiFiguron()));

    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->addWidget(m_rektanguloBtn);
    hbLayout->addWidget(m_kvaranguloBtn);
    hbLayout->addWidget(m_kvinanguloBtn);
    hbLayout->addWidget(m_sesanguloBtn);
    hbLayout->addWidget(m_eliminiFiguronBtn);
    hbLayout->addStretch(1);

    this->setLayout(hbLayout);
}
