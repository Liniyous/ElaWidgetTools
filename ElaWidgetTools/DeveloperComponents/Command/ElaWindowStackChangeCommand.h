#ifndef ELAFRAMEWORK_ELAWINDOWSTACKCHANGECOMMAND_H
#define ELAFRAMEWORK_ELAWINDOWSTACKCHANGECOMMAND_H

#include "ElaActionCommander.h"
#include "ElaWindowPrivate.h"
class ElaWindowStackChangeCommand : public ElaActionCommand
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, UndoStackIndex);
    Q_PRIVATE_CREATE(int, RedoStackIndex);
    Q_PRIVATE_CREATE(ElaWindowPrivate*, WindowPrivate)
public:
    explicit ElaWindowStackChangeCommand(QObject* parent = nullptr);
    ~ElaWindowStackChangeCommand() override;

    void undo() override;
    void redo() override;
};

#endif //ELAFRAMEWORK_ELAWINDOWSTACKCHANGECOMMAND_H
