#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include <QObject>
#include <cstdlib>

#include "calculator.h"



int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    system("chcp 65001 > nul");
#endif
    QCoreApplication app(argc, argv);

    QTextStream input(stdin);
    QTextStream output(stdout);

    Calculator calculator;

    QObject::connect(&calculator, &Calculator::resultReady,
                     [&](double result)
                     {
                         output << "Результат: " << result << "\n";
                     });

    QObject::connect(&calculator, &Calculator::errorOccurred,
                     [&](const QString &message)
                     {
                         output << "Ошибка: " << message << "\n";
                     });

    QObject::connect(&calculator, &Calculator::operationTimed,
                     [&](double milliseconds)
                     {
                         output << "Время операции: "
                                << milliseconds
                                << " мс\n";
                     });

    output << "=== Консольный калькулятор ===\n";
    output << "Команды:\n";
    output << "add a b - сложение\n";
    output << "sub a b - вычитание\n";
    output << "mul a b - умножение\n";
    output << "div a b - деление\n";
    output << "reset - сброс\n";
    output << "help - справка\n";
    output << "quit - выход\n\n";

    while (true)
    {
        output << "> ";
        output.flush();

        const QString line = input.readLine().trimmed();

        if (line.isEmpty())
            continue;

        const QStringList parts =
            line.split(' ', Qt::SkipEmptyParts);

        const QString command = parts[0].toLower();

        if (command == "quit")
        {
            output << "Завершение программы.\n";
            break;
        }

        if (command == "help")
        {
            output << "Используйте:\n";
            output << "add a b\n";
            output << "sub a b\n";
            output << "mul a b\n";
            output << "div a b\n";
            output << "reset\n";
            output << "quit\n";
            continue;
        }

        if (command == "reset")
        {
            calculator.reset();
            continue;
        }

        if (parts.size() != 3)
        {
            output << "Ошибка: необходимо указать два числа.\n";
            continue;
        }

        bool okA = false;
        bool okB = false;

        const double a = parts[1].toDouble(&okA);
        const double b = parts[2].toDouble(&okB);

        if (!okA || !okB)
        {
            output << "Ошибка: некорректные числа.\n";
            continue;
        }

        if (command == "add")
        {
            calculator.add(a, b);
        }
        else if (command == "sub")
        {
            calculator.subtract(a, b);
        }
        else if (command == "mul")
        {
            calculator.multiply(a, b);
        }
        else if (command == "div")
        {
            calculator.divide(a, b);
        }
        else
        {
            output << "Ошибка: неизвестная команда.\n";
        }
    }

    return 0;
}