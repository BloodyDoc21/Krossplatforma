
#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle("Qt Object Tree Demo");
    resize(520, 640);

    m_rootLayout = new QVBoxLayout(this);

    m_title = new LogLabel(
        "Дерево объектов Qt: владелец и потомки", this);
    m_rootLayout->addWidget(m_title.data());

    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText("Виджет-ребёнок главного окна");
    m_rootLayout->addWidget(m_input.data());

    m_status = new LogLabel("Статус: окно создано", this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    m_panel = new LogPanel("Панель-владелец", this);
    m_rootLayout->addWidget(m_panel.data());

    m_panelLayout = new QVBoxLayout(m_panel.data());

    LogButton *staticButton =
        new LogButton("Статическая кнопка внутри панели",
                      m_panel.data());
    m_panelLayout->addWidget(staticButton);

    m_movable = new LogButton(
        "Перемещаемый виджет (в панели)", m_panel.data());
    m_panelLayout->addWidget(m_movable.data());

    m_moveButton = new LogButton(
        "Переместить виджет: панель <-> окно", this);
    connect(m_moveButton.data(), &QPushButton::clicked,
            this, &MainWindow::moveMovableWidget);
    m_rootLayout->addWidget(m_moveButton.data());

    m_deleteButton = new LogButton(
        "Удалить панель (deleteLater)", this);
    connect(m_deleteButton.data(), &QPushButton::clicked,
            this, &MainWindow::deletePanel);
    m_rootLayout->addWidget(m_deleteButton.data());

    LogButton *detachButton =
        new LogButton("Демо setParent(nullptr)", this);
    connect(detachButton, &QPushButton::clicked,
            this, &MainWindow::demoDetaching);
    m_rootLayout->addWidget(detachButton);

    m_rootLayout->addStretch();

    setStatus(
        "Готово. Все виджеты созданы с родителями. "
        "Удаление родителя автоматически удалит детей.");
}

void MainWindow::setStatus(const QString &text)
{
    if (!m_status.isNull()) {
        m_status->setText(text);
    }
}

void MainWindow::moveMovableWidget()
{
    if (m_movable.isNull()) {
        setStatus("Перемещаемый виджет уже уничтожен.");
        return;
    }

    if (m_panel.isNull()) {
        setStatus("Панель уже удалена. Вернуть виджет в неё невозможно.");
        return;
    }

    if (m_movable->parentWidget() == m_panel.data()) {
        if (!m_panelLayout.isNull()) {
            m_panelLayout->removeWidget(m_movable.data());
        }

        m_movable->setParent(this);

        if (!m_rootLayout.isNull()) {
            m_rootLayout->addWidget(m_movable.data());
        }

        m_movable->setText("Перемещаемый виджет (вне панели)");

        setStatus(
            "Главное окно стало новым владельцем виджета. "
            "Панель больше не удалит его автоматически.");
    } else {
        if (!m_rootLayout.isNull()) {
            m_rootLayout->removeWidget(m_movable.data());
        }

        m_movable->setParent(m_panel.data());

        if (!m_panelLayout.isNull()) {
            m_panelLayout->addWidget(m_movable.data());
        }

        m_movable->setText("Перемещаемый виджет (в панели)");

        setStatus(
            "Виджет снова стал ребёнком панели. "
            "Теперь он будет удалён вместе с панелью.");
    }

    m_movable->show();

    qDebug() << "[SET_PARENT] Новый владелец виджета:"
             << m_movable->parentWidget()->objectName();
}

void MainWindow::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }

    if (!m_movable.isNull()
        && m_movable->parentWidget() == m_panel.data()) {
        setStatus(
            "Панель будет удалена. Перемещаемый виджет является "
            "её ребёнком и будет уничтожен автоматически.");
    } else {
        setStatus(
            "Панель будет удалена. Перемещаемый виджет "
            "не является её ребёнком и останется жив.");
    }

    if (!m_deleteButton.isNull()) {
        m_deleteButton->setEnabled(false);
    }

    if (!m_moveButton.isNull()) {
        m_moveButton->setEnabled(false);
    }

    qDebug() << "[DELETE_LATER] Запрошено удаление панели:"
             << m_panel->objectName();

    m_panel->deleteLater();
}

void MainWindow::demoDetaching()
{
    LogPanel *tempOwner =
        new LogPanel("TempOwner", this);
    tempOwner->hide();

    new LogLabel("Ребёнок TempOwner", tempOwner);

    LogLabel *detachedChild =
        new LogLabel("Отвязанный ребёнок TempOwner", tempOwner);

    qDebug() << "[DEMO] Количество детей tempOwner до detach:"
             << tempOwner->children().count();

    detachedChild->setParent(nullptr);

    qDebug() << "[DEMO] Количество детей tempOwner после detach:"
             << tempOwner->children().count();

    setStatus(
        "Демо: одна метка осталась ребёнком TempOwner, "
        "другая отвязана через setParent(nullptr). "
        "Обе будут удалены отдельно или вместе со своим владельцем.");

    tempOwner->deleteLater();
    detachedChild->deleteLater();
}
