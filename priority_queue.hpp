#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {
    /**
     * a container like std::priority_queue which is a heap internal.
     */
    template<typename T, class Compare = std::less<T> >
    class priority_queue {
    private:
        bool re_occur;
        struct Node {
            T * data;
            Node *leftTree;
            Node *rightTree;
            int dis= 0;
            Node() {
                data = nullptr;
                leftTree = rightTree = nullptr;
                dis = 0;
            }
            explicit Node(T _data) {
                data = new T(_data);
                leftTree = rightTree = nullptr;
                dis = 0;
            }
        };
        Node * merge(Node * a, Node * b) {
            if (!a) return b;
            if (!b)return a;
            try {
                if (Compare()(*(a->data), *(b->data))) {
                    std::swap(a, b);
                }
            }catch (exception &e) {
                re_occur = true;
                return a;
            }

            a -> rightTree = merge(a -> rightTree, b);
            if (re_occur) {
                return a;
            }
            if (! a -> leftTree|| a-> leftTree -> dis < a -> rightTree ->dis) {
                std::swap(a -> leftTree, a -> rightTree);
            }
            a -> dis = (a -> rightTree ? a -> rightTree -> dis + 1 : 0);
            return a;
        }
        Node * helpConstruct(Node * p) {
            if (!p)return nullptr;
            Node * tmp = new Node(*(p->data));
            tmp -> leftTree= helpConstruct(p -> leftTree);
            tmp -> rightTree= helpConstruct(p -> rightTree);
            tmp -> dis = p->dis ;
            return tmp;
        }
        void helpDeconstruct(Node * p) {
            if (!p) return;
            helpDeconstruct(p -> leftTree);
            helpDeconstruct(p -> rightTree);
            delete p -> data;
            delete p;
        }
        Node * root;
        size_t _size;

    public:
        /**
         * TODO constructors
         */
        priority_queue() {
            re_occur = false;
            root = nullptr;
            _size = 0;
        }

        priority_queue(const priority_queue &other) {
            re_occur = false;
            root = helpConstruct(other.root);
            this -> _size = other._size;
        }

        /**
         * TODO deconstructor
         */
        ~priority_queue() {
            helpDeconstruct(root);
            root = nullptr;
        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other) {
            re_occur = false;
            if (this ->root == other.root) {
                return *this;
            }
            helpDeconstruct(root);
            root = helpConstruct(other.root);
            this -> _size = other._size;
            return *this;
        }

        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const {
            if (!root)throw sjtu::container_is_empty();
            return *(root -> data);
        }

        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e) {
            Node * tmp = new Node(e);
            root = merge(root, tmp);
            if (re_occur) {
                re_occur = false;
                delete tmp -> data;
                delete tmp;
                throw sjtu::runtime_error();
            }else {
                _size++;
            }
        }

        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop() {
            if (!root)throw sjtu::container_is_empty();
            Node * tmp = root;
            root = merge(root -> leftTree, root -> rightTree);
            if (re_occur) {
                root = tmp;
                re_occur = false;
                throw sjtu::runtime_error();
            }
            else {
                _size--;
                delete tmp -> data;
                delete tmp;
            }
        }

        /**
         * return the number of the elements.
         */
        size_t size() const {
            return _size;
        }

        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const {
            return root == nullptr;
        }

        /**
         * merge two priority_queues with at most O(logn) complexity.
         * clear the other priority_queue.
         */
        void merge(priority_queue &other) {
            this -> root = merge(this -> root, other.root);
            if (re_occur) {
                re_occur = false;
                throw sjtu::runtime_error();
            }
            else {
                other.root = nullptr;
            }
        }
    };
}

#endif
