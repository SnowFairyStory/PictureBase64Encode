#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->imgPath = "";
    this->lastPath = ".";

    ui->setupUi(this);

    ui->status->setReadOnly(true);

    ui->imgLabel->installEventFilter(this);
    ui->imgLabel->setAcceptDrops(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_base64Encode_clicked()
{
    if (this->imgPath.isEmpty()) {
        QMessageBox::warning(this, "提示", "图片路径无效" + this->getCurrentTime());
        return;
    }

    this->ImgToBase64();
}


void Widget::on_selectImage_clicked()
{
    QString suffixList = "*.jpg *.jpeg *.png *.gif *tiff *svg *pdf";
    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", this->lastPath, suffixList);
    if (!fileName.isEmpty()) {
        this->imgPath = fileName;
        this->lastPath = QFileInfo(imgPath).absoluteFilePath();
        ui->status->setText("已选中: " + this->imgPath + this->getCurrentTime());
    }
}

QString Widget::getCurrentTime()
{
    return QString(" [%1]").arg(QDateTime::currentDateTime().toString("yy-M-d h:m:s"));
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->imgLabel) {
        if (event->type() == QEvent::DragEnter) {
            // [[2]]: 当拖放时鼠标进入label时, label接受拖放的动作
            QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
            dee->acceptProposedAction();
            return true;
        } else if (event->type() == QEvent::Drop) {
            // [[3]]: 当放操作发生后, 取得拖放的数据
            QDropEvent *de = dynamic_cast<QDropEvent *>(event);
            QList<QUrl> urls = de->mimeData()->urls();

            if (urls.isEmpty()) { return true; }
            this->imgPath = urls.first().toLocalFile();
            this->lastPath = QFileInfo(this->imgPath).absoluteFilePath();
            this->ImgToBase64();

            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void Widget::ImgToBase64()
{
    QString imgTemplate = "<img src=\"data:image/%1;base64,%2\">";

    QImage image(this->imgPath);
    QByteArray ba;
    QBuffer buf(&ba);
    const char* suffix = QFileInfo(this->imgPath).suffix().toStdString().c_str();
    image.save(&buf, suffix);
    QByteArray bytes = ba.toBase64();
    buf.close();


    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    clipboard->setText(imgTemplate.arg(suffix).arg(bytes.data()));       //设置剪贴板内容

    ui->status->setText("结果已复制到剪切板" + this->getCurrentTime());

    // [[4]]: 在label上显示拖放的图片
    QImage imageLabel(this->imgPath); // QImage对I/O优化过, QPixmap对显示优化
    if (!imageLabel.isNull()) {
        imageLabel = imageLabel.scaled(ui->imgLabel->size(),
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
        ui->imgLabel->setPixmap(QPixmap::fromImage(imageLabel));
    }
}
