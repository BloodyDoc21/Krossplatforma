#include "colorwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>

ColorWidget::ColorWidget(QWidget *parent)
    : QWidget(parent)
    , m_palette(defaultPalette())
    , m_index(0)
    , m_blinkOn(false)
{
    setMinimumSize(320, 220);
    qDebug() << "[CREATE] ColorWidget, палитра:" << m_palette.size();
}

ColorWidget::~ColorWidget()
{
    qDebug() << "[DESTROY] ColorWidget";
}

QVector<QColor> ColorWidget::defaultPalette()
{
    return {
        QColor("#e74c3c"), QColor("#e67e22"), QColor("#f1c40f"),
        QColor("#2ecc71"), QColor("#3498db"), QColor("#9b59b6")
    };
}

QColor ColorWidget::currentColor() const
{
    if (m_palette.isEmpty()) {
        return Qt::gray;
    }
    return m_palette.at(m_index % m_palette.size());
}

void ColorWidget::addColor(const QColor &c)
{
    m_palette.push_back(c);
    qDebug() << "[PALETTE] Добавлен цвет" << c.name()
             << ", всего:" << m_palette.size();
}

void ColorWidget::resetPalette()
{
    m_palette = defaultPalette();
    m_index = 0;
    m_blinkOn = false;
    update();
    emitCurrent();
}

void ColorWidget::stepForward()
{
    if (m_palette.isEmpty()) {
        return;
    }
    m_index = (m_index + 1) % m_palette.size();
    m_blinkOn = false;
    update();
    emitCurrent();
}

void ColorWidget::stepBackward()
{
    if (m_palette.isEmpty()) {
        return;
    }
    m_index = (m_index - 1 + m_palette.size()) % m_palette.size();
    m_blinkOn = false;
    update();
    emitCurrent();
}

void ColorWidget::setRandomColor()
{
    if (m_palette.isEmpty()) {
        return;
    }
    m_index = QRandomGenerator::global()->bounded(m_palette.size());
    m_blinkOn = false;
    update();
    emitCurrent();
}

void ColorWidget::toggleBlink()
{
    m_blinkOn = !m_blinkOn;
    update();
    emitCurrent();
}

void ColorWidget::emitCurrent()
{
    emit colorChanged(currentColor());
}

void ColorWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QColor fill = m_blinkOn ? QColor(Qt::black) : currentColor();
    p.fillRect(rect(), fill);
    p.setPen(fill.lightness() > 128 ? Qt::black : Qt::white);
    p.drawText(rect(), Qt::AlignCenter, fill.name());
}

void ColorWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "[MOUSE] Левый клик по холсту";
        stepForward();
    } else if (event->button() == Qt::RightButton) {
        qDebug() << "[MOUSE] Правый клик по холсту";
        stepBackward();
    }
    QWidget::mousePressEvent(event);
}
