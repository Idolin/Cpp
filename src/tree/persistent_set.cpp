#include "persistent_set.h"

typedef persistent_set::TKey TKey;

persistent_set::tree_node::tree_node(TKey key, tree_node *parent) : key(key), left(0), right(0), direct_links(0)
{
    if(parent)
    {
        if(parent->key > key)
            parent->left = this;
        else
            parent->right = this;
    }
}

persistent_set::tree_node::~tree_node()
{}

void persistent_set::tree_node::deleteNode(bool recursive)
{
    if(!direct_links--)
    {
        if(recursive)
        {
            if(left)
                left->deleteNode(true);
            if(right)
                right->deleteNode(true);
        }
        delete this;
    }
}

persistent_set::tree_node *persistent_set::tree_node::share(tree_node *parent)
{
    tree_node *shared = new tree_node(key, parent);
    this->direct_links--;
    if((shared->right = right))
        right->direct_links++;
    if((shared->left = left))
        left->direct_links++;
    return shared;
}

persistent_set::iterator::iterator(tree_node *root) : is_end(root == 0)
{
    path_from_root.push_back(root);
}

TKey const &persistent_set::iterator::operator*() const
{
    return path_from_root.back()->key;
}

persistent_set::iterator &persistent_set::iterator::operator++()
{
    if(path_from_root.back()->right)
    {
        path_from_root.push_back(path_from_root.back()->right);
        while(path_from_root.back()->left)
            path_from_root.push_back(path_from_root.back()->left);
        return *this;
    }
    for(int i = path_from_root.size() - 2; i >= 0; i--)
        if(path_from_root[i]->left == path_from_root[i + 1])
        {
            for(int k = path_from_root.size() - 1; k > i; k--)
                path_from_root.pop_back();
            return *this;
        }
    is_end = true;
    return *this;
}

persistent_set::iterator persistent_set::iterator::operator++(int)
{
    iterator r = *this;
    ++*this;
    return r;
}

persistent_set::iterator &persistent_set::iterator::operator--()
{
    if(is_end)
    {
        is_end = false;
        return *this;
    }
    if(path_from_root.back()->left)
    {
        path_from_root.push_back(path_from_root.back()->left);
        while(path_from_root.back()->right)
            path_from_root.push_back(path_from_root.back()->right);
        return *this;
    }
    for(int i = path_from_root.size() - 2; i >= 0; i--)
        if(path_from_root[i]->right == path_from_root[i + 1])
        {
            for(int k = path_from_root.size() - 1; k > i; k--)
                path_from_root.pop_back();
            return *this;
        }
    return *this;
}

persistent_set::iterator persistent_set::iterator::operator--(int)
{
    iterator r = *this;
    --*this;
    return r;
}

bool operator==(persistent_set::iterator i, persistent_set::iterator i2)
{
    return ((i.path_from_root.back() == i2.path_from_root.back()) && (i.is_end == i2.is_end));
}

bool operator!=(persistent_set::iterator i, persistent_set::iterator i2)
{
    return !(i == i2);
}

void persistent_set::iterator::share()
{
    bool has_links = false;
    for(unsigned i = 1; i < path_from_root.size(); i++)
    {
        if(!has_links && (path_from_root[i]->direct_links > 0))
            has_links = true;
        if(has_links)
            path_from_root[i] = path_from_root[i]->share(path_from_root[i - 1]);
    }
}

void persistent_set::iterator::followLeft()
{
    path_from_root.push_back(getNode()->left);
}

void persistent_set::iterator::followRight()
{
    path_from_root.push_back(getNode()->right);
}

persistent_set::tree_node *persistent_set::iterator::getNode()
{
    return path_from_root.back();
}

persistent_set::persistent_set() : root(0)
{}

persistent_set::persistent_set(persistent_set const &tree)
{
    if((root = tree.root))
    {
        root->direct_links++;
        //root = root -> share(0);
    }
}

persistent_set &persistent_set::operator=(persistent_set const &rhs)
{
    if(this != &rhs)
    {
        persistent_set c(rhs);
        swap(*this, c);
    }
    return *this;
}

persistent_set::~persistent_set()
{
    if(root)
        root->deleteNode(true);
}

persistent_set::iterator persistent_set::find(TKey key)
{
    iterator it = iterator(this->root);
    while(it.getNode() != 0)
        if(it.getNode()->key > key)
            it.followLeft();
        else if(it.getNode()->key < key)
            it.followRight();
        else
            return it;
    return end();
}

std::pair<persistent_set::iterator, bool> persistent_set::insert(TKey key)
{
    if(!root)
        return make_pair(iterator((root = new tree_node(key, 0))), true);
    iterator it = iterator(root);
    for(;;)
    {
        if(!it.getNode())
        {
            it.path_from_root.pop_back();
            it.share();
            it.path_from_root.push_back(new tree_node(key, it.getNode()));
            return make_pair(it, true);
        }
        if(it.getNode()->key != key)
            (it.getNode()->key > key) ? it.followLeft() : it.followRight();
        else
            return make_pair(it, false);
    }
}

void persistent_set::erase(iterator it)
{
    unsigned node_height = it.path_from_root.size() - 1;
    if(it.getNode()->right)
    {
        it.followRight();
        while(it.getNode()->left)
            it.followLeft();
    }
    it.share();
    if(root == it.path_from_root[node_height])
    {
        if(root == it.getNode())
            root = (it.getNode()->right) ? it.getNode()->right : it.getNode()->left;
        else
            root = it.getNode();
    }
    if(it.path_from_root.size() > node_height + 1)
    {
        if(node_height == it.path_from_root.size() - 2)
        {
            if(it.path_from_root[node_height]->left == it.getNode())
            {
                it.path_from_root[node_height]->left = it.getNode()->left;
                it.getNode()->left = it.path_from_root[node_height];
                swap(it.path_from_root[node_height]->right, it.getNode()->right);
            } else
            {
                it.path_from_root[node_height]->right = it.getNode()->right;
                it.getNode()->right = it.path_from_root[node_height];
                swap(it.path_from_root[node_height]->left, it.getNode()->left);
            }
        } else
        {
            if(it.path_from_root[it.path_from_root.size() - 2]->left == it.getNode())
                it.path_from_root[it.path_from_root.size() - 2]->left = it.path_from_root[node_height];
            else
                it.path_from_root[it.path_from_root.size() - 2]->right = it.path_from_root[node_height];
            swap(it.path_from_root[node_height]->left, it.getNode()->left);
            swap(it.path_from_root[node_height]->right, it.getNode()->right);
        }
        if(node_height)
        {
            if(it.path_from_root[node_height - 1]->left == it.path_from_root[node_height])
                it.path_from_root[node_height - 1]->left = it.getNode();
            else
                it.path_from_root[node_height - 1]->right = it.getNode();
        }
        tree_node *tmp = it.path_from_root[node_height];
        it.path_from_root[node_height] = it.path_from_root[it.path_from_root.size() - 1];
        it.path_from_root[it.path_from_root.size() - 1] = tmp;
    }
    if(it.path_from_root.size() >= 2)
    {
        tree_node *next = (it.getNode()->right) ? it.getNode()->right : it.getNode()->left;
        if(it.path_from_root[it.path_from_root.size() - 2]->right == it.getNode())
            it.path_from_root[it.path_from_root.size() - 2]->right = next;
        else
            it.path_from_root[it.path_from_root.size() - 2]->left = next;
    }
    it.getNode()->deleteNode();
}

persistent_set::iterator persistent_set::begin() const
{
    iterator it = iterator(root);
    if(!root)
        return it;
    while(it.getNode()->left)
        it.followLeft();
    return it;
}

persistent_set::iterator persistent_set::end() const
{
    iterator it = iterator(root);
    if(!root)
        return it;
    while(it.getNode()->right)
        it.followRight();
    return ++it;
}

void swap(persistent_set &a, persistent_set &b)
{
    swap(a.root, b.root);
}
