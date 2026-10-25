#include "adaptivefield.h"

#include <QDebug>
#include <QResizeEvent>

AdaptiveLineEdit::AdaptiveLineEdit(const QString &label, QWidget *parent)
    : QLineEdit(parent)
    , m_label(label)
{
    qDebug() << "[CREATE]" << m_label << "(AdaptiveLineEdit)";
}

void AdaptiveLineEdit::setHorizontalPolicy(QSizePolicy::Policy hp)
{
    QSizePolicy sp = sizePolicy();
    sp.setHorizontalPolicy(hp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "horiz ->" << int(hp);
    updateGeometry();
}

void AdaptiveLineEdit::setVerticalPolicy(QSizePolicy::Policy vp)
{
    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(vp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "vert ->" << int(vp);
    updateGeometry();
}

void AdaptiveLineEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    qDebug() << "[RESIZE]" << m_label << "->" << event->size().width()
             << "x" << event->size().height();
}

AdaptiveTextEdit::AdaptiveTextEdit(const QString &label, QWidget *parent)
    : QPlainTextEdit(parent)
    , m_label(label)
{
    qDebug() << "[CREATE]" << m_label << "(AdaptiveTextEdit)";
}

void AdaptiveTextEdit::setHorizontalPolicy(QSizePolicy::Policy hp)
{
    QSizePolicy sp = sizePolicy();
    sp.setHorizontalPolicy(hp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "horiz ->" << int(hp);
    updateGeometry();
}

void AdaptiveTextEdit::setVerticalPolicy(QSizePolicy::Policy vp)
{
    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(vp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "vert ->" << int(vp);
    updateGeometry();
}

void AdaptiveTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    qDebug() << "[RESIZE]" << m_label << "->" << event->size().width()
             << "x" << event->size().height();
}
