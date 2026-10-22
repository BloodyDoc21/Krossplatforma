#include "calculator.h"

#include <QElapsedTimer>
#include <QtGlobal>

Calculator::Calculator(QObject *parent)
    : QObject(parent)
{
}

void Calculator::setResult(double value)
{
    m_result = value;
    m_hasError = false;
    m_errorMessage.clear();

    emit resultReady(m_result);
}

void Calculator::add(double a, double b)
{
    QElapsedTimer timer;
    timer.start();

    setResult(a + b);

    emit operationTimed(timer.nsecsElapsed() / 1'000'000.0);
}

void Calculator::subtract(double a, double b)
{
    QElapsedTimer timer;
    timer.start();

    setResult(a - b);

    emit operationTimed(timer.nsecsElapsed() / 1'000'000.0);
}

void Calculator::multiply(double a, double b)
{
    QElapsedTimer timer;
    timer.start();

    setResult(a * b);

    emit operationTimed(timer.nsecsElapsed() / 1'000'000.0);
}

void Calculator::divide(double a, double b)
{
    QElapsedTimer timer;
    timer.start();

    if (qFuzzyIsNull(b))
    {
        m_hasError = true;
        m_errorMessage = "Деление на ноль невозможно";

        emit errorOccurred(m_errorMessage);
    }
    else
    {
        setResult(a / b);
    }

    emit operationTimed(timer.nsecsElapsed() / 1'000'000.0);
}

void Calculator::reset()
{
    QElapsedTimer timer;
    timer.start();

    setResult(0.0);

    emit operationTimed(timer.nsecsElapsed() / 1'000'000.0);
}