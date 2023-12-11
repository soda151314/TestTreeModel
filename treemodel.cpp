#include "treemodel.h"
#include <QStringList>
#include <QDebug>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{ 
    m_emptyItem = nullptr;
}

TreeModel::~TreeModel()
{
    if(m_emptyItem)
    {
        delete m_emptyItem;
        m_emptyItem = nullptr;
    }
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
   return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::ItemIsEditable)
        return QVariant();

    LPCusTreeNode item = static_cast<LPCusTreeNode>(index.internalPointer());
    return QVariant::fromValue((void *) item);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    LPCusTreeNode parentItem;

    if (!parent.isValid())
        parentItem = m_emptyItem;
    else
        parentItem = static_cast<LPCusTreeNode>(parent.internalPointer());

    if(parentItem != nullptr &&  row >= 0 && parentItem->lstChildNodes.size() > row)
    {
            LPCusTreeNode childItem = parentItem->lstChildNodes.value(row);
            return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    LPCusTreeNode childItem = static_cast<LPCusTreeNode>(index.internalPointer());
    if(childItem == nullptr)
    return QModelIndex();

    LPCusTreeNode parentItem = childItem->parent;
    if (parentItem == m_emptyItem || parentItem == nullptr)
        return QModelIndex();


    if(parentItem->parent == nullptr)
    {
        return QModelIndex();
    }
    else
    {
        for(auto rowIndex = 0; rowIndex < parentItem->parent->lstChildNodes.size(); rowIndex++)
        {
            if(parentItem->parent->lstChildNodes.at(rowIndex) == parentItem)
            return createIndex(rowIndex, index.column(), parentItem);
        }
     }
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    LPCusTreeNode parentItem;
    if (!parent.isValid())
        parentItem = m_emptyItem;
    else
        parentItem = static_cast<LPCusTreeNode>(parent.internalPointer());

    return parentItem ? parentItem->lstChildNodes.size() : 0;
}

void TreeModel::resetModelData(LPCusTreeNode rootItem)
{
    if(m_emptyItem)
    {
        delete m_emptyItem;
        m_emptyItem = nullptr;
    }
    if(m_emptyItem == nullptr)
    {
        m_emptyItem = new CusTreeNode();
    }
    m_emptyItem->parent = nullptr;
    if(rootItem != nullptr)
    {
        m_emptyItem->insertNode(rootItem);
        rootItem->parent = m_emptyItem;
    }
}

void TreeModel::beginResetTreeModel()
{
   beginResetModel();
}

void TreeModel::endResetTreeModel()
{
   endResetModel();
}

void TreeModel::updateModelData()
{
    emit layoutChanged();
}


void TreeModel::startDeletelModelData(const QModelIndex &parent, int first, int last)
{
    beginRemoveRows(parent, first, last);
}

void TreeModel::endDeleteModelData()
{
    endRemoveRows();
}

void TreeModel::startInsertModelData(const QModelIndex &parent, int first, int last)
{
    beginInsertRows(parent, first, last);
}

void TreeModel::endInsertModelData()
{
    endInsertRows();
}

