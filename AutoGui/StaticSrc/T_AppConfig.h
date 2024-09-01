#ifndef T_APPCONFIG_H
#define T_APPCONFIG_H

// 引入 Ela 系列组件的头文件
#include <ElaApplication.h>        // 程序初始化
#include <ElaAppBar.h>             // 窗口顶部标题栏，支持拖动窗口，最小化、最大化、关闭窗口、调整窗口尺寸
#include <ElaAcrylicUrlCard.h>     // 带图片的交互式亚克力卡片，支持 URL 跳转
#include <ElaBreadcrumbBar.h>      // 面包屑组件，自动处理点击事件
#include <ElaCheckBox.h>           // 勾选框
#include <ElaComboBox.h>           // 下拉框
#include <ElaMultiSelectComboBox.h>// 多选下拉框
#include <ElaContentDialog.h>      // 带遮罩的对话框
#include <ElaDockWidget.h>         // 停靠窗口
#include <ElaSpinBox.h>            // 微调框
#include <ElaDoubleSpinBox.h>      // 浮点数微调框
#include <ElaDxgiManager.h>        // DXGI 采集器，支持自动选择采集设备，效率远高于原生采集
#include <ElaEventBus.h>           // 事件总线
#include <ElaFlowLayout.h>         // 流式布局，支持动画
#include <ElaIcon.h>               // 图标
#include <ElaIconButton.h>         // 图标按钮
#include <ElaImageCard.h>          // 图片卡片
#include <ElaInteractiveCard.h>    // 带图片的交互式透明卡片
#include <ElaLineEdit.h>           // 输入框
#include <ElaListView.h>           // 列表视图
#include <ElaLog.h>                // 消息日志
#include <ElaMenu.h>               // 菜单
#include <ElaMenuBar.h>            // 菜单栏
#include <ElaMessageBar.h>         // 弹出式信息栏，支持八方向，锚定位置
#include <ElaMessageButton.h>      // 弹出信息按钮
//#include <ElaPivot.h>              // 轴转导航
#include <ElaPlainTextEdit.h>      // 文本编辑框
#include <ElaPushButton.h>         // 按钮
#include <ElaPopularCard.h>        // 热门卡片
#include <ElaProgressBar.h>        // 进度条
#include <ElaPromotionCard.h>      // 促销卡片
#include <ElaPromotionView.h>      // 促销卡片视窗
//#include <ElaSwitchButton.h>       // 切换按钮
#include <ElaToggleButton.h>
#include <ElaSuggestBox.h>         // 建议搜索框
#include <ElaRadioButton.h>        // 单选按钮
#include <ElaNavigationRouter.h>   // 路由跳转
#include <ElaToggleSwitch.h>       // 开关按钮
#include <ElaTabBar.h>             // 选项卡，谷歌浏览器风格
#include <ElaTabWidget.h>          // 选项卡页面，谷歌浏览器风格
#include <ElaTableView.h>          // 表格视图
#include <ElaTreeView.h>           // 树型视图
#include <ElaToolButton.h>         // 带下拉菜单的工具按钮
#include <ElaToolBar.h>            // 工具栏
#include <ElaTheme.h>              // 主题管理器
#include <ElaReminderCard.h>       // 带图片的提醒卡片
#include <ElaScrollbar.h>          // 滚动条
#include <ElaScrollArea.h>         // 滚动区域，可设置鼠标拖动
#include <ElaScrollPage.h>         // 滚动页面，自带堆栈页面和面包屑导航
#include <ElaScrollPageArea.h>     // 滚动页面区域组件
#include <ElaSlider.h>             // 拖动条
#include <ElaStatusBar.h>          // 状态栏
#include <ElaExponentialBlur.h>    // 指数模糊
#include <ElaText.h>               // Text 文本
#include <ElaGraphicsScene.h>      // 高级场景，大量实用 API
#include <ElaGraphicsView.h>       // 高级视图，按键缩放、拖动
#include <ElaGraphicsItem.h>       // 高级图元，大量实用 API
#include <ElaWindow.h>             // 带导航栏的无边框窗口
#include <ElaWidget.h>             // 无边框模态窗口

#include <QMutex>                  // Qt 线程互斥锁

class T_AppConfig {
public:
    static bool getEditMode();            // 获取编辑模式状态
    static void setEditMode(bool mode);   // 设置编辑模式状态

private:
    T_AppConfig() = default;  // 禁止实例化
    static bool isEditMode;   // 保存编辑模式状态
    static QMutex mutex;      // 用于线程安全的互斥锁
};

#endif // T_APPCONFIG_H
