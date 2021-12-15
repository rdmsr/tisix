#pragma once
#include <tisix/alloc.hpp>

namespace tisix
{

template <typename T>
class Node
{
public:
    T data;
    size_t index = 0;
    Node<T> *next, *prev;
};

template <typename T>
class List
{
public:
    Node<T> *head = nullptr;
    size_t size = 0;

public:
    T operator[](size_t index)
    {
        Node<T> *tmp = this->head;

        while (tmp->index != index)
        {
            tmp = tmp->next;
        }

        if (index == tmp->index)
        {
            return tmp->data;
        }

        return 0;
    }

    T *begin()
    {
        return &((Node<T> *)(head - (size * sizeof(T))))->data;
    }

    T *end()
    {
        return &head->data;
    }

    void push(T value)
    {
        Node<T> *temp = this->head;
        Node<T> *new_n = new_node(value);

        if (!this->head)
        {
            this->head = new_n;
            return;
        }

        while (temp->next)
            temp = temp->next; /* Go to last node */

        temp->next = new_n;
        new_n->prev = temp;

        this->size++;
    }

    void construct(){};

private:
    Node<T> *new_node(T value)
    {
        Node<T> *tmp = (Node<T> *)tisix::malloc(sizeof(Node<T>));
        tmp->index = size++;
        tmp->data = value;
        tmp->next = nullptr;
        tmp->prev = nullptr;

        return tmp;
    }
};
} // namespace tisix