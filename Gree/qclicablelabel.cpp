#include "qclicablelabel.h"


ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {


}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        emit LeftClicked();
    QLabel::mouseReleaseEvent(event);
}
