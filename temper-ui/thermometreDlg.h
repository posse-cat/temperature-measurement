#ifndef THERMOMETREDLG_H
#define THERMOMETREDLG_H
 
#include <QWidget>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>
 
class thermometreDlg : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ getValue WRITE setValue)  //声明属性
public:
    explicit thermometreDlg(QWidget *parent = nullptr);
    qreal getValue();
    void setValue(qreal value);
    void changeValue(qreal value);
 
protected:
    void paintEvent(QPaintEvent *e);
 
public slots:
    void startAnimation();
 
signals:
 
private:
    qreal m_value;
    qreal curValue;
    int m_width;
    QRectF m_rect;
    int maxValue, minValue;
    qreal m_radius;
    QPropertyAnimation *m_valueAnimation;
    void updateRect();
 
 
};
 
#endif // THERMOMETREDLG_H
