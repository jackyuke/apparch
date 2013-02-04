#ifndef _RB_TREE_H_
#define _RB_TREE_H_

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
        , m_parent(NULL)
        , m_leftChild(NULL)
        , m_rightChild(NULL)
    {
    }
    ~RBTreeNode()
    {
        m_parent = NULL;
        if (m_leftChild)
        {
            delete m_leftChild;
            m_leftChild = NULL;
        }
        if (m_rightChild)
        {
            delete m_rightChild;
            m_rightChild = NULL;
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
    ThisNodeType* GetLeftLeaf() const
    {
        ThisNodeType* leftLeaf = NULL;
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
            if (leftLeaf == NULL)
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
        return NULL;
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
            : m_currentNode(NULL)
            , m_traverseRightChild(false)
        {
        }
        explicit Iterator(TreeNodeType* currentNode)
            : m_currentNode(currentNode)
            , m_traverseRightChild(false)
        {
        }
        ValueType GetValue() const
        {
            return m_currentNode->GetValue();
        }
        void Next()
        {
            m_currentNode = m_currentNode->GetNext();
        }
        bool IsEnd() const
        {
            return m_currentNode == NULL;
        }
    private:
        TreeNodeType* m_currentNode;
        bool m_traverseRightChild;
    };
    RBTree()
        : m_root(NULL)
    {
    }
    ~RBTree()
    {
        ClearRootNode();
    }
    void Insert(ValueType v)
    {
        TreeNodeType* node = new TreeNodeType();
        node->SetValue(v);
        if (m_root == NULL)
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
    }
    Iterator Begin() const
    {
        typename TreeNodeType::ThisNodeType* leftLeaf = m_root ? m_root->GetLeftLeaf() : NULL;
        if (leftLeaf == NULL)
            leftLeaf = m_root;
        return Iterator(leftLeaf);
    }
protected:
    void ClearRootNode()
    {
        if (m_root)
        {
            delete m_root;
            m_root = NULL;
        }
    }
    TreeNodeType* m_root;
};

#endif

