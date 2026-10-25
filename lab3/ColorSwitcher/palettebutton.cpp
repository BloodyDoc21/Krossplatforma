#include "palettebutton.h"

#include <QDebug>

PaletteButton::PaletteButton(const QColor &color, QWidget *parent)
    : QPushButton(parent)
    , m_color(color)
{
    setText(color.name());
    setToolTip("Добавить этот цвет в палитру холста");
    setStyleSheet(QString("background-color: %1").arg(color.name()));
    qDebug() << "[CREATE] PaletteButton" << color.name();
}

PaletteButton::~PaletteButton()
{
    qDebug() << "[DESTROY] PaletteButton" << m_color.name();
}
