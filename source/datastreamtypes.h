/*

The MIT License (MIT)

Copyright (c) 2014 Thomas Boy, HTWK Leipzig, university of applied sciences,
European Social Fund (ESF) prototed the project: TOPINUS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/



#ifndef DATASTREAM_H
#define DATASTREAM_H



//! a class for the ENUM TYPE
//! just a header file
class DataStreamTypes
{
public:

    //! the types of the data stream
    //! when her something is added, it should be also in Data::TYPE Enum !!
    enum TYPE
    {
        //! everything which is order of picutre ist a VIDEO DataStream
        VIDEO,
        //! everything which is a modulatet signal in the frequenz of audible speach is an AUDIO DataStream
        AUDIO,
        //! everything which deliver cyclically measuredata is a SENSOR DataStream
        SENSOR,
        //! never to be used, unless it's a base class
        UNDEFINED
    };


};

#endif // DATASTREAM_H
