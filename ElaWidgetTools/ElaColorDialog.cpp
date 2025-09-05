#include "ElaColorDialog.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>
#include <utility>

#include "ElaBaseListView.h"
#include "ElaColorDialogPrivate.h"
#include "ElaColorDisplayDelegate.h"
#include "ElaColorDisplayModel.h"
#include "ElaColorPicker.h"
#include "ElaColorPreview.h"
#include "ElaColorValueSliderStyle.h"
#include "ElaComboBox.h"
#include "ElaIntValidator.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
Q_TAKEOVER_NATIVEEVENT_CPP(ElaColorDialog, d_func()->_appBar);
ElaColorDialog::ElaColorDialog(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaColorDialogPrivate())
{
    Q_D(ElaColorDialog);
    d->q_ptr = this;
    setFixedSize(600, 600); // 默认宽高
    setObjectName("ElaColorDialog");
    setWindowTitle("ElaColorDialog");
    setWindowModality(Qt::ApplicationModal);

    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    d->_appBar->setAppBarHeight(30);
    d->_appBar->setIsFixedSize(true);
    d->_appBar->setIsStayTop(true);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    d->_appBar->setIsDefaultClosed(false);
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, [=]() {
        close();
    });

    // 颜色选择器
    ElaText* colorPickerText = new ElaText("编辑颜色", this);
    colorPickerText->setTextPixelSize(17);
    QHBoxLayout* colorPickerTextLayout = new QHBoxLayout();
    colorPickerTextLayout->setContentsMargins(0, 0, 0, 0);
    colorPickerTextLayout->addSpacing(3);
    colorPickerTextLayout->addWidget(colorPickerText);

    d->_colorPicker = new ElaColorPicker(this);
    connect(d->_colorPicker, &ElaColorPicker::selectedColorChanged, d, &ElaColorDialogPrivate::onColorPickerColorChanged);

    QVBoxLayout* colorPickerLayout = new QVBoxLayout();
    colorPickerLayout->setContentsMargins(0, 0, 0, 0);
    colorPickerLayout->addWidget(d->_colorPicker);
    colorPickerLayout->addStretch();

    // 颜色预览
    d->_colorPreview = new ElaColorPreview(this);
    QVBoxLayout* colorPreviewLayout = new QVBoxLayout();
    colorPreviewLayout->setContentsMargins(0, 0, 0, 0);
    colorPreviewLayout->addWidget(d->_colorPreview);
    colorPreviewLayout->addStretch();

    // 颜色value控制
    d->_colorValueSlider = new QSlider(this);
    d->_colorValueSlider->setFixedHeight(257);
    d->_colorValueSlider->setOrientation(Qt::Vertical);
    d->_colorValueSlider->setRange(0, 255);
    d->_colorValueSlider->setValue(255);
    d->_colorValueSliderStyle = new ElaColorValueSliderStyle();
    d->_colorValueSlider->setStyle(d->_colorValueSliderStyle);
    connect(d->_colorValueSlider, &QSlider::valueChanged, d, &ElaColorDialogPrivate::onColorValueSliderChanged);
    QVBoxLayout* colorValueSliderLayout = new QVBoxLayout();
    colorValueSliderLayout->setContentsMargins(0, 0, 0, 0);
    colorValueSliderLayout->addWidget(d->_colorValueSlider);
    colorValueSliderLayout->addStretch();

    // 颜色控制器
    d->_htmlEdit = new ElaLineEdit(this);
    d->_htmlEdit->setText("#000000");
    ElaIntValidator* htmlValidator = new ElaIntValidator(0x000000, 0xFFFFFF, this);
    htmlValidator->setIsHexMode(true);
    d->_htmlEdit->setValidator(htmlValidator);
    d->_htmlEdit->setFixedSize(120, 33);
    connect(d->_htmlEdit, &ElaLineEdit::focusOut, d, &ElaColorDialogPrivate::onHtmlEditFocusOut);
    connect(d->_htmlEdit, &ElaLineEdit::textEdited, d, &ElaColorDialogPrivate::onHtmlEditChanged);

    // 数值预览
    QHBoxLayout* htmlEditLayout = new QHBoxLayout();
    htmlEditLayout->setContentsMargins(0, 0, 0, 0);
    htmlEditLayout->addSpacing(6);
    htmlEditLayout->addWidget(d->_htmlEdit);
    htmlEditLayout->addStretch();
    d->_modeComboBox = new ElaComboBox(this);
    d->_modeComboBox->setFixedSize(132, 33);
    d->_modeComboBox->addItem("RGB");
    d->_modeComboBox->addItem("HSV");
    d->_modeComboBox->setCurrentIndex(0);
    connect(d->_modeComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), d, &ElaColorDialogPrivate::onColorModeChanged);

    // R或H
    d->_firstEdit = new ElaLineEdit(this);
    d->_firstEdit->setText("0");
    d->_firstEdit->setFixedSize(120, 33);
    d->_firstEdit->setValidator(new ElaIntValidator(0, 255, this));
    d->_firstText = new ElaText("红色", this);
    d->_firstText->setTextPixelSize(13);
    connect(d->_firstEdit, &ElaLineEdit::textEdited, d, &ElaColorDialogPrivate::onColorEditChanged);

    QHBoxLayout* firstEditLayout = new QHBoxLayout();
    firstEditLayout->setContentsMargins(0, 0, 0, 0);
    firstEditLayout->addSpacing(6);
    firstEditLayout->addWidget(d->_firstEdit);
    firstEditLayout->addWidget(d->_firstText);
    firstEditLayout->addStretch();

    // G或S
    d->_secondEdit = new ElaLineEdit(this);
    d->_secondEdit->setText("0");
    d->_secondEdit->setFixedSize(120, 33);
    d->_secondEdit->setValidator(new ElaIntValidator(0, 255, this));
    d->_secondText = new ElaText("绿色", this);
    d->_secondText->setTextPixelSize(13);
    connect(d->_secondEdit, &ElaLineEdit::textEdited, d, &ElaColorDialogPrivate::onColorEditChanged);
    QHBoxLayout* secondEditLayout = new QHBoxLayout();
    secondEditLayout->setContentsMargins(0, 0, 0, 0);
    secondEditLayout->addSpacing(6);
    secondEditLayout->addWidget(d->_secondEdit);
    secondEditLayout->addWidget(d->_secondText);
    secondEditLayout->addStretch();

    // B或V
    d->_thridEdit = new ElaLineEdit(this);
    d->_thridEdit->setText("0");
    d->_thridEdit->setFixedSize(120, 33);
    d->_thridEdit->setValidator(new ElaIntValidator(0, 255, this));
    d->_thridText = new ElaText("蓝色", this);
    d->_thridText->setTextPixelSize(13);
    connect(d->_thridEdit, &ElaLineEdit::textEdited, d, &ElaColorDialogPrivate::onColorEditChanged);
    QHBoxLayout* thridEditLayout = new QHBoxLayout();
    thridEditLayout->setContentsMargins(0, 0, 0, 0);
    thridEditLayout->addSpacing(6);
    thridEditLayout->addWidget(d->_thridEdit);
    thridEditLayout->addWidget(d->_thridText);
    thridEditLayout->addStretch();

    QVBoxLayout* paramControlLayout = new QVBoxLayout();
    paramControlLayout->setContentsMargins(0, 0, 0, 0);
    paramControlLayout->setSpacing(10);
    paramControlLayout->addLayout(htmlEditLayout);
    paramControlLayout->addWidget(d->_modeComboBox);
    paramControlLayout->addLayout(firstEditLayout);
    paramControlLayout->addLayout(secondEditLayout);
    paramControlLayout->addLayout(thridEditLayout);
    paramControlLayout->addStretch();

    QHBoxLayout* colorControlLayout = new QHBoxLayout();
    colorControlLayout->setSpacing(0);
    colorControlLayout->setContentsMargins(0, 0, 0, 0);
    colorControlLayout->addLayout(colorPickerLayout);
    colorControlLayout->addSpacing(5);
    colorControlLayout->addLayout(colorPreviewLayout);
    colorControlLayout->addSpacing(20);
    colorControlLayout->addLayout(colorValueSliderLayout);
    colorControlLayout->addSpacing(10);
    colorControlLayout->addLayout(paramControlLayout);
    colorControlLayout->addStretch();

    // 基本颜色
    d->_basicColorView = new ElaBaseListView(this);
    d->_basicColorView->setFixedSize(380, 170);
    d->_basicColorView->setSelectionMode(QListView::NoSelection);
    d->_basicColorView->setFlow(QListView::LeftToRight);
    d->_basicColorView->setViewMode(QListView::IconMode);
    d->_basicColorView->setResizeMode(QListView::Adjust);
    d->_basicColorModel = new ElaColorDisplayModel(this);
    d->_basicColorView->setModel(d->_basicColorModel);
    d->_basicColorDelegate = new ElaColorDisplayDelegate(this);
    d->_basicColorView->setItemDelegate(d->_basicColorDelegate);
    d->_initBasicColor();
    connect(d->_basicColorView, &ElaBaseListView::clicked, d, &ElaColorDialogPrivate::onBasicColorViewClicked);

    ElaText* basicColorViewText = new ElaText("基本颜色", this);
    basicColorViewText->setTextPixelSize(14);
    QVBoxLayout* basicColorLayout = new QVBoxLayout();
    basicColorLayout->setContentsMargins(3, 0, 0, 0);
    basicColorLayout->addWidget(basicColorViewText);
    basicColorLayout->addSpacing(15);
    basicColorLayout->addWidget(d->_basicColorView);
    basicColorLayout->addStretch();

    // 自定义颜色
    d->_customColorView = new ElaBaseListView(this);
    d->_customColorView->setFixedSize(200, 170);
    d->_customColorView->setSelectionMode(QListView::SingleSelection);
    d->_customColorView->setFlow(QListView::LeftToRight);
    d->_customColorView->setViewMode(QListView::IconMode);
    d->_customColorView->setResizeMode(QListView::Adjust);
    d->_customColorModel = new ElaColorDisplayModel(this);
    d->_customColorView->setModel(d->_customColorModel);
    d->_customColorDelegate = new ElaColorDisplayDelegate(this);
    d->_customColorView->setItemDelegate(d->_customColorDelegate);
    d->_initCustomColor();
    connect(d->_customColorView, &ElaBaseListView::clicked, d, &ElaColorDialogPrivate::onCustomColorViewClicked);

    // 增加移除按钮
    d->_addCustomColorButton = new ElaToolButton(this);
    d->_addCustomColorButton->setIsTransparent(false);
    d->_addCustomColorButton->setElaIcon(ElaIconType::Plus);
    connect(d->_addCustomColorButton, &ElaToolButton::clicked, d, &ElaColorDialogPrivate::onAddCustomColorButtonClicked);

    d->_removeCustomColorButton = new ElaToolButton(this);
    d->_removeCustomColorButton->setIsTransparent(false);
    d->_removeCustomColorButton->setElaIcon(ElaIconType::Minus);
    connect(d->_removeCustomColorButton, &ElaToolButton::clicked, d, &ElaColorDialogPrivate::onRemoveCustomColorButtonClicked);

    ElaText* customColorViewText = new ElaText("自定义颜色", this);
    customColorViewText->setTextPixelSize(14);
    QHBoxLayout* customButtonLayout = new QHBoxLayout();
    customButtonLayout->setContentsMargins(0, 0, 0, 0);
    customButtonLayout->addWidget(customColorViewText);
    customButtonLayout->addSpacing(35);
    customButtonLayout->addWidget(d->_addCustomColorButton);
    customButtonLayout->addWidget(d->_removeCustomColorButton);
    customButtonLayout->addStretch();

    QVBoxLayout* customColorLayout = new QVBoxLayout();
    customColorLayout->setContentsMargins(3, 0, 0, 0);
    customColorLayout->addLayout(customButtonLayout);
    customColorLayout->addSpacing(5);
    customColorLayout->addWidget(d->_customColorView);
    customColorLayout->addStretch();

    QHBoxLayout* colorDisplayLayout = new QHBoxLayout();
    colorDisplayLayout->setContentsMargins(0, 0, 0, 0);
    colorDisplayLayout->addLayout(basicColorLayout);
    colorDisplayLayout->addLayout(customColorLayout);
    colorDisplayLayout->addStretch();

    // 确定取消按钮
    d->_overButton = new ElaPushButton("确定", this);
    d->_overButton->setBorderRadius(6);
    d->_overButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    d->_overButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    d->_overButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    d->_overButton->setLightTextColor(Qt::white);
    d->_overButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    d->_overButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    d->_overButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    d->_overButton->setDarkTextColor(Qt::white);
    connect(d->_overButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT colorSelected(d->_pCurrentColor);
        close();
    });
    d->_cancelButton = new ElaPushButton("取消", this);
    d->_cancelButton->setBorderRadius(6);
    connect(d->_cancelButton, &ElaPushButton::clicked, this, [=]() {
        close();
    });
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(d->_overButton);
    buttonLayout->addWidget(d->_cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 11, 20);
    mainLayout->addLayout(colorPickerTextLayout);
    mainLayout->addSpacing(3);
    mainLayout->addLayout(colorControlLayout);
    mainLayout->addSpacing(80);
    mainLayout->addLayout(colorDisplayLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
    setCurrentColor(QColor(0x80, 0xFF, 0xEF));
}

ElaColorDialog::~ElaColorDialog()
{
    Q_D(ElaColorDialog);
    delete d->_colorValueSlider->style();
}

void ElaColorDialog::setCurrentColor(QColor currentColor)
{
    Q_D(ElaColorDialog);
    d->_pCurrentColor = currentColor.toHsv();
    d->_updateHtmlEditValue();
    d->_updateEditValue();
    d->_updateColorPreview();
    d->_updateColorValueSlider();
    d->_colorPicker->setSelectedColor(d->_pCurrentColor);
    Q_EMIT pCurrentColorChanged();
}

QColor ElaColorDialog::getCurrentColor() const
{
    Q_D(const ElaColorDialog);
    return d->_pCurrentColor;
}

QList<QColor> ElaColorDialog::getCustomColorList() const
{
    Q_D(const ElaColorDialog);
    return d->_customColorModel->getDisplayColorList();
}

QColor ElaColorDialog::getCustomColor(int index) const
{
    Q_D(const ElaColorDialog);
    return d->_customColorModel->getDisplayColor(index);
}

QString ElaColorDialog::getCurrentColorRGB() const
{
    Q_D(const ElaColorDialog);
    return d->_getHexRgbValue();
}

void ElaColorDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaColorDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    // 背景绘制
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
    painter.drawRect(rect());

    // 按钮背景绘制
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogLayoutArea));
    painter.drawRect(QRect(0, height() - 78, width(), 78));
    painter.restore();
    QDialog::paintEvent(event);
}
