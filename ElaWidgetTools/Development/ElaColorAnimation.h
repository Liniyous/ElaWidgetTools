#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACOLORANIMATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACOLORANIMATION_H_

#include <QAbstractAnimation>
#include <QColor>
#include <QElapsedTimer>
#include <QHash>
#include <QPersistentModelIndex>
#include <QPointer>

class QWidget;
class ElaColorAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    // 同一控件的绘制区域通过 role 隔离 模型项和菜单项分别通过 index 和 subject 隔离
    static QColor getColor(const QWidget* widget, const QColor& target, const QByteArray& role = "Background", const QModelIndex& index = {}, const QObject* subject = nullptr);
    static QColor getColor(const QWidget* widget, const QColor& target, const QByteArray& role, const QModelIndex& index, const QObject* subject, bool isImmediate);
    static bool getIsAnimationEnabled(const QWidget* widget);
    static void beginThemeTransition();
    int duration() const override;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void updateCurrentTime(int currentTime) override;

private:
    struct ColorKey {
        QByteArray role;                 // 控件内绘制区域的稳定名称
        QPersistentModelIndex index;     // 模型项身份 随模型行移动保持关联
        const QObject* subject{nullptr}; // QAction 等对象的身份 不参与所有权

        bool operator==(const ColorKey& other) const;
        friend uint qHash(const ColorKey& key, uint seed)
        {
            return ::qHash(key.role, seed) ^ ::qHash(key.index, seed) ^ ::qHash(key.subject, seed);
        }
    };
    struct ColorState {
        QColor current;                  // 当前实际绘制颜色
        QColor start;                    // 本次过渡的起始颜色
        QColor target;                   // 当前交互状态对应的目标颜色
        QPointer<const QObject> subject; // 用于识别已销毁的菜单项
        quint64 lastUse{0};              // 最近一次绘制序号 用于限制缓存大小
        quint64 themeRevision{0};        // 此颜色已处理的主题版本
        qint64 startTime{0};             // 过渡起始毫秒数 主题使用统一时钟 交互使用控件动画时钟
        bool isModelItem{false};         // 是否需要检查模型索引失效
        bool isRunning{false};           // 当前颜色是否仍在过渡
        bool isThemeTransition{false};   // 当前过渡是否按主题切换时刻计算
    };

    explicit ElaColorAnimation(QWidget* widget);
    static ElaColorAnimation* _getAnimation(const QWidget* widget);
    QColor _getColor(const ColorKey& key, const QColor& target, bool isImmediate);
    static void _updateColor(ColorState& color, qint64 currentTime);
    void _syncFocusProxy();
    void _pruneColors();
    void _updateWidget();

    QPointer<QWidget> _widget;
    QPointer<QWidget> _focusProxy;
    QHash<ColorKey, ColorState> _colors;
    quint64 _useSerial{0};
    static constexpr int _transitionDuration{83};
    static QElapsedTimer _themeClock;
    static qint64 _themeChangeTime;
    static quint64 _themeRevision;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELACOLORANIMATION_H_
