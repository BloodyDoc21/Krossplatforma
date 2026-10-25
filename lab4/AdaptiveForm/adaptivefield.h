#ifndef ADAPTIVEFIELD_H
#define ADAPTIVEFIELD_H

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSizePolicy>

class QResizeEvent;

class AdaptiveLineEdit : public QLineEdit
{
public:
    explicit AdaptiveLineEdit(const QString &label,
                              QWidget *parent = nullptr);

    void setHorizontalPolicy(QSizePolicy::Policy hp);
    void setVerticalPolicy(QSizePolicy::Policy vp);
    QString label() const { return m_label; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_label;
};

class AdaptiveTextEdit : public QPlainTextEdit
{
public:
    explicit AdaptiveTextEdit(const QString &label,
                              QWidget *parent = nullptr);

    void setHorizontalPolicy(QSizePolicy::Policy hp);
    void setVerticalPolicy(QSizePolicy::Policy vp);
    QString label() const { return m_label; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_label;
};

#endif
