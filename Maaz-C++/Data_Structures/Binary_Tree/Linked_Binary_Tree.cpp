#include <iostream>
#include <cstring>
using namespace std;

template <class type>
class Binary_Tree_Node
{
    type data;
    Binary_Tree_Node<type> *left;
    Binary_Tree_Node<type> *right;

public:
    Binary_Tree_Node()
    {
        memset(this, 0, sizeof(Binary_Tree_Node<type>));
    }

    Binary_Tree_Node(type data)
    {
        this->data = data;
        left = right = (Binary_Tree_Node<type> *)NULL;
    }

    Binary_Tree_Node<type> &Get_Left() { return (*left); }

    Binary_Tree_Node<type> &Get_Right() { return (*right); }

    Binary_Tree_Node<type> &Set_Left(type data)
    {
        if (left)
            left->data = data;
        else
            left = new Binary_Tree_Node<type>(data);

        return (*left);
    }

    Binary_Tree_Node<type> &Set_Right(type data)
    {
        if (right)
            right->data = data;
        else
            right = new Binary_Tree_Node<type>(data);

        return (*right);
    }

    void Preorder()
    {
        cout << data << " ";

        if (left)
            left->Preorder();

        if (right)
            right->Preorder();
    }

    void Postorder()
    {
        if (left)
            left->Postorder();

        if (right)
            right->Postorder();

        cout << data << " ";
    }

    void Inorder()
    {
        if (left)
            left->Inorder();

        cout << data << " ";

        if (right)
            right->Inorder();
    }

    ~Binary_Tree_Node()
    {
        if (left)
            delete left;

        if (right)
            delete right;
    }
};

int main()
{
    cout << "\nHello, World!\n"
         << endl;

    Binary_Tree_Node<int> self(4);

    self.Set_Left(1);
    self.Set_Right(6);

    self.Get_Left().Set_Left(5);
    self.Get_Right().Set_Right(2);

    self.Preorder();
    cout << endl;

    self.Postorder();
    cout << endl;

    self.Inorder();
    cout << endl;

    cout << endl;
    return 0;
}