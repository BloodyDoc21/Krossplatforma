#include "mainwindow.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QVBoxLayout>

const QString kTitle = "QTimer + лямбда-слоты + динамические соединения";

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_mode(Mode::Cycle)
    , m_clickHandlerAlt(false)
    , m_rootLayout(nullptr)
    , m_paletteGrid(nullptr)
    , m_statusLabel(nullptr)
    , m_intervalLabel(nullptr)
    , m_modeLabel(nullptr)
{
    m_timer.setInterval(600);
    setupUi();

    rebuildTimeoutConnection();
    reconnectCanvasHandler();

    setStatus("Готово. Нажмите «Старт/Стоп», чтобы запустить таймер.");
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
    m_timer.stop();
}

void MainWindow::setupUi()
{
    setWindowTitle(kTitle);
    resize(560, 640);

    m_rootLayout = new QVBoxLayout(this);

    m_canvas.setParent(this);
    m_rootLayout->addWidget(&m_canvas);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_rootLayout->addWidget(m_statusLabel);

    m_intervalLabel = new QLabel(this);
    m_modeLabel = new QLabel(this);
    m_rootLayout->addWidget(m_intervalLabel);
    m_rootLayout->addWidget(m_modeLabel);

    QHBoxLayout *controls = new QHBoxLayout();
    QPushButton *btnStart = new QPushButton("Старт/Стоп", this);
    QPushButton *btnSlow = new QPushButton("Медленнее", this);
    QPushButton *btnFast = new QPushButton("Быстрее", this);
    QPushButton *btnMode = new QPushButton("Режим", this);
    controls->addWidget(btnStart);
    controls->addWidget(btnSlow);
    controls->addWidget(btnFast);
    controls->addWidget(btnMode);
    m_rootLayout->addLayout(controls);

    connect(btnStart, &QPushButton::clicked,
            this, &MainWindow::toggleTimer);
    connect(btnSlow, &QPushButton::clicked,
            this, &MainWindow::decreaseInterval);
    connect(btnFast, &QPushButton::clicked,
            this, &MainWindow::increaseInterval);
    connect(btnMode, &QPushButton::clicked,
            this, &MainWindow::cycleMode);

    QLabel *palHeader =
        new QLabel("Палитра (кнопки создаются динамически):", this);
    m_rootLayout->addWidget(palHeader);

    m_paletteGrid = new QGridLayout();
    m_rootLayout->addLayout(m_paletteGrid);

    QHBoxLayout *palControls = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("+ Цвет", this);
    QPushButton *btnClear =
        new QPushButton("Очистить палитру", this);
    QPushButton *btnClick =
        new QPushButton("Сменить обработчик клика", this);
    palControls->addWidget(btnAdd);
    palControls->addWidget(btnClear);
    palControls->addWidget(btnClick);
    m_rootLayout->addLayout(palControls);

    connect(btnAdd, &QPushButton::clicked,
            this, &MainWindow::addColorButton);
    connect(btnClear, &QPushButton::clicked,
            this, &MainWindow::clearPaletteButtons);
    connect(btnClick, &QPushButton::clicked,
            this, &MainWindow::toggleClickHandler);

    m_rootLayout->addStretch();

    updateIntervalLabel();
    updateModeLabel();
}

void MainWindow::setStatus(const QString &text)
{
    if (m_statusLabel) {
        m_statusLabel->setText(text);
    }
}

void MainWindow::updateIntervalLabel()
{
    if (m_intervalLabel) {
        m_intervalLabel->setText(
            QString("Интервал: %1 мс").arg(m_timer.interval()));
    }
}

void MainWindow::updateModeLabel()
{
    if (m_modeLabel) {
        m_modeLabel->setText(QString("Режим: %1").arg(modeName()));
    }
}

QString MainWindow::modeName() const
{
    switch (m_mode) {
    case Mode::Cycle:
        return "Цикл";
    case Mode::Random:
        return "Случайный";
    case Mode::Blink:
        return "Мигание";
    }
    return QString();
}

void MainWindow::toggleTimer()
{
    if (m_timer.isActive()) {
        m_timer.stop();
        setStatus("Таймер остановлен.");
        qDebug() << "[TIMER] stop()";
    } else {
        m_timer.start();
        setStatus(QString("Таймер запущен (интервал %1 мс).")
                      .arg(m_timer.interval()));
        qDebug() << "[TIMER] start(), interval =" << m_timer.interval();
    }
}

void MainWindow::decreaseInterval()
{
    m_timer.setInterval(qMin(m_timer.interval() + 200, 5000));
    updateIntervalLabel();
    qDebug() << "[TIMER] interval ->" << m_timer.interval();
}

void MainWindow::increaseInterval()
{
    m_timer.setInterval(qMax(m_timer.interval() - 200, 50));
    updateIntervalLabel();
    qDebug() << "[TIMER] interval ->" << m_timer.interval();
}

void MainWindow::cycleMode()
{
    switch (m_mode) {
    case Mode::Cycle:
        m_mode = Mode::Random;
        break;
    case Mode::Random:
        m_mode = Mode::Blink;
        break;
    case Mode::Blink:
        m_mode = Mode::Cycle;
        break;
    }
    updateModeLabel();
    rebuildTimeoutConnection();
    setStatus(QString("Режим переключён: %1").arg(modeName()));
    qDebug() << "[TIMER] режим ->" << modeName()
             << "(переподключение timeout)";
}

void MainWindow::rebuildTimeoutConnection()
{
    if (m_timeoutConn) {
        disconnect(m_timeoutConn);
        m_timeoutConn = QMetaObject::Connection();
    }

    m_timeoutConn = connect(&m_timer, &QTimer::timeout, this, [this]() {
        switch (m_mode) {
        case Mode::Cycle:
            m_canvas.stepForward();
            break;
        case Mode::Random:
            m_canvas.setRandomColor();
            break;
        case Mode::Blink:
            m_canvas.toggleBlink();
            break;
        }
        qDebug() << "[TIMER] timeout, режим =" << modeName()
                 << ", цвет =" << m_canvas.currentColor().name();
    });
}

void MainWindow::reconnectCanvasHandler()
{
    if (m_clickConn) {
        disconnect(m_clickConn);
        m_clickConn = QMetaObject::Connection();
    }

    if (!m_clickHandlerAlt) {
        m_clickConn = connect(&m_canvas, &ColorWidget::colorChanged,
                              this, [this](const QColor &c) {
            setStatus(QString("Цвет холста: %1").arg(c.name()));
        });
    } else {
        m_clickConn = connect(&m_canvas, &ColorWidget::colorChanged,
                              this, [this](const QColor &c) {
            setWindowTitle(QString("ColorSwitcher – %1").arg(c.name()));
        });
    }
}

void MainWindow::toggleClickHandler()
{
    m_clickHandlerAlt = !m_clickHandlerAlt;
    reconnectCanvasHandler();
    if (m_clickHandlerAlt) {
        setStatus("Обработчик клика: вариант Б (заголовок окна).");
    } else {
        setWindowTitle(kTitle);
        setStatus("Обработчик клика: вариант А (строка статуса).");
    }
    qDebug() << "[CANVAS] переподключение colorChanged, альтернатива ="
             << m_clickHandlerAlt;
}

void MainWindow::addColorButton()
{
    QColor color = QColor::fromHsv(
        QRandomGenerator::global()->bounded(360),
        200 + QRandomGenerator::global()->bounded(55),
        200 + QRandomGenerator::global()->bounded(55));

    PaletteButton *btn = new PaletteButton(color, this);

    int row = m_paletteButtons.size() / 4;
    int col = m_paletteButtons.size() % 4;
    m_paletteGrid->addWidget(btn, row, col);

    QMetaObject::Connection conn =
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            m_canvas.addColor(btn->color());
            setStatus(QString("Добавлен цвет %1 в палитру холста.")
                          .arg(btn->color().name()));
            qDebug() << "[DYNAMIC] нажата кнопка-цвет"
                     << btn->color().name();
        });

    m_paletteButtons.append(QPointer<PaletteButton>(btn));
    m_dynamicConnections.append(conn);

    setStatus(QString("Создана кнопка-цвет %1 (динамическое соединение).")
                  .arg(color.name()));
    qDebug() << "[DYNAMIC] создано соединение для" << color.name()
             << ", всего динамических:" << m_dynamicConnections.size();
}

void MainWindow::clearPaletteButtons()
{
    int count = 0;
    for (QMetaObject::Connection &c : m_dynamicConnections) {
        if (c) {
            disconnect(c);
            count++;
        }
    }
    m_dynamicConnections.clear();

    for (QPointer<PaletteButton> &btn : m_paletteButtons) {
        if (!btn.isNull()) {
            btn->deleteLater();
        }
    }
    m_paletteButtons.clear();

    m_canvas.resetPalette();

    setStatus("Палитра очищена: соединения разорваны, кнопки удалены.");
    qDebug() << "[DYNAMIC] очищена палитра, разорвано соединений:"
             << count;
}
