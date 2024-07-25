#include "ElaWidgetPlugin.h"
#include "ElaLineEdit.h"
#include "ElaCheckBox.h"
#include <QtPlugin>

ElaWidgetPlugin::ElaWidgetPlugin(QObject *parent) : QObject(parent)
{
    widgets.append(new ElaLineEditPlugin(this));
    widgets.append(new ElaCheckBoxPlugin(this));
}

QList<QDesignerCustomWidgetInterface *> ElaWidgetPlugin::customWidgets() const
{
    return widgets;
}


using namespace Qt::StringLiterals;
ElaLineEditPlugin::ElaLineEditPlugin(QObject *parent)
    : QObject(parent)
{
}
void ElaLineEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}
bool ElaLineEditPlugin::isInitialized() const
{
    return initialized;
}
QWidget *ElaLineEditPlugin::createWidget(QWidget *parent)
{
    return new ElaLineEdit(parent);
}
QString ElaLineEditPlugin::name() const
{
    return u"ElaLineEdit"_s;
}
QString ElaLineEditPlugin::group() const
{
    return u"Ela Widgets"_s;
}
QIcon ElaLineEditPlugin::icon() const
{
    return {};
}
QString ElaLineEditPlugin::toolTip() const
{
    return {};
}
QString ElaLineEditPlugin::whatsThis() const
{
    return {};
}
bool ElaLineEditPlugin::isContainer() const
{
    return false;
}
QString ElaLineEditPlugin::domXml() const
{
    return uR"(
<ui language="c++">
  <widget class="ElaLineEdit" name="elalineedit">
)"
//! [11]
R"(
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>100</width>
        <height>100</height>
      </rect>
    </property>
")
//! [11]
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;
}
QString ElaLineEditPlugin::includeFile() const
{
    return u"ElaLineEdit.h"_s;
}


ElaCheckBoxPlugin::ElaCheckBoxPlugin(QObject *parent)
    : QObject(parent)
{
}
void ElaCheckBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}
bool ElaCheckBoxPlugin::isInitialized() const
{
    return initialized;
}
QWidget *ElaCheckBoxPlugin::createWidget(QWidget *parent)
{
    return new ElaCheckBox(parent);
}
QString ElaCheckBoxPlugin::name() const
{
    return u"ElaCheckBox"_s;
}
QString ElaCheckBoxPlugin::group() const
{
    return u"Ela Widgets"_s;
}
QIcon ElaCheckBoxPlugin::icon() const
{
    return {};
}
QString ElaCheckBoxPlugin::toolTip() const
{
    return {};
}
QString ElaCheckBoxPlugin::whatsThis() const
{
    return {};
}
bool ElaCheckBoxPlugin::isContainer() const
{
    return false;
}
QString ElaCheckBoxPlugin::domXml() const
{
    return uR"(
<ui language="c++">
  <widget class="ElaCheckBox" name="elacheckbox">
)"
//! [11]
R"(
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>100</width>
        <height>100</height>
      </rect>
    </property>
")
//! [11]
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;
}
QString ElaCheckBoxPlugin::includeFile() const
{
    return u"ElaCheckBot.h"_s;
}
