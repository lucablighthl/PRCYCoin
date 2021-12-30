<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once
/*
    Template used for reverse iteration in C++11 range-based for loops.
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto x : reverse_iterate(v))
        std::cout << x << " ";
=======
// Copyright (c) 2017-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_REVERSE_ITERATE_H
#define BITCOIN_REVERSE_ITERATE_H

/**
 * Template used for reverse iteration in C++11 range-based for loops.
 *
 *   std::vector<int> v = {1, 2, 3, 4, 5};
 *   for (auto x : reverse_iterate(v))
 *       std::cout << x << " ";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 */

template <typename T>
class reverse_range
{
<<<<<<< HEAD
    T &x;
    
public:
    reverse_range(T &x) : x(x) {}
    
    auto begin() const -> decltype(this->x.rbegin())
    {
        return x.rbegin();
    }
    
    auto end() const -> decltype(this->x.rend())
    {
        return x.rend();
    }
};
 
=======
    T &m_x;

public:
    reverse_range(T &x) : m_x(x) {}

    auto begin() const -> decltype(this->m_x.rbegin())
    {
        return m_x.rbegin();
    }

    auto end() const -> decltype(this->m_x.rend())
    {
        return m_x.rend();
    }
};

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
template <typename T>
reverse_range<T> reverse_iterate(T &x)
{
    return reverse_range<T>(x);
}

<<<<<<< HEAD
=======
#endif // BITCOIN_REVERSE_ITERATE_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
