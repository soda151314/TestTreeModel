#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
class TreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TreeDelegate(QObject *parent = nullptr);
    ~TreeDelegate();

    QWidget* createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const;

	void setEditorData(QWidget *editor,
                               const QModelIndex &index) const;
							   
	void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const;
							  
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};

#endif // TREEDELEGATE_H
