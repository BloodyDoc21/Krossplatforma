#include "formwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QVBoxLayout>

FormWindow::FormWindow(QWidget *parent)
    : QWidget(parent)
    , m_name("Имя")
    , m_email("Email")
    , m_age("Возраст")
    , m_country(nullptr)
    , m_description("Описание")
    , m_agree(nullptr)
    , m_rootLayout(nullptr)
    , m_formLayout(nullptr)
    , m_descriptionExpanding(true)
    , m_ageExpanding(false)
{
    setupUi();
    setComfortableMode();

    qDebug() << "[INIT] окно создано; растяните его и смотрите"
             << "[RESIZE] в логе";
}

FormWindow::~FormWindow()
{
    qDebug() << "[DESTROY] FormWindow";
}

void FormWindow::setupUi()
{
    setWindowTitle(
        "Адаптивная форма – QSizePolicy / addStretch / margins");
    resize(560, 620);

    m_rootLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("<b>Анкета</b>", this);
    m_rootLayout->addWidget(title);

    m_formLayout = new QFormLayout();
    m_formLayout->setContentsMargins(0, 0, 0, 0);
    m_formLayout->setSpacing(8);
    m_rootLayout->addLayout(m_formLayout);

    m_name.setHorizontalPolicy(QSizePolicy::Expanding);
    m_name.setVerticalPolicy(QSizePolicy::Fixed);
    m_name.setPlaceholderText("Иван Иванов");
    m_formLayout->addRow("&Имя:", &m_name);

    m_email.setHorizontalPolicy(QSizePolicy::Expanding);
    m_email.setVerticalPolicy(QSizePolicy::Fixed);
    m_email.setPlaceholderText("ivan@example.com");
    m_formLayout->addRow("&Email:", &m_email);

    m_age.setHorizontalPolicy(QSizePolicy::Fixed);
    m_age.setVerticalPolicy(QSizePolicy::Fixed);
    m_age.setPlaceholderText("лет");
    m_age.setValidator(new QIntValidator(0, 130, &m_age));
    QHBoxLayout *ageRow = new QHBoxLayout();
    ageRow->setContentsMargins(0, 0, 0, 0);
    ageRow->addWidget(&m_age);
    ageRow->addStretch();
    QLabel *ageLabel = new QLabel("&Возраст:", this);
    ageLabel->setBuddy(&m_age);
    m_formLayout->addRow(ageLabel, ageRow);

    m_country = new QComboBox(this);
    m_country->addItems({"Россия", "Беларусь", "Казахстан", "Другая"});
    m_country->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_formLayout->addRow("&Страна:", m_country);

    m_description.setHorizontalPolicy(QSizePolicy::Expanding);
    m_description.setVerticalPolicy(QSizePolicy::Expanding);
    m_description.setPlaceholderText("Расскажите о себе…");
    m_formLayout->addRow("&Описание:", &m_description);

    m_agree = new QCheckBox("Я согласен с условиями", this);
    m_agree->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_formLayout->addRow(QString(), m_agree);

    m_trackedFields = { &m_name, &m_email, &m_age, m_country,
                        &m_description, m_agree };
    m_trackedNames = { "Имя", "Email", "Возраст", "Страна",
                       "Описание", "Согласие" };

    QHBoxLayout *modeRow = new QHBoxLayout();
    QPushButton *btnCompact = new QPushButton("Компактный", this);
    QPushButton *btnComfort = new QPushButton("Развёрнутый", this);
    QPushButton *btnDesc =
        new QPushButton("Описание: тянуть/фикс.", this);
    QPushButton *btnAge = new QPushButton("Возраст: фикс./тянуть", this);
    QPushButton *btnPol = new QPushButton("Дамп политик", this);
    QPushButton *btnSize = new QPushButton("Дамп размеров", this);
    modeRow->addWidget(btnCompact);
    modeRow->addWidget(btnComfort);
    modeRow->addWidget(btnDesc);
    modeRow->addWidget(btnAge);
    modeRow->addWidget(btnPol);
    modeRow->addWidget(btnSize);
    modeRow->addStretch();
    m_rootLayout->addLayout(modeRow);

    QHBoxLayout *actionRow = new QHBoxLayout();
    QPushButton *btnOk = new QPushButton("ОК", this);
    QPushButton *btnCancel = new QPushButton("Отмена", this);
    actionRow->addStretch();
    actionRow->addWidget(btnOk);
    actionRow->addWidget(btnCancel);
    m_rootLayout->addLayout(actionRow);

    connect(btnCompact, &QPushButton::clicked,
            this, &FormWindow::setCompactMode);
    connect(btnComfort, &QPushButton::clicked,
            this, &FormWindow::setComfortableMode);
    connect(btnDesc, &QPushButton::clicked,
            this, &FormWindow::toggleDescriptionPolicy);
    connect(btnAge, &QPushButton::clicked,
            this, &FormWindow::toggleAgePolicy);
    connect(btnPol, &QPushButton::clicked,
            this, &FormWindow::dumpSizePolicies);
    connect(btnSize, &QPushButton::clicked,
            this, &FormWindow::dumpSizes);
    connect(btnOk, &QPushButton::clicked,
            this, &FormWindow::onAccept);
    connect(btnCancel, &QPushButton::clicked,
            this, &FormWindow::onReject);
}

void FormWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << "[RESIZE] окно ->" << event->size().width() << "x"
             << event->size().height()
             << "| описание:" << m_description.width() << "x"
             << m_description.height();
}

void FormWindow::setCompactMode()
{
    m_rootLayout->setContentsMargins(8, 8, 8, 8);
    m_rootLayout->setSpacing(4);
    qDebug() << "[MARGIN] компактный: margins=8, spacing=4";
}

void FormWindow::setComfortableMode()
{
    m_rootLayout->setContentsMargins(24, 24, 24, 24);
    m_rootLayout->setSpacing(12);
    qDebug() << "[MARGIN] развёрнутый: margins=24, spacing=12";
}

void FormWindow::toggleDescriptionPolicy()
{
    m_descriptionExpanding = !m_descriptionExpanding;
    if (m_descriptionExpanding) {
        m_description.setVerticalPolicy(QSizePolicy::Expanding);
    } else {
        m_description.setVerticalPolicy(QSizePolicy::Fixed);
    }
    qDebug() << "[TOGGLE] описание вертикально ->"
             << (m_descriptionExpanding ? "Expanding" : "Fixed");
}

void FormWindow::toggleAgePolicy()
{
    m_ageExpanding = !m_ageExpanding;
    if (m_ageExpanding) {
        m_age.setHorizontalPolicy(QSizePolicy::Expanding);
    } else {
        m_age.setHorizontalPolicy(QSizePolicy::Fixed);
    }
    qDebug() << "[TOGGLE] возраст горизонтально ->"
             << (m_ageExpanding ? "Expanding" : "Fixed");
}

QString FormWindow::policyName(QSizePolicy::Policy p)
{
    switch (p) {
    case QSizePolicy::Fixed:
        return "Fixed";
    case QSizePolicy::Minimum:
        return "Minimum";
    case QSizePolicy::Maximum:
        return "Maximum";
    case QSizePolicy::Preferred:
        return "Preferred";
    case QSizePolicy::Expanding:
        return "Expanding";
    case QSizePolicy::MinimumExpanding:
        return "MinimumExpanding";
    case QSizePolicy::Ignored:
        return "Ignored";
    }
    return "Other";
}

void FormWindow::dumpSizePolicies()
{
    qDebug() << "===== ДАМП ПОЛИТИК =====";
    for (int i = 0; i < m_trackedFields.size(); i++) {
        QWidget *w = m_trackedFields[i];
        QSizePolicy sp = w->sizePolicy();
        qDebug().noquote()
            << " " << m_trackedNames[i]
            << "(" + QString(w->metaObject()->className()) + ")"
            << "horiz=" + policyName(sp.horizontalPolicy())
            << "vert=" + policyName(sp.verticalPolicy());
    }
    qDebug() << "========================";
}

void FormWindow::dumpSizes()
{
    qDebug() << "===== ДАМП РАЗМЕРОВ (текущий | sizeHint | minHint) =====";
    for (int i = 0; i < m_trackedFields.size(); i++) {
        QWidget *w = m_trackedFields[i];
        QSize hint = w->sizeHint();
        QSize minHint = w->minimumSizeHint();
        qDebug().noquote()
            << " " << m_trackedNames[i]
            << QString("cur=%1x%2").arg(w->width()).arg(w->height())
            << QString("hint=%1x%2").arg(hint.width()).arg(hint.height())
            << QString("min=%1x%2").arg(minHint.width())
                   .arg(minHint.height());
    }
    qDebug() << "========================================================";
}

void FormWindow::onAccept()
{
    qDebug() << "[ACTION] ОК: имя =" << m_name.text()
             << "email =" << m_email.text()
             << "возраст =" << m_age.text()
             << "страна =" << m_country->currentText()
             << "согласие =" << m_agree->isChecked();
    close();
}

void FormWindow::onReject()
{
    qDebug() << "[ACTION] Отмена";
    close();
}
