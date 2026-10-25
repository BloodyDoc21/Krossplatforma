#ifndef FORMWINDOW_H
#define FORMWINDOW_H

#include <QList>
#include <QStringList>
#include <QWidget>

#include "adaptivefield.h"

class QFormLayout;
class QVBoxLayout;
class QComboBox;
class QCheckBox;

class FormWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FormWindow(QWidget *parent = nullptr);
    ~FormWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void setCompactMode();
    void setComfortableMode();
    void toggleDescriptionPolicy();
    void toggleAgePolicy();
    void dumpSizePolicies();
    void dumpSizes();
    void onAccept();
    void onReject();

private:
    void setupUi();
    static QString policyName(QSizePolicy::Policy p);

    AdaptiveLineEdit m_name;
    AdaptiveLineEdit m_email;
    AdaptiveLineEdit m_age;
    QComboBox *m_country;
    AdaptiveTextEdit m_description;
    QCheckBox *m_agree;

    QVBoxLayout *m_rootLayout;
    QFormLayout *m_formLayout;

    QList<QWidget *> m_trackedFields;
    QStringList m_trackedNames;

    bool m_descriptionExpanding;
    bool m_ageExpanding;
};

#endif
