#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <treemodel.h>
#include <treedelegate.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // 树相关
    void initTree();
    void initConnection();
    void slotExpandedNode(const QModelIndex &index);
    void slotCollapsedNode(const QModelIndex &index);

    TreeModel *m_treeModel;
    TreeDelegate *m_treeDelegate;
    LPCusTreeNode createNodeByFile(const QString& fileName, const QString& TestData);
    QList<LPCusTreeNode> m_lstNodes;
    LPCusTreeNode m_rootNode = nullptr;
};
#endif // MAINWINDOW_H
