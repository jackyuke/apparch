#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <algorithm>
typedef enum
{
    Red,
    Black
}RBTreeNodeColor;

template<typename T>
class RBTreeNode
{
public:
    typedef RBTreeNode<T> ThisNodeType;
    typedef T ValueType;
    RBTreeNode()
        : m_color(Red)
        , m_parent(nullptr)
        , m_leftChild(nullptr)
        , m_rightChild(nullptr)
    {
    }
    ~RBTreeNode()
    {
        m_parent = nullptr;
        if (m_leftChild)
        {
            delete m_leftChild;
            m_leftChild = nullptr;
        }
        if (m_rightChild)
        {
            delete m_rightChild;
            m_rightChild = nullptr;
        }
    }
    void SetColor(RBTreeNodeColor color)
    {
        m_color = color;
    }
    RBTreeNodeColor GetColor() const
    {
        return m_color;
    }
    void SetParent(ThisNodeType* parent)
    {
        m_parent = parent;
    }
    ThisNodeType* GetParent() const
    {
        return m_parent;
    }
    void SetValue(ValueType value)
    {
        m_value = value;
    }
    ValueType GetValue() const
    {
        return m_value;
    }
    ThisNodeType* GetLeftChild() const
    {
        return m_leftChild;
    }
    void SetLeftChild(ThisNodeType* node)
    {
        m_leftChild = node;
    }
    ThisNodeType* GetRightChild() const
    {
        return m_rightChild;
    }
    void SetRightChild(ThisNodeType* node)
    {
        m_rightChild = node;
    }
    ThisNodeType* RotateLeft(ThisNodeType *root)
    {
        ThisNodeType *parent = GetParent();
        if (this->m_rightChild == nullptr)
            return root; // do not rotate in case of root or no right child
        if (parent)
        {
            if (parent->m_leftChild == this)
                parent->m_leftChild = this->m_rightChild;
            else
                parent->m_rightChild = this->m_rightChild;
        }
        else
            root = this->m_rightChild;
        this->m_rightChild->m_parent = parent;
        this->m_parent = this->m_rightChild;
        this->m_rightChild = this->m_parent->m_leftChild;
        if (this->m_parent->m_leftChild)
            this->m_parent->m_leftChild->m_parent = this;
        this->m_parent->m_leftChild = this;
        return root;
    }
    ThisNodeType* RotateRight(ThisNodeType *root)
    {
        ThisNodeType *parent = GetParent();
        if (this->m_leftChild == nullptr)
            return root; // do not rotate in case of root or no left child
        if (parent)
        {
            if (parent->m_leftChild == this)
                parent->m_leftChild = this->m_leftChild;
            else
                parent->m_rightChild = this->m_leftChild;
        }
        else
            root = this->m_leftChild;
        this->m_leftChild->m_parent = parent;
        this->m_parent = this->m_leftChild;
        this->m_leftChild = this->m_parent->m_rightChild;
        if (this->m_parent->m_rightChild)
            this->m_parent->m_rightChild->m_parent = this;
        this->m_parent->m_rightChild = this;
        return root;
    }
    ThisNodeType* GetLeftLeaf() const
    {
        ThisNodeType* leftLeaf = nullptr;
        const ThisNodeType* currentNode = this;
        while (currentNode->GetLeftChild())
        {
            leftLeaf = currentNode->GetLeftChild();
            currentNode = leftLeaf;
        }
        return leftLeaf;
    }
    ThisNodeType* GetNext() const
    {
        if (GetLeftChild())
            return GetLeftLeaf();
        else if (GetRightChild())
        {
            // get left leaf of right child
            ThisNodeType* leftLeaf = GetRightChild()->GetLeftLeaf();
            // no left leaf child, child right will be next
            if (leftLeaf == nullptr)
                return GetRightChild();
        }
        else
        {
            // find parent
            ThisNodeType* parent = GetParent();
            const ThisNodeType* currentNode = this;
            while (parent && currentNode == parent->GetRightChild())
            {
                currentNode = parent;
                if (parent)
                    parent = parent->GetParent();
            }
            return parent;
        }
        return nullptr;
    }
protected:
    RBTreeNodeColor m_color;
    ThisNodeType* m_parent;
    ThisNodeType* m_leftChild, *m_rightChild;
    ValueType m_value;
};

template<typename T>
class RBTree
{
    typedef RBTreeNode<T> TreeNodeType;
    typedef RBTree<T> TreeType;
public:
    typedef T ValueType;
    class Iterator
    {
    public:
        Iterator()
            : m_currentNode(nullptr)
            , m_traverseRightChild(false)
        {
        }
        explicit Iterator(TreeNodeType* currentNode)
            : m_currentNode(currentNode)
            , m_traverseRightChild(false)
        {
        }
        Iterator(TreeNodeType *currentNode, bool traverseRight)
            : m_currentNode(currentNode)
            , m_traverseRightChild(traverseRight)
        {}
        ValueType GetValue() const
        {
            return m_currentNode->GetValue();
        }
        Iterator Next()
        {
            if (m_currentNode == nullptr)
                return Iterator();
            TreeNodeType *nextNode = nullptr;
            bool nextTraverseRight = false;
            if (m_traverseRightChild)
            {
                nextNode = m_currentNode->GetRightChild();
            }
            if (nextNode == nullptr)
            {
                nextNode = m_currentNode->GetParent();
                if (nextNode != nullptr)
                {
                    nextNode = nextNode->GetParent();
                    nextTraverseRight = true;
                }
            }
            else
            {
                TreeNodeType *leftLeaf = nextNode->GetLeftLeaf();
                if (leftLeaf)
                {
                    nextNode = leftLeaf;
                    if (nextNode->GetLeftChild() == nullptr)
                        nextTraverseRight = true;
                }
            }
            return Iterator(nextNode, nextTraverseRight);
        }
        bool IsEnd() const
        {
            return m_currentNode == nullptr;
        }
    private:
        TreeNodeType* m_currentNode;
        bool m_traverseRightChild;
    };
    RBTree()
        : m_root(nullptr)
    {
    }
    ~RBTree()
    {
        ClearRootNode();
    }
    void FixupInsertTree(TreeNodeType *node)
    {
        if (node == nullptr)
            return;
        while (true)
        {
            TreeNodeType *parent = node->GetParent();
            if (parent == nullptr || parent->GetColor() == Black)
                break;
            // parent is red
            TreeNodeType *gf = parent->GetParent();
            if (gf == nullptr)
                return; // it shall not happen
            bool isParentInLeft = parent == gf->GetLeftChild();
            TreeNodeType *uncle = isParentInLeft ? gf->GetRightChild() : gf->GetLeftChild();
            if (uncle && uncle->GetColor() == Red)
            {
                // fine now
                gf->SetColor(Red);
                parent->SetColor(Black);
                uncle->SetColor(Black);
                node = gf;
            }
            else if (uncle == nullptr || uncle->GetColor() == Black)
            {
                bool isThisInLeft = parent->GetLeftChild() == node;
                if (isThisInLeft == isParentInLeft)
                {
                    parent->SetColor(Black);
                    gf->SetColor(Red);
                    if (isParentInLeft)
                        m_root = gf->RotateRight(m_root);
                    else
                        m_root = gf->RotateLeft(m_root);
                    node = gf;
                }
                else
                {
                    if (isParentInLeft)
                        m_root = parent->RotateLeft(m_root);
                    else
                        m_root = parent->RotateRight(m_root);
                    node = parent;
                }
            }
        }
        m_root->SetColor(Black);
    }
    void Insert(ValueType v)
    {
        TreeNodeType* node = new TreeNodeType();
        node->SetValue(v);
        if (m_root == nullptr)
        {
            m_root = node;
            return;
        }
        // not root
        TreeNodeType* insertNode = m_root;
        while (insertNode)
        {
            if (v < insertNode->GetValue())
            {
                if (insertNode->GetLeftChild())
                    insertNode = insertNode->GetLeftChild();
                else
                {
                    insertNode->SetLeftChild(node);
                    node->SetParent(insertNode);
                    break;
                }
            }
            else
            {
                if (insertNode->GetRightChild())
                    insertNode = insertNode->GetRightChild();
                else
                {
                    insertNode->SetRightChild(node);
                    node->SetParent(insertNode);
                    break;
                }
            }
        }
        FixupInsertTree(node);
    }
    bool Check() const
    {
        if (m_root == nullptr)
            return true;// null tree
        if (m_root->GetColor() == Red)
            return false; // root much be black
        // check no consecutive red node
        if (!CheckConsecutiveRed(m_root))
            return false;
        // check black node count
        if (!CheckBlackNodes())
            return false;
        return true;
    }
    Iterator Begin() const
    {
        typename TreeNodeType::ThisNodeType* leftLeaf = m_root ? m_root->GetLeftLeaf() : nullptr;
        if (leftLeaf == nullptr)
            leftLeaf = m_root;
        return Iterator(leftLeaf);
    }
protected:
    bool CheckConsecutiveRed(TreeNodeType *node) const
    {
        if (node == nullptr)
            return true;
        TreeNodeType *leftChild = node->GetLeftChild();
        CheckConsecutiveRed(leftChild);
        TreeNodeType *rightChild = node->GetRightChild();
        CheckConsecutiveRed(rightChild);
        if (node->GetColor() == Red)
        {
            if (leftChild && leftChild->GetColor() == Red)
                return false;
            if (rightChild && rightChild->GetColor() == Red)
                return false;
        }
        return true;
    }
    int GetBlackNodeCountForLeafNode(TreeNodeType *node) const
    {
        int blackNodeCount = 1;
        TreeNodeType *thisNode = node;
        while(thisNode != m_root)
        {
            if (thisNode->GetColor() == Black)
                ++blackNodeCount;
            thisNode = thisNode->GetParent();
        }
        return blackNodeCount;
    }
    bool TraverseChildNode(TreeNodeType *node, int currentBlackNodeCount, int blackNodeCount) const
    {
        if (node->GetColor() == Black)
            ++currentBlackNodeCount;
        if (node->GetLeftChild() == nullptr && node->GetRightChild() == nullptr)
            return currentBlackNodeCount == blackNodeCount;
        if (node->GetLeftChild())
        {
            if (!TraverseChildNode(node->GetLeftChild(), currentBlackNodeCount, blackNodeCount))
                return false;
        }
        if (node->GetRightChild())
        {
            if (!TraverseChildNode(node->GetRightChild(), currentBlackNodeCount, blackNodeCount))
                return false;
        }
        return true;
    }
    bool CheckBlackNodes() const
    {
        if (m_root == nullptr)
            return true;
        TreeNodeType *leftLeafNode = m_root->GetLeftLeaf();
        int blackNodeCount = GetBlackNodeCountForLeafNode(leftLeafNode);
        int currentBlackNodeCount = 0;
        return TraverseChildNode(m_root, currentBlackNodeCount, blackNodeCount);
    }
    void ClearRootNode()
    {
        if (m_root)
        {
            delete m_root;
            m_root = nullptr;
        }
    }
    TreeNodeType* m_root;
};

#endif

