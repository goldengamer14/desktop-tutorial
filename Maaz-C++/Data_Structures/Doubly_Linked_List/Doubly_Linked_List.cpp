#include <iomanip>
#include <iostream>
using namespace std;

template <class var>
class Linked_List
{
  Linked_List *next = NULL;
  Linked_List *previous = NULL;

public:
  var data;
  int element_num = 0;
  int length = 1;

  Linked_List(var data) : data(data) {}

  void push(var data)
  {

    Linked_List *last_element = this;

    while (last_element->next != NULL)
    {
      last_element->next->next == NULL
          ? last_element->next->previous = last_element
          : 0;
      last_element = last_element->next;
    }

    last_element->next = new Linked_List(data);
    last_element->next->previous = last_element;
    last_element->next->element_num = last_element->element_num + 1;

    length++;
  }

  void unshift(var data)
  {
    Linked_List *new_element = new Linked_List(this->data);

    new_element->next = next;
    new_element->previous = this;

    new_element->element_num = element_num + 1;
    new_element->length = length;

    this->data = data;
    next = new_element;

    Linked_List *first_element = new_element;
    while (first_element->previous != NULL)
    {
      first_element = first_element->previous;
      first_element->length++;
    }

    Linked_List *last_element = new_element;
    while (last_element->next != NULL)
    {
      last_element = last_element->next;
      last_element->element_num++;
    }
  }

  void pop()
  {
    Linked_List *last_element = this;

    while (last_element->next != NULL)
    {
      last_element = last_element->next;
    }

    last_element->previous->next = NULL;

    length--;
    delete last_element;
  }

  void shift()
  {
    Linked_List *del_ele = next;
    Linked_List *element = this;

    swap(data, del_ele->data);
    next = del_ele->next;
    del_ele->next->previous = this;

    length--;
    element_num--;

    while (element->previous != NULL)
    {
      element->previous->length--;
      element = element->previous;
    }

    while (element->next != NULL)
    {
      element->next->element_num--;
      element = element->next;
    }

    del_ele->next = NULL;
    delete del_ele;
  }

  void insert(var data, int index)
  {

    if (index > length || index < 0)
    {
      cout << "****The index " << index << " is out of range!!!****" << endl
           << endl;
      return;
    }
    else if (index == length)
    {
      push(data);
      return;
    }
    else if (index == 0)
    {
      unshift(data);
      return;
    }

    Linked_List *element = get_element(index);
    Linked_List *new_element = new Linked_List(data);

    new_element->next = element;
    new_element->previous = element->previous;
    new_element->previous->next = new_element;
    new_element->element_num = element->element_num;
    new_element->length = element->length + 1;
    element->previous = new_element;

    Linked_List *traverse_element = new_element;

    while (traverse_element->previous != NULL)
    {
      traverse_element->previous->length++;
      traverse_element = traverse_element->previous;
    }

    traverse_element = new_element;

    while (traverse_element->next != NULL)
    {
      traverse_element->next->element_num++;
      traverse_element = traverse_element->next;
    }

    return;
  }

  void remove(int index)
  {
    if (index > length || index < 0)
    {
      cout << "****The index " << index << " is out of range!!!****" << endl
           << endl;
      return;
    }
    else if (index == 0)
    {
      shift();
      return;
    }
    else if (index == length - 1)
    {
      pop();
      return;
    }

    Linked_List *del_ele = get_element(index);

    del_ele->next->previous = del_ele->previous;
    del_ele->previous->next = del_ele->next;

    Linked_List *traverse_element = del_ele;

    while (traverse_element->previous != NULL)
    {
      traverse_element->previous->length--;
      traverse_element = traverse_element->previous;
    }

    traverse_element = del_ele;

    while (traverse_element->next != NULL)
    {
      traverse_element->next->element_num--;
      traverse_element = traverse_element->next;
    }

    del_ele->next = NULL;
    delete del_ele;
  }

  Linked_List *get_element(int index)
  {

    if (index >= length || index < 0)
    {
      cout << "****The index " << index << " is out of range!!!****" << endl
           << endl;
      return NULL;
    }

    Linked_List *current_element = this;

    for (int i = 0; i < index; i++)
    {
      current_element = current_element->next;
    }

    return current_element;
  }

  int index_of(var data)
  {
    Linked_List *current_element = this;
    int i = 0;

    while (current_element != NULL)
    {
      if (current_element->data != data)
        current_element = current_element->next;
      else
      {
        return i;
      }

      i++;
    }

    return -1;
  }

  void display()
  {
    cout << "Data: " << data << endl
         << "Previous: " << (previous == NULL ? 0 : previous->data) << endl
         << "Next: " << (next == NULL ? 0 : next->data) << endl
         << endl;
  }

  ~Linked_List()
  {
    cout << "Destructor for the Object with data: " << setw(4) << data
         << setw(14) << "!!!Called!!!" << endl
         << endl;

    if (next != NULL)
      delete next;

    cout << "Object with data " << setw(4) << data << setw(17)
         << "!!!Destroyed!!!" << endl
         << endl;
  }
};

template <class var>
void display_index_of(Linked_List<var> *head, var data, int index)
{
  cout << "The element with data " << data;
  index > -1 ? cout << " is at index " << index
             : cout << " is not in the list";
  cout << endl;
}

template <class var>
void display_Linked_List(Linked_List<var> *head)
{
  cout << "{";
  for (int i = 0; i < head->length; i++)
  {
    cout << head->get_element(i)->data << (i < head->length - 1 ? ", " : "}");
  }
  cout << endl
       << endl;
}

int main(void)
{
  Linked_List<int> *head = new Linked_List<int>(24);

  head->push(485);
  head->push(92);
  head->push(392);
  head->push(34);
  head->push(982);
  head->push(56);

  cout << "Head element: " << head->data << endl;

  cout << "Second element: " << head->get_element(1)->data << endl;

  cout << "Third element: " << head->get_element(2)->data << endl
       << endl;

  display_Linked_List(head);

  head->pop();
  head->pop();

  display_Linked_List(head);

  head->insert(3489, 0);
  head->insert(3489, head->length);
  head->insert(3489, (head->length - 1) / 2);

  display_Linked_List(head);

  head->unshift(2390);
  head->unshift(4825);

  display_Linked_List(head);

  head->shift();
  head->shift();

  display_Linked_List(head);

  head->remove(0);
  head->remove(head->length - 1);
  head->remove((head->length - 1) / 2);

  display_Linked_List(head);

  display_index_of(head, 24, head->index_of(24));
  display_index_of(head, 34, head->index_of(34));
  display_index_of(head, 432, head->index_of(432));
  cout << endl;

  delete head;
  head = nullptr;

  cout << "Welcome to the character List!!!" << endl
       << endl;

  Linked_List<char> *head_char = new Linked_List<char>('d');

  head_char->push('m');
  head_char->push('l');
  head_char->push('j');
  head_char->push('g');

  display_Linked_List(head_char);

  delete head_char;

  return 0;
};