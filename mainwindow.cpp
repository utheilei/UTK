#include "mainwindow.h"
#include "hltooltip.h"
#include "stylebutton.h"
#include "messagebox.h"
#include "widgets/hllistview.h"
#include "widgets/hlmenu.h"
#include "widgets/hlstylebutton.h"

#include "widgetutils.h"
#include "titleswidget.h"
#include "titlebar.h"

#include <QBoxLayout>
#include <QIcon>
#include <QTimer>
#include <QApplication>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <QDebug>

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
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMenu()
{
    HLMenu* menu = new HLMenu(this);
    QAction* about = menu->addAction(tr("About"));
    about->setToolTip("ddd");
    about->setIcon(QIcon(":/images/about.png"));
    menu->addSeparator();
    QAction* exit = menu->addAction(tr("Exit"));
    HLMenu* menu1 = new HLMenu(this);
    menu1->setTitle("sub");
    menu1->addAction("test");
    menu->addMenu(menu1);

    addMenu(menu);

    connect(about, &QAction::triggered, this, &MainWindow::handleAbout);
    connect(exit, &QAction::triggered, this, &MainWindow::handleQuit);
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
        item->setIcon(WidgetUtils::getNumCircleIcon(i + 1, QFont("SimHei", 14)));
        item->setSizeHint(QSize(160, 50));
        model->appendRow(item);
    }
}

void MainWindow::initContent()
{
    UWidget* labelWidget = new UWidget(this);
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

void MainWindow::handleAbout()
{
    MessageBox customMsgBox;
    customMsgBox.setWindowTitle(tr("关于本软件"));
    StyleButton* styleButton = new StyleButton(tr("好的"), this);
    styleButton->setRadius(8);
    customMsgBox.addButton(styleButton, QMessageBox::ActionRole);
    customMsgBox.setIconPixmap(QPixmap(":/images/about.png").scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    customMsgBox.setText(tr("欢迎使用Qt-Demo软件"));
    customMsgBox.setTextFont(QFont("SimHei", 10));
    customMsgBox.setTextWordWrap(true);
    customMsgBox.exec();
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
