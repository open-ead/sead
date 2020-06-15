#ifndef SEAD_TREENODE_H_
#define SEAD_TREENODE_H_

#include <basis/seadTypes.h>

namespace sead {

class TreeNode
{
  public:
    TreeNode();

    void clearChildLinksRecursively_();
    void clearLinks();
    s32 countChildren() const;
    void detachAll();
    void detachSubTree();
    TreeNode* findRoot();
    const TreeNode* findRoot() const;
    void insertAfterSelf(TreeNode*);
    void insertBeforeSelf(TreeNode*);
    void pushBackChild(TreeNode*);
    void pushBackSibling(TreeNode*);
    void pushFrontChild(TreeNode*);

    TreeNode* mParent;
    TreeNode* mChild;
    TreeNode* mNext;
    TreeNode* mPrev;
};

template <typename T>
class TTreeNode : public TreeNode
{
public:
    T mData;
};

} // namespace sead

#endif // SEAD_TREENODE_H_
