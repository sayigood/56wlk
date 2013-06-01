/**
 * @file bsonformater.h
 * @brief
 *
 **/




#ifndef  _COMPACK_BSONFORMATTER_H_
#define  _COMPACK_BSONFORMATTER_H_

#include <sstream>
#include <stdexcept>
#include <vector>
#include <compack/reader.h>
#include <compack/exception.h>

namespace compack {

template<typename INPUT>
class BsonFormatter {
public:
    explicit BsonFormatter(bool encoding = false ) : _encoding(encoding) {
    }
    template <typename STREAM>
    void format(const INPUT& input, STREAM* stream) {
        _recursive = 0;
        if(stream == 0) {
            throw_mcpack_arg("null pointer");
        }
        Iterator it = Reader<INPUT>::getIterator(input);
        if(it.getType() == protocol::OBJECT) {
            Reader<INPUT> reader(input);
            printObject(reader, *stream);
        } else if(it.getType() == protocol::OBJECTISOARRAY) {
            Reader<INPUT> reader(input);
            printObjectIsoArray(reader, *stream);
        } else if(it.getType() == protocol::ARRAY || it.getType() == protocol::ISOARRAY) {
            Reader<INPUT> reader(input);
            printArray(reader, *stream);
        } else {
            printItem(*stream, it);
        }
    }

    template <typename STREAM>
    void format(Reader<INPUT> reader, STREAM* stream) {
        _recursive = 0;
        if(stream == 0) {
            throw_mcpack_arg("null pointer");
        }
        if(reader.getType() == protocol::OBJECT) {
            printObject(reader, *stream);
        } else if(reader.getType() == protocol::OBJECTISOARRAY) {
            printObjectIsoArray(reader, *stream);
        } else if(reader.getType() == protocol::ARRAY or reader.getType() == protocol::ISOARRAY) {
            printArray(reader, *stream);
        }
    }

    template <typename STREAM>
    void format(const Reader<INPUT> & reader, const Iterator& it, STREAM* stream) {
        _recursive = 0;
        if(stream == 0) {
            throw_mcpack_arg("null pointer");
        }
        printValue(reader, it, *stream);
    }

protected:
    int _recursive;
    template <typename STREAM>
    void printValue(const Reader<INPUT> & reader, const Iterator& it, STREAM& stream) {
        _recursive++;
        if(_recursive > 100) {
            throw_mcpack_data("resursive too deep");
        }
        if(it.isObject()) {
            Reader<INPUT> subreader(reader, it);
            printObject(subreader, stream);
        } else if(it.isObjectIsoArray()) {
            Reader<INPUT> subreader(reader, it);
            printObjectIsoArray(subreader, stream);
        } else if(it.isArray()) {
            Reader<INPUT> subreader(reader, it);
            printArray(subreader, stream);
        } else {
            printItem(stream, it);
        }
        _recursive--;
    }
    std::vector<char> _escape_buf;
    bool _encoding;
    void escape(const char* value) {
        _escape_buf.clear();
        _escape_buf.push_back('\"');
        while(true) {
            switch(*value) {
                case '\0':
                    _escape_buf.push_back('\"');
                    _escape_buf.push_back(0);
                    return;
                case '\n':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('n');
                    break;
                case '\r':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('r');
                    break;
                case '\t':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('t');
                    break;
                case '\b':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('b');
                    break;
                case '\f':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('f');
                    break;
                case '"':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('"');
                    break;
                case '\'':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('\'');
                    break;
                case '\\':
                    _escape_buf.push_back('\\');
                    _escape_buf.push_back('\\');
                    break;
                default:
                    if(_encoding && *value < 0) {
                        _escape_buf.push_back(*value++);
                        _escape_buf.push_back(*value);
                    } else {
                        _escape_buf.push_back(*value);
                    }
            }
            value++;
        }
    }

    template <typename STREAM>
    inline void printItem(STREAM& stream, const Iterator& it) {
        switch(it.getType()) {
            case protocol::INT_8:
                stream << (int)*reinterpret_cast<const char*>(it.value());
                return;
            case protocol::INT_16:
                stream << *reinterpret_cast<const short*>(it.value());
                return;
            case protocol::INT_32:
                stream << *reinterpret_cast<const int*>(it.value());
                return;
            case protocol::INT_64:
                stream << *reinterpret_cast<const long long*>(it.value());
                return;
            case protocol::UINT_8:
                stream << (int)*reinterpret_cast<const unsigned char*>(it.value());
                return;
            case protocol::UINT_16:
                stream << *reinterpret_cast<const short*>(it.value());
                return;
            case protocol::UINT_32:
                stream << *reinterpret_cast<const unsigned int*>(it.value());
                return;
            case protocol::UINT_64:
                stream << *reinterpret_cast<const unsigned long long*>(it.value());
                return;
            case protocol::FLOAT:
                stream << *reinterpret_cast<const float*>(it.value());
                return;
            case protocol::DOUBLE:
                stream << *reinterpret_cast<const double*>(it.value());
                return;
            case protocol::BOOL:
                if(*reinterpret_cast<const bool*>(it.value())) {
                    stream << "true";
                } else {
                    stream << "false";
                }
                return;
            case protocol::MCPACK_NULL:
                stream << "null";
                return ;

            case protocol::BINARY:
            case protocol::BINARY | protocol::SHORT_ITEM: {
                stream << "unhex(\"";
                const unsigned char*   b = (const unsigned char*)it.getAsBinary();
                for(unsigned int i = 0; i < it.length(); i++) {
                    char c = b[i] >> 4;
                    if(c < 10) {
                        stream << (char)(c + '0');
                    } else {
                        stream << (char)(c + 'A' - 10);
                    }
                    c = b[i] & 0xf;
                    if(c < 10) {
                        stream << (char)(c + '0');
                    } else {
                        stream << (char)(c + 'A' - 10);
                    }
                }
                stream << "\")";
            }
            return;
            case protocol::STRING:
            case protocol::STRING | protocol::SHORT_ITEM:
                escape(reinterpret_cast<const char*>(it.getAsString()));
                stream << &_escape_buf[0];
                return;
            default:
                throw_mcpack_data("unknown type");

        }
    }

    template <typename STREAM>
    void printObjectIsoArray(Reader<INPUT> & reader, STREAM& stream) {
        stream << '[';
        ObjectIterator it;
        unsigned int field = reader.count();
        unsigned int size;
        if(field > 128) {
            throw_mcpack_data("too many isoarray field");
        } else if(field == 0) {
            stream << ']';
            return;
        }
        Reader<INPUT> readers[field];
        const char* fieldnames[field];
        for(unsigned int i = 0; i<field; i++) {
            if(!reader.next(it)) {
                throw_mcpack_data("too many isoarray field");
            }
            new (&readers[i]) Reader<INPUT>(reader, it);
            fieldnames[i] = it.getName();
        }
        size  = readers[0].count();
        for(unsigned int i = 0; i<size; i++) {
            stream << "{";
            for(unsigned int f = 0; f < field; f++) {
                escape(fieldnames[f]);
                stream << &_escape_buf[0] << ':';
                if(!readers[f].next(it)) {
                    throw_mcpack_data("too many isoarray field");
                }
                if(it.isObject()) {
                    Reader<INPUT> subreader(reader, it);
                    printObject(subreader, stream);
                } else if(it.isArray()) {
                    Reader<INPUT> subreader(reader, it);
                    printArray(subreader, stream);
                } else {
                    printItem(stream, it);
                }
                if(f != field - 1) {
                    stream << ',';
                }
            }
            stream << '}';
            if(i != size - 1) {
                stream << ',';
            }
        }
        stream << ']';

    }

    template <typename STREAM>
    void printObject(Reader<INPUT> & reader, STREAM& stream) {
        stream << '{';
        ObjectIterator it;
        if(reader.next(it)) {
            escape(it.getName());
            stream << &_escape_buf[0] << ':';
            printValue(reader, it, stream);
        }
        while(reader.next(it)) {
            escape(it.getName());
            stream << ',' << &_escape_buf[0] << ':';
            printValue(reader, it, stream);
        }
        stream << '}';
    }

    template <typename STREAM>
    void printArray(Reader<INPUT> & reader, STREAM& stream) {
        stream << '[';
        Iterator it;
        if(reader.next(it)) {
            printValue(reader, it, stream);
        }
        while(reader.next(it)) {
            stream << ',';
            printValue(reader, it, stream);
        }
        stream << ']';
    }
};


}

#endif  //__JSONFORMATER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
