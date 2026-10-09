#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACOLORDIALOGPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACOLORDIALOGPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QObject>
#include <QPixmap>

#include "ElaWidgetToolsDef.h"
class ElaAppBar;
class ElaColorDialog;
class ElaColorPicker;
class ElaBaseListView;
class ElaColorDisplayModel;
class ElaColorDisplayDelegate;
class ElaColorPreview;
class ElaLineEdit;
class ElaComboBox;
class ElaText;
class ElaPushButton;
class ElaToolButton;
class QSlider;
class ElaColorValueSliderStyle;
class ElaColorDialogPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaColorDialog)
    ELA_PROPERTY_D(QColor, CurrentColor)
public:
    explicit ElaColorDialogPrivate(QObject* parent = nullptr);
    ~ElaColorDialogPrivate() override;

    Q_SLOT void onColorPickerColorChanged(const QColor& selectedColor);
    Q_SLOT void onColorValueSliderChanged(int value);

    Q_SLOT void onColorModeChanged(int index);
    Q_SLOT void onHtmlEditFocusOut(const QString& text);
    Q_SLOT void onHtmlEditChanged(const QString& text);
    Q_SLOT void onColorEditChanged(const QString& text);

    Q_SLOT void onBasicColorViewClicked(const QModelIndex& index);
    Q_SLOT void onCustomColorViewClicked(const QModelIndex& index);

    Q_SLOT void onAddCustomColorButtonClicked();
    Q_SLOT void onRemoveCustomColorButtonClicked();

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaAppBar* _appBar{nullptr};
    ElaColorPicker* _colorPicker{nullptr};
    ElaColorPreview* _colorPreview{nullptr};
    QSlider* _colorValueSlider{nullptr};
    ElaColorValueSliderStyle* _colorValueSliderStyle{nullptr};
    ElaLineEdit* _htmlEdit{nullptr};
    ElaComboBox* _modeComboBox{nullptr};
    ElaLineEdit* _firstEdit{nullptr};
    ElaLineEdit* _secondEdit{nullptr};
    ElaLineEdit* _thridEdit{nullptr};
    ElaText* _firstText{nullptr};
    ElaText* _secondText{nullptr};
    ElaText* _thridText{nullptr};

    ElaBaseListView* _basicColorView{nullptr};
    ElaColorDisplayModel* _basicColorModel{nullptr};
    ElaColorDisplayDelegate* _basicColorDelegate{nullptr};

    ElaToolButton* _addCustomColorButton{nullptr};
    ElaToolButton* _removeCustomColorButton{nullptr};
    ElaBaseListView* _customColorView{nullptr};
    ElaColorDisplayModel* _customColorModel{nullptr};
    ElaColorDisplayDelegate* _customColorDelegate{nullptr};

    ElaPushButton* _overButton{nullptr};
    ElaPushButton* _cancelButton{nullptr};

    void _initBasicColor();
    void _initCustomColor();
    void _updateHtmlEditValue();
    void _updateEditValue();
    void _updateColorPreview();
    void _updateColorValueSlider();
    QString _completeColorText(const QString& text) const;
    QString _getHexRgbValue() const;
    QColor _getColorFromEdit() const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACOLORDIALOGPRIVATE_H_

