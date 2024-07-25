#ifndef ELAWIDGETPLUGIN_H
#define ELAWIDGETPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>
#include "ElaFlowLayout.h"


class ElaWidgetPlugin: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
    ElaWidgetPlugin(QObject *parent = 0);
    QList<QDesignerCustomWidgetInterface*> customWidgets() const override;
private:
    QList<QDesignerCustomWidgetInterface*> widgets;
};

class ElaAcrylicUrlCardPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaAcrylicUrlCardPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaAppBarPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaAppBarPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaBreadcrumbBarPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaBreadcrumbBarPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaCheckBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaCheckBoxPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false; };

class ElaComboBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaComboBoxPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaContentDialogPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaContentDialogPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaDockWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaDockWidgetPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaDoubleSpinBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaDoubleSpinBoxPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaFlowLayoutPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaFlowLayoutPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;

    QWidget * createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};

class ElaLineEditPlugin : public QObject, public QDesignerCustomWidgetInterface
{   Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public: explicit ElaLineEditPlugin(QObject *parent = nullptr);bool isContainer() const override;bool isInitialized() const override;QIcon icon() const override;QString domXml() const override;QString group() const override;QString includeFile() const override;QString name() const override;QString toolTip() const override;QString whatsThis() const override;QWidget *createWidget(QWidget *parent) override;void initialize(QDesignerFormEditorInterface *core) override;
private: bool initialized = false;};


#endif // ELAWIDGETPLUGIN_H