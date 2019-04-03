

#ifndef QCLICABLELABEL_H
#define QCLICABLELABEL_H

#include <QLabel>

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

signals:
    void LeftClicked();

protected:
    void mouseReleaseEvent(QMouseEvent* event);

};

#endif //QCLICABLELABEL_H
