#pragma once
#include<QString>
#include<QList>

typedef struct CusTreeNode
{
    CusTreeNode* parent;    // 父节点
    QString qsNodeName;     // 节点名称
    QString qsNodeTestData; // 节点测试数据
    bool bExpanded;     // 是否展开节点
    QList<CusTreeNode*> lstChildNodes;


    CusTreeNode()
    {
        parent = nullptr;
        qsNodeName = "";
        qsNodeTestData = "";
        bExpanded = false;
        lstChildNodes.clear();
    }

    void insertNode(CusTreeNode* pNode, int index = -1)
    {
        if(index < 0)
            lstChildNodes.append(pNode);
        else
            lstChildNodes.insert(index, pNode);

        pNode->parent = this;
    }

    void removeNode(CusTreeNode* pNode)
    {
       lstChildNodes.removeOne(pNode);
    }

    int childNodeIndex(CusTreeNode* pNode)
    {
        return lstChildNodes.indexOf(pNode);
    }

    void rename(const QString& qsNewName)
    {
        qsNodeName = qsNewName;
    }

}CusTreeNode, *LPCusTreeNode;
