
/** 
    A class for arrays that can be initialized in constant time (total!)
    Copyright (C) 2021 Diego Arroyuelo

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


#ifndef INIT_ARRAY
#define INIT_ARRAY

#include <bits/stdc++.h> 
#include <cstdint>

using namespace std;


namespace sdsl {
template<typename array_t>
class initializable_array_classic
{
    uint64_t D; // array size
    array_t init_value;  // value used to initialize the array
    
    array_t* V;  // array
    
    uint64_t* S;
    uint64_t top;    
    
    uint64_t* U; // auxiliary array

public:
    
    initializable_array_classic() = default;
    
    initializable_array_classic(const uint64_t _D, array_t _init_value)
    {
        D = _D;
        top = 0;
        V = new array_t[D];
        S = new uint64_t[D];
        U = new uint64_t[D];
        init_value = _init_value;
    };
   
    void init(const uint64_t _D, array_t _init_value)
    {
        D = _D;
        top = 0;
        V = new array_t[D];
        S = new uint64_t[D];
        U = new uint64_t[D];
        init_value = _init_value;
    };

 
    ~initializable_array_classic()
    {
        delete [] V;
        delete [] S;
        delete [] U;
    };


    array_t operator[](const uint64_t i) const
    {
        if (i >= D) {
            cerr << "Caution init_array.hpp: index " << i << " is greater than array size " << D << endl;
        }  
      //std::cout << "U[i] = " << U[i] << " top = " << top << " S[U[i]] = " << S[U[i]] << " i = " << i << std::endl;
        if (U[i] < top && S[U[i]] == i)
            return V[i];
        else
            return init_value;   
    };

    array_t atPos(const uint64_t i) const
    {
         if (i >= D) {
            cerr << "Caution init_array.hpp function atPos: index " << i << " is greater than array size " << D << endl;
        }

        //std::cout << "U[i] = " << U[i] << " top = " << top << " S[U[i]] = " << S[U[i]] << " i = " << i << std::endl;
        if (U[i] < top && S[U[i]] == i)
            return V[i];
        else
            return init_value;
    };

    array_t& operator[](uint64_t i)   
    {
        if (i >= D) {
            cerr << "Caution init_array.hpp operator array_t& [uint64_t i]: index " << i << " is greater than array size " << D << endl;
        }

        //std::cout << "Oh! " << i << std::endl; 
        if (U[i] < top && S[U[i]] == i)
            return V[i];
        else {
            U[i] = top;
            S[top++] = i;
            V[i] = init_value;   // OJO, revisar esto
            return V[i];
        }
    };

    

    uint64_t size()
    {
        return D;
    };

    uint64_t size_in_bytes()
    {
        return sizeof(array_t)*D + sizeof(array_t) + 3*sizeof(uint64_t)*D + sizeof(uint64_t) + 3*sizeof(uint64_t*);  
    };

};


template<typename array_t>
class initializable_array
{
    uint64_t n; // array size
    array_t init_value;  // value used to initialize the array

    array_t* A;  // array

    initializable_array_classic<uint64_t> W;

public:

    initializable_array() = default;

    initializable_array(const uint64_t _n, array_t _init_value)
    {
        n = _n;
        A = new array_t[n];
        init_value = _init_value;
        W.init((n+63)/64, 0); // = initializable_array_classic<uint64_t>((n+63)/64 + 1, 0);
    };

    ~initializable_array()
    {
        delete [] A;
    };

   array_t operator[](const uint64_t i) const
    {
        //if (i >= D) {
        //    cerr << "Caution init_array.hpp: index " << i << " is greater than array size " << D << endl;
        //}
        //std::cout << "U[i] = " << U[i] << " top = " << top << " S[U[i]] = " << S[U[i]] << " i = " << i << std::endl;
        //cout << "[ ]" << i << " " << n <<std::endl;
        //register uint64_t temp = W.atPos(i/64); 
        /*if (!temp)
            return init_value;
        else {*/
            if ((W.atPos(i/64)>>(i%64/*&0x3F*/)) & 1ull) {
	        return A[i];
            }
            else 
	        return init_value;
        /*}*/
    };

    array_t atPos(const uint64_t i) const
    {
        //if (i >= D) {
        //    cerr << "Caution init_array.hpp function atPos: index " << i << " is greater than array size " << D << endl;
        //}

        //std::cout << "U[i] = " << U[i] << " top = " << top << " S[U[i]] = " << S[U[i]] << " i = " << i << std::endl;
        //cout << "[ ]" << i << " " << n <<std::endl;
        //register uint64_t temp = W.atPos(i/64);
        /*if (!temp)
            return init_value;
        else {*/
            if ((W.atPos(i/64)>>(i%64/*&0x3F*/)) & 1ull) {
                return A[i];
            }
            else 
                return init_value;
        //}
    };

    array_t& operator[](uint64_t i)
    {
        //if (i >= D) {
        //    cerr << "Caution init_array.hpp operator array_t& [uint64_t i]: index " << i << " is greater than array size " << D << endl;
        //}

        //std::cout << "[ ]" << i << " " << n <<std::endl; 
        register uint64_t temp = W.atPos(i/64);
        if ((/*W.atPos(i/64)*/temp>>(i%64/*&0x3F*/)) & 1ull) {
            return A[i];
        }
        else {
            W[i/64] = /*W.atPos(i/64)*/ temp | (1ull<<(i%64/*&0x3F*/));       
            A[i] = init_value;
            return A[i];
        }
    };

    // number of elements in the array
    uint64_t size()
    {
        return n;
    };

    uint64_t size_in_bytes() 
    {
        return sizeof(array_t)*n + W.size_in_bytes() + sizeof(array_t) + sizeof(array_t *) + sizeof(uint64_t);
    };

};

}

#endif
