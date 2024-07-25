#include "ElaWidgetPlugin.h"

#include "ElaAcrylicUrlCard.h"
#include "ElaAppBar.h"
#include "ElaApplication.h"
#include "ElaBreadcrumbBar.h"
#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaLineEdit.h"



ElaWidgetPlugin::ElaWidgetPlugin(QObject *parent) : QObject(parent)
{
    widgets.append(new ElaLineEditPlugin(this));
    widgets.append(new ElaCheckBoxPlugin(this));
    widgets.append(new ElaAcrylicUrlCardPlugin(this));
    widgets.append(new ElaAppBarPlugin(this));
}
QList<QDesignerCustomWidgetInterface *> ElaWidgetPlugin::customWidgets() const
{
    return widgets;
}

using namespace Qt::StringLiterals;

ElaAcrylicUrlCardPlugin::ElaAcrylicUrlCardPlugin(QObject *parent) : QObject(parent){}
void ElaAcrylicUrlCardPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaAcrylicUrlCardPlugin::isInitialized() const { return initialized; }
QWidget *ElaAcrylicUrlCardPlugin::createWidget(QWidget *parent){ return new ElaAcrylicUrlCard(parent); }
QString ElaAcrylicUrlCardPlugin::name() const { return u"ElaAcrylicUrlCard"_s; }
QString ElaAcrylicUrlCardPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaAcrylicUrlCardPlugin::icon() const{ return {}; }
QString ElaAcrylicUrlCardPlugin::toolTip() const{ return {}; }
QString ElaAcrylicUrlCardPlugin::whatsThis() const { return {}; }
bool ElaAcrylicUrlCardPlugin::isContainer() const { return false; }
QString ElaAcrylicUrlCardPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaAcrylicUrlCard" name="elaAcrylicUrlCard">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaAcrylicUrlCardPlugin::includeFile() const{ return u"ElaAcrylicUrlCard.h"_s; }

ElaAppBarPlugin::ElaAppBarPlugin(QObject *parent) : QObject(parent){}
void ElaAppBarPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaAppBarPlugin::isInitialized() const { return initialized; }
QWidget *ElaAppBarPlugin::createWidget(QWidget *parent){ return new ElaAppBar(parent); }
QString ElaAppBarPlugin::name() const { return u"ElaAppBar"_s; }
QString ElaAppBarPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaAppBarPlugin::icon() const{ return {}; }
QString ElaAppBarPlugin::toolTip() const{ return {}; }
QString ElaAppBarPlugin::whatsThis() const { return {}; }
bool ElaAppBarPlugin::isContainer() const { return true; }
QString ElaAppBarPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaAppBar" name="elaAppBar">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaAppBarPlugin::includeFile() const{ return u"ElaAppBar.h"_s; }

ElaBreadcrumbBarPlugin::ElaBreadcrumbBarPlugin(QObject *parent) : QObject(parent){}
void ElaBreadcrumbBarPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaBreadcrumbBarPlugin::isInitialized() const { return initialized; }
QWidget *ElaBreadcrumbBarPlugin::createWidget(QWidget *parent){ return new ElaBreadcrumbBar(parent); }
QString ElaBreadcrumbBarPlugin::name() const { return u"ElaBreadcrumbBar"_s; }
QString ElaBreadcrumbBarPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaBreadcrumbBarPlugin::icon() const{ return {}; }
QString ElaBreadcrumbBarPlugin::toolTip() const{ return {}; }
QString ElaBreadcrumbBarPlugin::whatsThis() const { return {}; }
bool ElaBreadcrumbBarPlugin::isContainer() const { return false; }
QString ElaBreadcrumbBarPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaBreadcrumbBar" name="elaBreadcrumbBar">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaBreadcrumbBarPlugin::includeFile() const{ return u"ElaBreadcrumbBar.h"_s; }

ElaComboBoxPlugin::ElaComboBoxPlugin(QObject *parent) : QObject(parent){}
void ElaComboBoxPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaComboBoxPlugin::isInitialized() const { return initialized; }
QWidget *ElaComboBoxPlugin::createWidget(QWidget *parent){ return new ElaComboBox(parent); }
QString ElaComboBoxPlugin::name() const { return u"ElaComboBox"_s; }
QString ElaComboBoxPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaComboBoxPlugin::icon() const{ return {}; }
QString ElaComboBoxPlugin::toolTip() const{ return {}; }
QString ElaComboBoxPlugin::whatsThis() const { return {}; }
bool ElaComboBoxPlugin::isContainer() const { return false; }
QString ElaComboBoxPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaComboBox" name="elaComboBox">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaComboBoxPlugin::includeFile() const{ return u"ElaComboBox.h"_s; }

ElaCheckBoxPlugin::ElaCheckBoxPlugin(QObject *parent) : QObject(parent){}
void ElaCheckBoxPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaCheckBoxPlugin::isInitialized() const{ return initialized; }
QWidget *ElaCheckBoxPlugin::createWidget(QWidget *parent){ return new ElaCheckBox(parent); }
QString ElaCheckBoxPlugin::name() const{ return u"ElaCheckBox"_s; }
QString ElaCheckBoxPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaCheckBoxPlugin::icon() const{ return {}; }
QString ElaCheckBoxPlugin::toolTip() const{ return {}; }
QString ElaCheckBoxPlugin::whatsThis() const{ return {}; }
bool ElaCheckBoxPlugin::isContainer() const{ return false; }
QString ElaCheckBoxPlugin::domXml() const
{return uR"(
<ui language="c++">
  <widget class="ElaCheckBox" name="elacheckbox">
)"
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
)"_s;}
QString ElaCheckBoxPlugin::includeFile() const{ return u"ElaCheckBot.h"_s; }

ElaContentDialogPlugin::ElaContentDialogPlugin(QObject *parent) : QObject(parent){}
void ElaContentDialogPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaContentDialogPlugin::isInitialized() const { return initialized; }
QWidget *ElaContentDialogPlugin::createWidget(QWidget *parent){ return new ElaContentDialog(parent); }
QString ElaContentDialogPlugin::name() const { return u"ElaContentDialog"_s; }
QString ElaContentDialogPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaContentDialogPlugin::icon() const{ return {}; }
QString ElaContentDialogPlugin::toolTip() const{ return {}; }
QString ElaContentDialogPlugin::whatsThis() const { return {}; }
bool ElaContentDialogPlugin::isContainer() const { return false; }
QString ElaContentDialogPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaContentDialog" name="elaContentDialog">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaContentDialogPlugin::includeFile() const{ return u"ElaContentDialog.h"_s; }

ElaDockWidgetPlugin::ElaDockWidgetPlugin(QObject *parent) : QObject(parent){}
void ElaDockWidgetPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaDockWidgetPlugin::isInitialized() const { return initialized; }
QWidget *ElaDockWidgetPlugin::createWidget(QWidget *parent){ return new ElaDockWidget(parent); }
QString ElaDockWidgetPlugin::name() const { return u"ElaDockWidget"_s; }
QString ElaDockWidgetPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaDockWidgetPlugin::icon() const{ return {}; }
QString ElaDockWidgetPlugin::toolTip() const{ return {}; }
QString ElaDockWidgetPlugin::whatsThis() const { return {}; }
bool ElaDockWidgetPlugin::isContainer() const { return false; }
QString ElaDockWidgetPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaDockWidget" name="elaDockWidget">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaDockWidgetPlugin::includeFile() const{ return u"ElaDockWidget.h"_s; }

ElaLineEditPlugin::ElaLineEditPlugin(QObject *parent) : QObject(parent){}
void ElaLineEditPlugin::initialize(QDesignerFormEditorInterface *){ if (initialized) return; initialized = true;}
bool ElaLineEditPlugin::isInitialized() const { return initialized; }
QWidget *ElaLineEditPlugin::createWidget(QWidget *parent){ return new ElaLineEdit(parent); }
QString ElaLineEditPlugin::name() const { return u"ElaLineEdit"_s; }
QString ElaLineEditPlugin::group() const { return u"Ela Widgets"_s; }
QIcon ElaLineEditPlugin::icon() const{ return {}; }
QString ElaLineEditPlugin::toolTip() const{ return {}; }
QString ElaLineEditPlugin::whatsThis() const { return {}; }
bool ElaLineEditPlugin::isContainer() const { return false; }
QString ElaLineEditPlugin::domXml() const { return uR"(
<ui language="c++">
  <widget class="ElaLineEdit" name="elalineedit">
)"
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
R"(
    <property name="toolTip">
      <string>The current time</string>
    </property>
    <property name="whatsThis">
      <string>The analog clock widget displays the current time.</string>
    </property>
  </widget>
</ui>
)"_s;}
QString ElaLineEditPlugin::includeFile() const{ return u"ElaLineEdit.h"_s; }
