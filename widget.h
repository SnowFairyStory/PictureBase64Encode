#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBuffer>
#include <QClipboard>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QEvent>
#include <QDropEvent>
#include <QUrl>
#include <QList>
#include <QMimeData>
#include <QDateTime>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
void ImgToBase64();
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:

    void on_base64Encode_clicked();

    void on_selectImage_clicked();

    void on_encodeFromClipboard_clicked();

private:
    Ui::Widget *ui;
    QString getCurrentTime();

private:
    // 选中的图片路径
    QString imgPath;

    // 上一次打开的路径
    QString lastPath;
};
#endif // WIDGET_H
