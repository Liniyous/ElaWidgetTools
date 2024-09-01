#include "DxgiWidget.h"
DxgiWidget::DxgiWidget(QWidget* parent)
    : QWidget{parent}
{
    
    
   
}
DxgiWidget::DxgiWidget(QWidget* parent,T_ElaScreen* _elaScreenPage)
    : QWidget{parent}
{
    
    
    QVBoxLayout* centerLayout = new QVBoxLayout(this);
    //创建开启vnc按钮
    //  _toggleSwitch = new ElaToggleSwitch(this);
    // ElaScrollPageArea* toggleSwitchArea = new ElaScrollPageArea(this);
    // toggleSwitchArea->setMinimumSize(200,20);
    // toggleSwitchArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    // QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(toggleSwitchArea);
    // ElaText* toggleSwitchText = new ElaText("开启VNC", this);
    // toggleSwitchText->setTextPixelSize(15);
    // toggleSwitchText->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    // toggleSwitchLayout->addWidget(toggleSwitchText);
    // toggleSwitchLayout->addWidget(_toggleSwitch);
    // toggleSwitchLayout->addSpacing(10);
 
    // centerLayout->addWidget(toggleSwitchArea);
    
    centerLayout->addWidget(createPageArea("开启VNC","切换开启按钮"));
    centerLayout->addWidget(createPageArea("OCR识别","切换开启按钮"));
    centerLayout->addWidget(createPageArea("目标检测","切换开启按钮"));
    t_ElaScreen=_elaScreenPage;
    centerLayout->addWidget(createPageArea("采集对象设备","下拉框"));
    centerLayout->addWidget(createPageArea("采集屏幕","下拉框"));
    
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
}

ElaScrollPageArea* DxgiWidget::createPageArea(QString text,QString widgetType){
    ElaScrollPageArea* pageArea = new ElaScrollPageArea(this);
    QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(pageArea);
    pageArea->setMinimumSize(200,20);
    pageArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    if(widgetType=="下拉框" && text== "采集屏幕"){
        _outputComboBox=new ElaComboBox(this);
        _outputComboBox->addItems(t_ElaScreen->dxgiManager->getOutputDeviceList());
        _outputComboBox->setCurrentIndex(t_ElaScreen->dxgiManager->getOutputDeviceID());
        toggleSwitchLayout->addWidget(_outputComboBox);
        connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        t_ElaScreen->dxgiManager->setOutputDeviceID(index);
        t_ElaScreen->_dxgiScreen->update();
        });
       return pageArea;
    }else if(widgetType=="下拉框" && text== "采集对象设备"){
        ElaComboBox* _t= new ElaComboBox(this);
        toggleSwitchLayout->addWidget(_t);
        _t->addItems(t_ElaScreen->dxgiManager->getDxDeviceList());
        _t->setCurrentIndex(t_ElaScreen->dxgiManager->getDxDeviceID());

        connect(_t, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
            t_ElaScreen->dxgiManager->setDxDeviceID(index);
            _outputComboBox->blockSignals(true);
            _outputComboBox->clear();
            _outputComboBox->addItems(t_ElaScreen->dxgiManager->getOutputDeviceList());
            _outputComboBox->setCurrentIndex(t_ElaScreen->dxgiManager->getOutputDeviceID());
            _outputComboBox->blockSignals(false);
            t_ElaScreen->_dxgiScreen->update();
        });
       return pageArea;
    }
    ElaText* toggleSwitchText = new ElaText(text, this);
    toggleSwitchText->setTextPixelSize(15);
    toggleSwitchText->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    toggleSwitchLayout->addWidget(toggleSwitchText);
    if(widgetType=="切换开启按钮" && text== "开启VNC"){
        ElaToggleSwitch* _t=new ElaToggleSwitch(this);
        toggleSwitchLayout->addWidget(_t);
       connect(_t, &ElaToggleSwitch::toggled, this, [=](bool checked) {
            if(checked){
                
            }
        });
    }else  if(widgetType=="切换开启按钮" && text== "OCR识别"){
        ElaToggleSwitch* _t=new ElaToggleSwitch(this);
        toggleSwitchLayout->addWidget(_t);
       connect(_t, &ElaToggleSwitch::toggled, this, [=](bool checked) {
            if(checked){
                
            }
        });
    }else if(widgetType=="切换开启按钮" && text== "目标检测"){
        ElaToggleSwitch* _t=new ElaToggleSwitch(this);
        toggleSwitchLayout->addWidget(_t);
       connect(_t, &ElaToggleSwitch::toggled, this, [=](bool checked) {
            if(checked){
                
            }
        });
    } 
    toggleSwitchLayout->addSpacing(10);
    return pageArea;
}

DxgiWidget::~DxgiWidget()
{
}
