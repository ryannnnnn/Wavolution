//
//  EndianIO.cpp
//  Wavolution
//
//  Created by Ryan Larson on 12/26/16.
//  Copyright © 2016 RyanLarson. All rights reserved.
//

#pragma once

#include <fstream>


// to be used with fstream binary mode to read and write data of specified byte length
namespace endianIO
{
    template <typename Word>
    void write_little( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
    {
        for (; size; --size, value >>= 8)
            outs.put( static_cast <char> (value & 0xFF) );
    }
    
    
    template <typename Word>
    void read_little( std::istream& ins, Word& value, unsigned size )
    {
        value = 0;
        for (unsigned i = 0; i < size; ++i)
        {
            value |= ins.get() << (8 * i);
        }
    }
}
