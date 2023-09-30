#include "mainwindow.h"
#include "hltooltip.h"
#include "widgets/hllistview.h"
#include "widgets/hlmenu.h"
#include "widgets/hlslider.h"
#include "widgets/hlstylebutton.h"
#include "uaboutdialog.h"
#include "uapplication.h"
#include "widgets/uscrollbar.h"
#include "style/uproxystyle.h"
#include "uwidgetutils.h"
#include "titleswidget.h"
#include "titlebar.h"

#include <message/UMessageDispatcher>
#include <thread/UThread>

#include <QBoxLayout>
#include <QIcon>
#include <QTimer>
#include <QApplication>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <QDebug>
#include <QScrollArea>
#include <QLabel>
#include <QActionGroup>

MainWindow::MainWindow(QWidget* parent)
    : UMainWindow(parent)
    , m_listview(new HLListView(this))
    , m_mainWidget(new QStackedWidget(this))
    , m_stackedWidget(new QStackedWidget(this))
    , m_titlesWidget(new TitlesWidget(this))
    , m_backBtn(new HLStyleButton("", this))
{
    initMenu();
    m_listview->initListView();

    if (titleBar())
    {
        m_backBtn->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
        m_backBtn->setEnabled(false);
        m_backBtn->setFixedSize(35, 35);
        m_backBtn->setCheckable(false);
        titleBar()->insertWidget(2, m_backBtn);
    }

    QHBoxLayout* mainLayout = new QHBoxLayout;
    QWidget* centerWidget = new QWidget(this);
    centerWidget->setLayout(mainLayout);
    setContentWidget(m_stackedWidget);

    mainLayout->addWidget(m_listview, 1);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_mainWidget, 4);

    m_stackedWidget->addWidget(m_titlesWidget);
    m_stackedWidget->addWidget(centerWidget);

    initListView();
    initContent();
    initConnection();
    m_listview->setCurrentIndex(m_listview->model()->index(0, 0));

    UMessageDispatcher::instance()->registerEvent(1, std::bind(&MainWindow::test, this, std::placeholders::_1));
    auto res = std::make_shared<UMessageResult>();
    UMessageDispatcher::instance()->sendMessage(1, std::make_shared<UMessage>(1, 1, "test"), std::move(res));
    int code = UMessageDispatcher::instance()->postMessage(1, std::make_shared<UMessage>(1, 1, "test"));
    qDebug() << res->msgCode << res->msgResult << code;

    QThread *thread = UThread::createFuture<std::shared_ptr<UMessageResult>>(std::bind(&MainWindow::test, this, std::make_shared<UMessage>()));
    connect(thread, &QThread::finished, this, [=](){
        auto s = dynamic_cast<QThreadCreateTaskThread<std::shared_ptr<UMessageResult>>*>(sender());
        qDebug() << "UThread finished" << s->getResult()->msgResult;
    });
    thread->start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMenu()
{
    QActionGroup* group = new QActionGroup(this);
    HLMenu* menu = new HLMenu(this);
    QAction* about = menu->addAction(tr("About"));
    about->setIcon(QIcon::fromTheme("about"));
    menu->addSeparator();
    HLMenu* menu1 = new HLMenu(this);
    menu1->setTitle("theme");
    QAction* dark = menu1->addAction("dark");
    dark->setCheckable(true);
    group->addAction(dark);
    QAction* light = menu1->addAction("light");
    light->setCheckable(true);
    group->addAction(light);
    menu->addMenu(menu1);
    QAction* exit = menu->addAction(tr("Exit"));
    addMenu(menu);

    if (uApp->applicationTheme() == UTheme::DarkTheme)
    {
        dark->setChecked(true);
    }
    else
    {
        light->setChecked(true);
    }

    connect(about, &QAction::triggered, this, &MainWindow::handleAbout);
    connect(exit, &QAction::triggered, this, &MainWindow::handleQuit);
    connect(dark, &QAction::triggered, this, [ = ]()
    {
        if (auto style = qobject_cast<UProxyStyle*>(uApp->style()))
        {
            style->setTheme(UTheme::DarkTheme);
            uApp->setPalette(style->standardPalette());
        }
    });
    connect(light, &QAction::triggered, this, [ = ]()
    {
        if (auto style = qobject_cast<UProxyStyle*>(uApp->style()))
        {
            style->setTheme(UTheme::LightTheme);
            uApp->setPalette(style->standardPalette());
        }
    });
}

void MainWindow::showFloatMessage(bool isOK, const QString &message)
{
    if (message.isEmpty())
        return;

    if (toolTip && toolTip->isVisible())
        return;

    toolTip = new HLToolTip(this);
    toolTip->setIcon(isOK ? QIcon(":/images/success_20px.svg") : QIcon(":/images/warning_30px.svg"));
    toolTip->setText(message);
    QPoint point = QPoint((width() - toolTip->sizeHint().width()) / 2,
                          (height() - toolTip->sizeHint().height() - 30));
    toolTip->show();
    toolTip->move(point);

    QTimer::singleShot(5000, this, [ = ]()
    {
        toolTip->close();
        toolTip->deleteLater();
        toolTip = Q_NULLPTR;
    });
}

void MainWindow::initListView()
{
    const QVector<QString> textList =
    {
        QObject::tr("Label"), QObject::tr("Button"), QObject::tr("LineEdit"), QObject::tr("defapp"),
        QObject::tr("personalization"), QObject::tr("network"), QObject::tr("sound"), QObject::tr("datetime"),
        QObject::tr("power"), QObject::tr("mouse"), QObject::tr("keyboard"), QObject::tr("unionid"),
        QObject::tr("update"), QObject::tr("systeminfo"), QObject::tr("commoninfo")
    };

    QStandardItemModel* model = new QStandardItemModel(this);
    m_listview->setModel(model);

    for (int i = 0; i < textList.size(); i++)
    {
        QStandardItem* item = new QStandardItem(textList.at(i));
        item->setIcon(UWidgetUtils::getNumCircleIcon(i + 1, QFont("SimHei", 14)));
        item->setSizeHint(QSize(160, 50));
        model->appendRow(item);
    }
}

void MainWindow::initContent()
{
    UWidget* labelWidget = new UWidget(this);
    auto slider = new HLSlider(Qt::Horizontal, labelWidget);
    slider->setGeometry(600, 400, 200, 40);
    QScrollArea* area = new QScrollArea(labelWidget);
    auto b = new UScrollBar(area);
    area->setHorizontalScrollBar(b);
    auto c = new UScrollBar(area);
    area->setVerticalScrollBar(c);
    area->setGeometry(100, 100, 400, 400);
    QLabel* label = new QLabel("this is label", area);
    label->setFixedSize(600, 600);
    area->setWidgetResizable(true);
    area->setWidget(label);
    labelWidget->setBackgroundColor(Qt::red);
    m_mainWidget->addWidget(labelWidget);
    m_listWidget.insert(0, labelWidget);

    UWidget* buttonWidget = new UWidget(this);
    buttonWidget->setBackgroundColor(Qt::green);
    m_mainWidget->addWidget(buttonWidget);
    m_listWidget.insert(1, buttonWidget);

    UWidget* comboxLineEditWidget = new UWidget(this);
    comboxLineEditWidget->setBackgroundColor(Qt::blue);
    m_mainWidget->addWidget(comboxLineEditWidget);
    m_listWidget.insert(2, comboxLineEditWidget);
}

void MainWindow::initConnection()
{
    connect(m_listview, &HLListView::clicked, this, &MainWindow::onChangeWidget);

    connect(m_backBtn, &QPushButton::clicked, this, [ = ]()
    {
        m_stackedWidget->setCurrentIndex(0);
        m_backBtn->setEnabled(false);
    });

    connect(m_titlesWidget, &QListView::clicked, this, &MainWindow::onChangeCenterWidget);
}

std::shared_ptr<UMessageResult> MainWindow::test(std::shared_ptr<UMessage> msg)
{
    qDebug() << msg->msgId << msg->msgType << msg->msgData << QThread::currentThreadId();
    return std::make_shared<UMessageResult>(true, "aaa");
}

void MainWindow::handleAbout()
{
    UAboutDialog dialog(this);
    dialog.setAppIcon(uApp->windowIcon());
    dialog.setAppVersion(QString("Version: %1").arg(uApp->applicationVersion()));
    dialog.setAppName(uApp->applicationName());
    dialog.setAppDescribe("this is a utkwidget");
    dialog.setAppHomePage(uApp->organizationDomain());
    dialog.exec();
}

void MainWindow::handleQuit()
{
    qApp->quit();
}

void MainWindow::onChangeWidget(const QModelIndex &index)
{
    qDebug() << "============" << index.data() << index.row();
    if (m_listWidget.contains(index.row()))
    {
        m_mainWidget->setCurrentIndex(index.row());
    }
}

void MainWindow::onChangeCenterWidget(const QModelIndex &index)
{
    m_stackedWidget->setCurrentIndex(1);
    m_backBtn->setEnabled(true);

    QModelIndex currentIndex = m_listview->model()->index(index.row(), 0);
    if (currentIndex.isValid())
    {
        m_listview->setCurrentIndex(currentIndex);
        onChangeWidget(currentIndex);
    }
}
