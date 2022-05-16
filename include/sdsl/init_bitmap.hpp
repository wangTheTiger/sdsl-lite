
/** 
    A class for arrays that can be initialized in constant time (total!)
    Copyright (C) 2022 Fabrizio Barisione
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/
/*! \file init_bitmap.hpp
    \brief init_bitmap.hpp implements an initializable bitmap based on https://users.dcc.uchile.cl/~gnavarro/ps/acmcs14.pdf and init_array.hpp.
    \author Fabrizio Barisione
*/
#ifndef INCLUDED_INIT_BITMAP
#define INCLUDED_INIT_BITMAP

#include <bits/stdc++.h>
#include <cstdint>

using namespace std;

class initializable_bitmap
{
    uint64_t D;          // array size
    uint64_t init_value; // value used to initialize bitmap B[D].


    uint64_t *S;
    uint64_t top;

    uint64_t *U; // auxiliary array

    uint64_t *B;

public:
    initializable_bitmap() = default;

    initializable_bitmap(const uint64_t _D, uint64_t _init_value)
    {
        D = _D / 64;
        top = 0;
        S = new uint64_t[D];
        U = new uint64_t[D];
        B = new uint64_t[D];
        init_value = _init_value;
    };

    void init(const uint64_t _D, uint64_t _init_value)
    {
        D = _D / 64;
        top = 0;
        S = new uint64_t[D];
        U = new uint64_t[D];
        init_value = _init_value;
        B = new uint64_t[D];
    };

    ~initializable_bitmap()
    {
        delete[] S;
        delete[] U;
        delete[] B;
    };


    //! Returns a mem. address (entire word)
    uint64_t operator[](const uint64_t _i) const
    {
        const int i = _i / 64;
        if (i > D)
        {
            cerr << "Caution init_bitmap.hpp: index " << i << " is greater than bitmap size " << D << endl;
        }
        //std::cout << "U[i] = " << U[i] << " top = " << top << " S[U[i]] = " << S[U[i]] << " i = " << i << std::endl;
        if (U[i] < top && S[U[i]] == i)
            return B[i];
        else
            return init_value;
    };

    //! Returns the value of a specific bit in the bitmap.
    uint64_t atPos(const uint64_t _i) const
    {
        const int i = _i / 64;
        if (i > D)
        {
            cerr << "Caution init_bitmap.hpp function atPos: index " << i << " is greater than bitmap size " << D << endl;
        }

        //std::cout << "U[i] = " << U[i] << " top = " << top << " S[U[i]] = " << S[U[i]] << " i = " << i << std::endl;
        if (U[i] < top && S[U[i]] == i)
            //If we are here the B[i] (size uint64_t), therefore we return the value of the specific requested bit.
            //B[i] bits are right shifted (_i % 64) positions.
            return (B[i] >> (_i % 64) & 1ULL);
        else
            return init_value;
    };

    //! Initializes a bitmap cell (whole word) as 'init_value' and returns its mem. address.
    uint64_t &operator[](uint64_t _i)
    {
        const int i = _i / 64;
        if (i > D)
        {
            cerr << "Caution init_bitmap.hpp operator uint64_t& [uint64_t i]: index " << i << " is greater than bitmap size " << D << endl;
        }

        if (U[i] < top && S[U[i]] == i)
        {
            return B[i]; //Returning the memory address of bitmap[i] (uint64_t).
        }
        else
        {
            U[i] = top;
            S[top++] = i;
            B[i] = init_value; // The whole bitmap cell (8 bytes) is initialized as 'init_value'.
            return B[i];       //Returns memory address of the bitmap cell.
        }
    };

    uint64_t size()
    {
        return D;
    };

    uint64_t size_in_bytes()
    {
        return 3 * sizeof(uint64_t) * D /*three arrays of size D*/ + sizeof(uint64_t) * 3 /*three uint64_t variables*/;
    };
};
}

#endif
