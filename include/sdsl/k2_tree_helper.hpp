/* sdsl - succinct data structures library
    Copyright (C) 2016 Francisco Montoto

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/ .
*/
/*! \file k2_tree_helper.hpp
    \brief k2_tree_helper.hpp contains helper functions and definitions for a k^2-tree implementation.
    \author Francisco Montoto
*/
#ifndef INCLUDED_SDSL_K2_TREE_HELPER
#define INCLUDED_SDSL_K2_TREE_HELPER

#include <cmath>
#include <iostream>

#include "sdsl/bit_vectors.hpp"

//! Namespace for the succinct data structure library.
namespace sdsl
{

//! Namespace for the k2_tree
namespace k2_tree_ns
{

typedef int_vector<>::size_type idx_type;
typedef int_vector<>::size_type size_type;

template<typename t_bv=bit_vector>
int _build_from_matrix(const std::vector<std::vector <int>>& matrix,
                       const uint8_t k, int n, const int height,
                       int l, int p, int q, std::vector<std::deque<t_bv>>& acc)
{
    unsigned i, j, b_size = pow(k, 2);
    t_bv b(b_size, 0);
    bool is_leaf = (l == height);

    if(is_leaf) {
        for(i = 0; i < k; i++)
            for(j = 0; j < k; j++)
                if(p + i < matrix.size()
                   && q + j < matrix.size()
                   && matrix[p + i][q + j] == 1)
                    b[i * k + j] = 1;
    }
    else { // Internal node
        for(i = 0; i < k; i++)
            for(j = 0; j < k; j++)
                b[i * k + j] = _build_from_matrix(matrix, k, n/k, height, l + 1,
                                                  p + i * (n/k), q + j * (n/k),
                                                  acc);
    }

    // TODO There must be a better way to check if there is a 1 at b.
    for(i = 0; i < b_size; i++)
        if(b[i] == 1)
            break;
    if(i == b_size) // If there are not 1s at b.
        return 0;

    acc[l].push_back(std::move(b));
    return 1;
}

inline uint16_t get_chunk_idx(idx_type v, idx_type u, idx_type c_0,
							  idx_type r_0, size_type l, uint8_t k)
{
    return std::floor(static_cast<double>(v - r_0) / l) * k +
           std::floor(static_cast<double>(u - c_0) / l);
}
} // end namespace k2_tree_ns
} // end namespace sdsl

#endif
