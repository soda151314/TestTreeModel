#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initConnection();
    initTree();
}

MainWindow::~MainWindow()
{
    delete ui;
}


LPCusTreeNode MainWindow::createNodeByFile(const QString& fileName, const QString& TestData)
{
    LPCusTreeNode pNode = new CusTreeNode();
    pNode->qsNodeName = fileName;
    pNode->qsNodeTestData = TestData;
    m_lstNodes.push_back(pNode);
    return pNode;
}


void MainWindow::slotExpandedNode(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    LPCusTreeNode nodeData = (LPCusTreeNode)(index.model()->data(index).value<void*>());
    nodeData->bExpanded = true;
    ui->treeView->expand(index);
}

void MainWindow::slotCollapsedNode(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    LPCusTreeNode nodeData = (LPCusTreeNode)(index.model()->data(index).value<void*>());
    nodeData->bExpanded = false;
    ui->treeView->collapse(index);
}

void MainWindow::initConnection()
{
    connect(ui->treeView, &QTreeView::expanded, this, &MainWindow::slotExpandedNode);
    connect(ui->treeView, &QTreeView::collapsed, this, &MainWindow::slotCollapsedNode);
}

void MainWindow::initTree()
{
    // 创建委托
    m_treeDelegate = new TreeDelegate;
    ui->treeView->setItemDelegate(m_treeDelegate);

    // 创建model
    m_treeModel = new TreeModel;
    ui->treeView->setModel(m_treeModel);

    // 设置UI
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->treeView->setExpandsOnDoubleClick(false);

    // 构造数据
    // -root
    //  ++++node1
    //  ----node2
    //      ++++node3
    //      ++++node4

    m_rootNode = createNodeByFile("", "");
    auto node1 = createNodeByFile("文件1", "1111");
    m_rootNode->insertNode(node1);
    auto node2 = createNodeByFile("文件2", "2222");
    m_rootNode->insertNode(node2);
    auto node3 = createNodeByFile("文件3", "3333");
    node2->insertNode(node3);
    auto node4 = createNodeByFile("文件4", "4444");
    node2->insertNode(node4);
    auto node5 = createNodeByFile("文件5", "5555");
    m_rootNode->insertNode(node5);
    // 节点5下面放入10万个节点试试
    for(int idx = 0; idx < 100000; ++idx) {
        auto node = createNodeByFile(QString("文件%1").arg(idx), "节点5的子文件");
        node5->insertNode(node);
    }
    m_treeModel->beginResetTreeModel();
    m_treeModel->resetModelData(m_rootNode);
    m_treeModel->endResetTreeModel();

    // 选中第一行
    QModelIndex rootIndex = m_treeModel->index(0, 0, QModelIndex());
    if(rootIndex.isValid())
    {
        ui->treeView->setCurrentIndex(rootIndex);
        ui->treeView->expand(rootIndex);
    }
}
