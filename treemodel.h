#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "treedata.h"

class TreeItem;
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    ////////////////业务相关///////////////////////////
    // 重新构建整棵树,设置虚假节点
    void resetModelData(LPCusTreeNode rootItem);
    // 开始重新加载model
    void beginResetTreeModel();
    // 结束重新加载
    void endResetTreeModel();
    // 刷新指定节点
    void updateModelData();
    // 删除刷新
    void startDeletelModelData(const QModelIndex &parent, int first, int last);
    void endDeleteModelData();
    // 插入刷新
    void startInsertModelData(const QModelIndex &parent, int first, int last);
    void endInsertModelData();

private:
    LPCusTreeNode m_emptyItem;     // (-1,-1)的虚假节点
};

#endif // TREEMODEL_H
