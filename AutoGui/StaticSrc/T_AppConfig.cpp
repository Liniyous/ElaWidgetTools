// T_AppConfig.cpp

#include "T_AppConfig.h"

// 定义静态成员
bool T_AppConfig::isEditMode = true;  // 默认值
QMutex T_AppConfig::mutex;            // 初始化线程安全保护

// 实现静态函数
void T_AppConfig::setEditMode(bool mode) {
    QMutexLocker locker(&mutex);
    isEditMode = mode;
}

bool T_AppConfig::getEditMode() {
    QMutexLocker locker(&mutex);
    return isEditMode;
}
