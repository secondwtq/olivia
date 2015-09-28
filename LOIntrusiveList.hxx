//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOINTRUSIVELIST_HXX
#define VMFOROLIVE_LOINTRUSIVELIST_HXX

#include <stddef.h>
#include <assert.h>

namespace Olivia {
namespace Base {

template <typename T, typename pT, typename constPT>
class IntrusiveListNode {
private:

    typedef pT PointerT;
    typedef constPT ConstPointerT;

    // do we need to avoid cyclic-reference?
    T *node_prev = nullptr;
    PointerT node_next = nullptr;

    template <typename T_>
    friend class IntrusiveList;
    template <typename T_>
    friend class ilist_iterator_base;
};

// TODO: more serious iterator
//  now it is only has support for 'for each' loops.
template <typename IteratorPtrT>
class ilist_iterator_base {
public:
    ilist_iterator_base(IteratorPtrT t) : target(t) { }
    ilist_iterator_base& operator++() {
        target = target->node_next;
        return *this;
    }
    IteratorPtrT operator*() {
        return target; }
private:
    IteratorPtrT target;

    template <typename T>
    friend class IntrusiveList;
    template <typename T1, typename T2>
    friend bool operator!=(const ilist_iterator_base<T1>& lhs,
            const ilist_iterator_base<T2>& rhs);
};

template <typename T1, typename T2>
inline bool operator!=(const ilist_iterator_base<T1>& lhs,
        const ilist_iterator_base<T2>& rhs) {
    return lhs.target != rhs.target; }

template <typename T>
class IntrusiveList {
public:

    typedef typename T::PointerT PointerT;
    typedef typename T::ConstPointerT ConstPointerT;

    typedef ilist_iterator_base<PointerT> iterator;
    typedef ilist_iterator_base<ConstPointerT> const_iterator;

    iterator begin() {
        return iterator(head()); }
    iterator end() {
        return iterator(nullptr); }
    const_iterator begin() const {
        return const_iterator(head()); }
    const_iterator end() const {
        return const_iterator(nullptr); }

    PointerT head() {
        return m_head; }
    const PointerT head() const {
        return m_head; }
    PointerT tail() {
        return m_tail; }
    const PointerT tail() const {
        return m_tail; }

    static PointerT node_next(PointerT ptr) {
        return ptr->node_next; }

    void erase(PointerT ptr) {
        assert(ptr);
        assert(!empty());

        if (ptr->node_prev) {
            ptr->node_prev->node_next = node_next(ptr); }
        if (node_next(ptr)) {
            ptr->node_next->node_prev = ptr->node_prev; }
        if (ptr == head()) {
            m_head = ptr->node_next; }
        if (ptr == tail()) {
            PointerT prev = get_previous(tail());
            assert(node_next(prev) == tail());
            m_tail = prev;
        }
        ptr->node_prev = nullptr, ptr->node_next = nullptr;
        m_size--;
    }

    void pop_front() {
        assert(head());
        erase(head());
    }

    void pop_back() {
        assert(tail());
        erase(tail());
    }

    // insert *ptr as the *next* of pos
    void insert(PointerT pos, PointerT ptr) {
        assert(pos && ptr);
        assert(!node_in_list(ptr) && node_in_list(pos));
        pos->node_next = ptr;
        ptr->node_prev = recover_ptr(pos);
        ptr->node_next = node_next(pos);

        if (pos == tail()) {
            m_tail = ptr; }
        m_size++;
    }

    void push_back(PointerT ptr) {
        assert(!node_in_list(ptr));

        if (!empty()) {
            tail()->node_next = ptr;
            ptr->node_prev = recover_ptr(tail());
        } else { m_head = ptr; }

        m_tail = ptr;
        m_size++;
    }

    void push_front(PointerT ptr) {
        assert(!node_in_list(ptr));

        if (!empty()) {
            head()->node_prev = recover_ptr(ptr);
            ptr->node_next = head();
            ptr->node_prev = nullptr;
            m_head = ptr;
            m_size++;
        } else { push_back(ptr); }
    }

    void clear() {
        m_head = m_tail = nullptr;
        m_size = 0; }
    bool empty() const {
        return head() == nullptr; }
    size_t size() const {
        return m_size; }

private:

    static bool node_in_list(PointerT ptr) {
        return node_next(ptr) || ptr->node_prev; }

    PointerT get_previous(PointerT ptr) {
        if (!ptr) {
            return tail(); }
        PointerT current = head();
        while (true) {
            PointerT t = node_next(current);
            if (!t) {
                break; }
            if (t == ptr) {
                return current; }
        }
        return nullptr;
    }

    T *recover_ptr(PointerT ptr) {
        return (&(*ptr)); }

    size_t m_size = 0;
    PointerT m_head = nullptr;
    PointerT m_tail = nullptr;
};

}
}

#endif // VMFOROLIVE_LOINTRUSIVELIST_HXX
