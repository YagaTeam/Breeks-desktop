#ifndef TIMETABLETEXTEDIT_H
#define TIMETABLETEXTEDIT_H
#include "gentextedit.h"

class TimetableTextEdit : public GenTextEdit
{
public:
    TimetableTextEdit();
    void fillCharsAndSetText(QString text, const QJsonArray jArr);
};

#endif // TIMETABLETEXTEDIT_H