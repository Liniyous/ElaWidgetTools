#include "T_Icon.h"

#include "ElaLineEdit.h"
#include "ElaListView.h"
#include "ElaText.h"
#include "T_IconDelegate.h"
#include "T_IconModel.h"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QVBoxLayout>

T_Icon::T_Icon(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("图标资源");
    createCustomWidget("按英文名称搜索图标，点击图标复制对应的 ElaIconType 枚举。", "图标资源");
    auto* mainLayout = createPageLayout();
    auto* section = addExampleSection(mainLayout, "图标浏览器", "悬停可查看完整名称，支持大小写不敏感的关键字搜索。");
    _metaEnum = QMetaEnum::fromType<ElaIconType::IconName>();
    _iconModel = new T_IconModel(this);
    _iconDelegate = new T_IconDelegate(this);
    _searchEdit = new ElaLineEdit(this);
    _searchEdit->setPlaceholderText("搜索图标，例如 Search、Folder、Arrow");
    _searchEdit->setClearButtonEnabled(true);
    _searchEdit->setMinimumWidth(120);
    _searchEdit->setMaximumWidth(360);
    _resultText = new ElaText(QString("%1 个图标").arg(_iconModel->rowCount()), this);
    _resultText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    auto* searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(16);
    searchLayout->addWidget(_searchEdit, 1);
    searchLayout->addWidget(_resultText);
    _iconView = new ElaListView(this);
    _iconView->setIsTransparent(true);
    _iconView->setFlow(QListView::LeftToRight);
    _iconView->setViewMode(QListView::IconMode);
    _iconView->setResizeMode(QListView::Adjust);
    _iconView->setUniformItemSizes(true);
    _iconView->setSpacing(4);
    _iconView->setFixedHeight(420);
    _iconView->setModel(_iconModel);
    _iconView->setItemDelegate(_iconDelegate);
    _iconView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _emptyText = new ElaText("没有找到匹配的图标，请尝试其他关键字。", this);
    _emptyText->setAlignment(Qt::AlignCenter);
    _emptyText->setMinimumHeight(140);
    _emptyText->hide();
    auto* selectedText = new ElaText("点击一个图标即可复制枚举名称", this);
    selectedText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    selectedText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    connect(_iconView, &ElaListView::clicked, this, [this, selectedText](const QModelIndex& index) {
        const QString iconName = _iconModel->getIconNameFromModelIndex(index);
        if (!iconName.isEmpty())
        {
            qApp->clipboard()->setText(iconName);
            selectedText->setText("已复制：" + iconName);
        }
    });
    connect(_searchEdit, &ElaLineEdit::textChanged, this, &T_Icon::onSearchEditTextEdit);
    section->addLayout(searchLayout);
    section->addWidget(_iconView);
    section->addWidget(_emptyText);
    section->addWidget(selectedText);
}

T_Icon::~T_Icon()
{
}

void T_Icon::onSearchEditTextEdit(const QString& searchText)
{
    const QString query = searchText.trimmed();
    QStringList names;
    if (!query.isEmpty())
    {
        for (int index = 1; index < _metaEnum.keyCount(); ++index)
        {
            const QString name = _metaEnum.key(index);
            if (name.contains(query, Qt::CaseInsensitive))
            {
                names.append(name);
            }
        }
    }
    _iconModel->setIsSearchMode(!query.isEmpty());
    _iconModel->setSearchKeyList(names);
    _iconView->clearSelection();
    _iconView->scrollToTop();
    const int count = _iconModel->rowCount();
    _resultText->setText(QString("%1 个图标").arg(count));
    _emptyText->setVisible(count == 0);
    _iconView->setVisible(count > 0);
}
