#include "treedelegate.h"
#include <QDebug>
#include <QLineEdit>
#include <QPainter>
#include <QRegExpValidator>
#include "treedata.h"

#define DIR_NAME_REGEXP      "[^\\s\\\\/:.\\*\\?\\\"<>\\|](\\x20|[^\\s\\\\/:.\\*\\?\\\"<>\\|])*[^\\s\\\\/:.\\*\\?\\\"<>\\|\\.]$"
#define FILE_NAME_REGEXP     "[^\\s\\\\/:\\*\\?\\\"<>\\|](\\x20|[^\\s\\\\/:\\*\\?\\\"<>\\|])*[^\\s\\\\/:\\*\\?\\\"<>\\|\\.]$"

TreeDelegate::TreeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{ 

}

TreeDelegate::~TreeDelegate()
{
   
}

QWidget* TreeDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    if(index.isValid() && index.column() == 0)
    {
        LPCusTreeNode nodeData = (LPCusTreeNode)(index.model()->data(index).value<void*>());
        if(nodeData != nullptr)
        {
            //qDebug() << "createEditor";
            QLineEdit *edit = new QLineEdit(parent);
            QRegExp rx2(FILE_NAME_REGEXP);
            QRegExpValidator *pRep = new QRegExpValidator(rx2, edit);
            edit->setValidator(pRep);
            edit->setMaxLength(64);
            return edit;
        }
    }
    return NULL;
}
 
void TreeDelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    //qDebug() << "updateEditorGeometry";
    editor->setGeometry(option.rect);
}
 
void TreeDelegate::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        qDebug() << "setEditorData";
        LPCusTreeNode nodeData = (LPCusTreeNode)(index.model()->data(index).value<void*>());
        QLineEdit *edit = (QLineEdit *)editor;
        if(nodeData->parent != nullptr && nodeData->parent->parent != nullptr)
        {
            edit->move(edit->pos().x() + 35, edit->pos().y());
            edit->setFixedWidth(edit->size().width() - 35);
        }
        else
        {
            edit->move(edit->pos().x() + 20, edit->pos().y());
            edit->setFixedWidth(edit->size().width() - 20);
        }
        edit->setText(nodeData->qsNodeName);
	}
}
 
void TreeDelegate::setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const
{
	
    if(index.column() == 0)
    {
        qDebug() << "setModelData";
		QLineEdit *edit = (QLineEdit *)editor;
		QString value = edit->text();
        if(value.isEmpty())
            return;
        LPCusTreeNode nodeData = (LPCusTreeNode)(index.model()->data(index).value<void*>());
        if(nodeData != nullptr) {
            nodeData->rename(value);
            emit model->dataChanged(index, index);
        }
	}
}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);
    LPCusTreeNode nodeData = (LPCusTreeNode)(index.model()->data(index).value<void*>());
    painter->setRenderHints(QPainter::SmoothPixmapTransform);

    if (index.column() == 0) {
		QString strIcon = "";
        if(nodeData == nullptr)
            return;

        if(nodeData->bExpanded)
            strIcon = ":/res/png/file_open.png";
        else
            strIcon = ":/res/png/file_close.png";


		// 绘制图标
        QPixmap pixmap(strIcon);
        pixmap =  pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter->setRenderHints(QPainter::SmoothPixmapTransform);
        QRect rect = QRect(QPoint(option.rect.left() + 2, option.rect.top() + 4), QSize(15, 15));
        painter->drawPixmap(rect.left(), rect.top(), rect.width(), rect.height(), pixmap);


        // 绘制文本
        QString strText = nodeData->qsNodeName;

        if(nodeData->parent != nullptr && nodeData->parent->parent != nullptr)
            painter->drawText(QRect(QPoint(option.rect.left() + 25, option.rect.top()), \
                                    QSize(option.rect.width() - 25, option.rect.height())), option.displayAlignment, QString("%1. %2").arg(index.row() + 1).arg(strText));
        else
            painter->drawText(QRect(QPoint(option.rect.left() + 25, option.rect.top()), \
                                    QSize(option.rect.width() - 25, option.rect.height())), option.displayAlignment, QString("%1").arg(strText));
    }
}


QSize TreeDelegate::sizeHint ( const QStyleOptionViewItem & option,
                 const QModelIndex & index ) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(20);
    return size;
}
