#include "c_timepage.h"
#include "ui_c_timepage.h"

#include "ElaSvgIcon.h"
#include <QStandardItemModel>

#include "ElaApplication.h"

Q_SINGLETON_CREATE_CPP(C_TimePage)
C_TimePage::C_TimePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::C_TimePage)
{
    ui->setupUi(this);

    initUI();
}

C_TimePage::~C_TimePage()
{
    delete ui;
}

void C_TimePage::initUI()
{
    setWindowTitle("日期和时间");

    ui->widget->setFixedHeight(75);
    ui->widget_3->setFixedHeight(75);
    ui->widget_4->setFixedHeight(75);

    ui->label->setTextStyle(ElaTextType::CardTitle);
    ui->label_2->setTextStyle(ElaTextType::CardTitle);
    ui->label_3->setTextStyle(ElaTextType::CardTitle);
    ui->label_4->setTextStyle(ElaTextType::CardsubTitle);

    QFont font = ui->comboBox->font();
    font.setPixelSize(font.pixelSize() + 1);
    ui->comboBox->view()->setFont(font);
    ui->comboBox->setFont(font);

    ui->label_icon->setSvgIcon(SvgIconType::DOWNLOAD, 18, 18);

    ui->widget->setBorderRadius(8, 8, 0, 0);
    ui->widget_3->setBorderRadius(0, 0, 0, 0);
    ui->widget_4->setBorderRadius(0, 0, 8, 8);

    QFont tableHeaderFont = ui->tableView->horizontalHeader()->font();
    tableHeaderFont.setPixelSize(16);
    ui->tableView->horizontalHeader()->setFont(tableHeaderFont);
    //ui->tableView->setModel(new Tui->tableViewModel(this));
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setIconSize(QSize(38, 38));
    ui->tableView->verticalHeader()->setHidden(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(60);
    ui->tableView->verticalHeader()->setMinimumSectionSize(46);
    ui->tableView->setFixedHeight(450);

    QStringList heardList;//表头
    heardList << "姓名" << "性别" << "备注";
    QHeaderView *hearview = new QHeaderView(Qt::Horizontal);
    QStandardItemModel* model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(heardList);
    hearview->setModel(model);
    hearview->setSectionResizeMode(QHeaderView::Stretch);    //先自适应宽度
    hearview->setSectionResizeMode(0, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
    ui->tableView->setHorizontalHeader(hearview);

    update();
}

void C_TimePage::on_pushButton_clicked()
{
    MainWindow::getInstance()->showLoading(true);
}

