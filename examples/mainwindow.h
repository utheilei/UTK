#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <message/UMessage>
#include "umainwindow.h"

#include <QMap>

class HLToolTip;
class QStackedWidget;
class HLListView;
class TitlesWidget;
class HLStyleButton;
class MainWindow : public UMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() Q_DECL_OVERRIDE;

    void initMenu();

    void showFloatMessage(bool isOK, const QString &message);

    void initListView();

    void initContent();

    void initConnection();
    
    std::shared_ptr<UMessageResult> test(std::shared_ptr<UMessage> msg);

signals:

public slots:
    void handleAbout();
    void handleQuit();
    void onChangeWidget(const QModelIndex &index);
    void onChangeCenterWidget(const QModelIndex &index);

private:
    HLToolTip* toolTip = Q_NULLPTR;
    QMap<int, QWidget*> m_listWidget;
    HLListView* m_listview = Q_NULLPTR;
    QStackedWidget* m_mainWidget = Q_NULLPTR;
    QStackedWidget* m_stackedWidget = Q_NULLPTR;
    TitlesWidget* m_titlesWidget = Q_NULLPTR;
    HLStyleButton* m_backBtn = Q_NULLPTR;
};

#endif // MAINWINDOW_H
