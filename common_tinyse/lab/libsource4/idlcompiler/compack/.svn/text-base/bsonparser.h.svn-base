
/**
 * @file bsonparser.h
 * @brief
 *
 **/




#ifndef  _COMPACK_BSONPARSER_H_
#define  _COMPACK_BSONPARSER_H_

#include <vector>
#include <compack/writer.h>
namespace compack {

template<typename BUFFER>
class BsonParser {
public:
    explicit BsonParser(bool encoding = false) : _unescape_key_buf(256), _unescape_value_buf(256), _encoding(encoding) {
    }
    void parse(const char* input, ArrayWriter<BUFFER> * output) {
        _recursive = 0;
        if(input == 0 || output == 0) {
            throw_mcpack_arg("null pointer");
        }
        skip_white(&input);
        switch(*input) {
            case '\0':
                throw_json_data("unexpected end");
            case '{': {
                input++;
                ObjectWriter<BUFFER> object(output);
                input = parseobject(input, object);
            }
            break;
            case '[': {
                input++;
                ArrayWriter<BUFFER> array(output);
                input = parsearray(input, array);
            }
            case '"': {
                input++;
                input = unescape(input, _unescape_value_buf);
                output->putString(&_unescape_value_buf[0],
                                  _unescape_value_buf.size());
            }
            break;
            case 't':
                if(strncmp(input, "true", 4) == 0) {
                    output->template putNum<bool>(true);
                    break;
                } else {
                    throw_json_data("unexpected char");
                }
            case 'f':
                if(strncmp(input, "false", 5) == 0) {
                    output->template putNum<bool>(false);
                    break;
                } else {
                    throw_json_data("unexpected char");
                }
                break;
            case 'n':
                if(strncmp(input, "null", 4) == 0) {
                    output->putNull();
                    break;
                } else {
                    throw_json_data("unexpected char");
                }
                break;
            default:
                input = output->putNum(input);
        }
        expect(&input, 0);
    }
    void parse(const char* input, ObjectWriter<BUFFER> * output, const char* name) {
        _recursive = 0;
        if(input == 0 || output == 0) {
            throw_mcpack_arg("null pointer");
        }
        skip_white(&input);
        switch(*input) {
            case '\0':
                throw_json_data("unexpected end");
            case '{': {
                input++;
                ObjectWriter<BUFFER> object(output, name);
                input = parseobject(input, object);
            }
            break;
            case '[': {
                input++;
                ArrayWriter<BUFFER> array(output, name);
                input = parsearray(input, array);
            }
            break;
            case '"': {
                input++;
                input = unescape(input, _unescape_value_buf);
                output->putString(name, strlen(name + 1),
                                  &_unescape_value_buf[0],
                                  _unescape_value_buf.size());
            }
            break;
            case 't':
                if(strncmp(input, "true", 4) == 0) {
                    output->template putNum<bool>(name, true);
                    break;
                } else {
                    throw_json_data("unexpected char");
                }
            case 'f':
                if(strncmp(input, "false", 5) == 0) {
                    output->template putNum<bool>(name, false);
                    break;
                } else {
                    throw_json_data("unexpected char");
                }
            case 'n':
                if(strncmp(input, "null", 4) == 0) {
                    output->putNull(name);
                    break;
                } else {
                    throw_json_data("unexpected char");
                }
            default:
                input = output->putNum(name, input);
        }
        expect(&input, 0);
    }
    unsigned int parse(const char* input, BUFFER* output) {
        _recursive = 0;
        if(input == 0 || output == 0) {
            throw_mcpack_arg("null pointer");
        }
        skip_white(&input);
        switch(*input) {
            case '\0':
                throw_json_data("unexpected end");
            case '{': {
                input++;
                ObjectWriter<BUFFER> object(output);
                input = parseobject(input, object);
                object.close();
                expect(&input, 0);
                return object.size();
            }
            case '[': {
                input++;
                ArrayWriter<BUFFER> array(output);
                input = parsearray(input, array);
                array.close();
                expect(&input, 0);
                return array.size();
            }
            case '"': {
                input++;
                input = unescape(input, _unescape_value_buf);
                expect(&input, 0);
                return Writer<BUFFER>::putString(output,
                                                 &_unescape_value_buf[0], _unescape_value_buf.size());
            }
            case 't':
                if(strncmp(input, "true", 4) == 0) {
                    expect(&input, 0);
                    return Writer<BUFFER>::template putNum<bool>(output, true);
                } else {
                    throw_json_data("unexpected char");
                }
            case 'f':
                if(strncmp(input, "false", 5) == 0) {
                    expect(&input, 0);
                    return Writer<BUFFER>::template putNum<bool>(output, false);
                } else {
                    throw_json_data("unexpected char");
                }
            case 'n':
                if(strncmp(input, "null", 4) == 0) {
                    expect(&input, 0);
                    return Writer<BUFFER>::putNull(output);
                } else {
                    throw_json_data("unexpected char");
                }
            default:
                unsigned int s;
                s = Writer<BUFFER>::putNum(output, &input);
                expect(&input, 0);
                return s;
        }
    }
private:
    std::vector<char> _unescape_key_buf;
    std::vector<char> _unescape_value_buf;
    bool _encoding;
    int _recursive;
protected:
    const char* unescape(const char* value, std::vector<char> & buf) {
        buf.clear();
        while(true) {
            switch(*value) {
                case '\\': {
                    switch(*++value) {
                        case '\\':
                            buf.push_back('\\');
                            break;
                        case '"':
                            buf.push_back('"');
                            break;
                        case '\'':
                            buf.push_back('\'');
                            break;
                        case 'n':
                            buf.push_back('\n');
                            break;
                        case 'r':
                            buf.push_back('\r');
                            break;
                        case 't':
                            buf.push_back('\t');
                            break;
                        case 'b':
                            buf.push_back('\b');
                            break;
                        case '/':
                            buf.push_back('/');
                            break;
                        case 'f':
                            buf.push_back('\f');
                            break;
                        case 'u':
                            buf.push_back('\\');
                            buf.push_back('u');
                            for(int i=1; i<5; i++) {
                                char C = value[i];
                                if(((C >= '0') && (C <= '9'))
                                        || ((C >='A') && (C <= 'F'))
                                        || ((C >= 'a') && (C <= 'f'))) {
                                    buf.push_back(C);
                                } else {
                                    throw_json_data("unexpected char");
                                }
                            }
                            break;
                        default:
                            throw_json_data("unexpected char");

                    }
                    value++;
                    break;
                }
                case '"':
                    buf.push_back(0);
                    return ++value;
                case '\'':
                    buf.push_back(0);
                    return ++value;
                default:
                    if(*value < 32 && *value >=0 ) {
                        throw_json_data("unexpected char");
                    }
                    if(_encoding) {
                        if(*value < 0) {
                            buf.push_back(*value++);
                            buf.push_back(*value++);
                        } else {
                            buf.push_back(*value++);
                        }
                    } else {
                        buf.push_back(*value++);
                    }
            }
        }
    }
    const char* parseobject(const char* value, ObjectWriter<BUFFER> & object) {
        _recursive++;
        if(_recursive > 100) {
            throw_json_data("recursize too deep");
        }
        skip_white(&value);
        if(*value == '}') {
            _recursive--;
            return ++value;
        }
        while(true) {
            skip_white(&value);
            if(*value == '"') {
                expect(&value,'\"');
                value = unescape(value, _unescape_key_buf);
            } else if(*value == '\'') {
                expect(&value,'\'');
                value = unescape(value, _unescape_key_buf);
            } else {
                value = readname(value, _unescape_key_buf);
            }
            expect(&value, ':');
            skip_white(&value);
            switch(*value) {
                case 0:
                    throw_json_data("unexpected end");
                case '{': {
                    value++;
                    ObjectWriter<BUFFER> item(&object,
                                              &_unescape_key_buf[0], _unescape_key_buf.size());
                    value = parseobject(value, item);
                    break;
                }
                case '[': {
                    value++;
                    ArrayWriter<BUFFER> item(&object,
                                             &_unescape_key_buf[0], _unescape_key_buf.size());
                    value = parsearray(value, item);
                    break;
                }
                case '"': {
                    value++;
                    value = unescape(value, _unescape_value_buf);
                    object.putString(&_unescape_key_buf[0], _unescape_key_buf.size(),
                                     &_unescape_value_buf[0], _unescape_value_buf.size());
                    break;
                }
                case '\'': {
                    value++;
                    value = unescape(value, _unescape_value_buf);
                    object.putString(&_unescape_key_buf[0], _unescape_key_buf.size(),
                                     &_unescape_value_buf[0], _unescape_value_buf.size());
                    break;
                }
                case 't':
                    if(strncmp(value, "true", 4) == 0) {
                        object.putNum(&_unescape_key_buf[0], _unescape_key_buf.size(),
                                      true);
                        value += 4;
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                case 'f':
                    if(strncmp(value, "false", 5) == 0) {
                        object.putNum(&_unescape_key_buf[0], _unescape_key_buf.size(),
                                      false);
                        value += 5;
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                case 'n':
                    if(strncmp(value, "null", 4) == 0) {
                        object.putNull(&_unescape_key_buf[0], _unescape_key_buf.size());
                        value += 4;
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                case 'u':
                    if(strncmp(value, "unhex", 5) == 0) {
                        value += 5;
                        expect(&value, '(');
                        expect(&value, '"');
                        value = readbinary(value, _unescape_value_buf);
                        object.putBinary(&_unescape_key_buf[0], _unescape_key_buf.size(), &_unescape_value_buf[0], _unescape_value_buf.size());
                        expect(&value, ')');
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                default:
                    value = object.putNum(&_unescape_key_buf[0], _unescape_key_buf.size(), value);
            }
            skip_white(&value);
            if(*value == ',') {
                value++;
                continue;
            } else if(*value == '}') {
                value++;
                break;
            } else {
                throw_json_data("unexpected char");
            }
        }
        _recursive--;
        return value;

    }
    const char* parsearray(const char* value, ArrayWriter<BUFFER> & array) {
        _recursive++;
        if(_recursive > 100) {
            throw_json_data("recursize too deep");
        }
        skip_white(&value);
        if(*value == ']') {
            _recursive--;
            return ++value;
        }
        while(true) {
            skip_white(&value);
            switch(*value) {
                case 0:
                    throw_json_data("unexpected end");
                case '{': {
                    value++;
                    ObjectWriter<BUFFER> item(&array);
                    value = parseobject(value, item);
                }
                break;
                case '[': {
                    value++;
                    ArrayWriter<BUFFER> item(&array);
                    value = parsearray(value, item);
                }
                break;
                case '"':
                    value++;
                    value = unescape(value, _unescape_value_buf);
                    array.putString(&_unescape_value_buf[0], _unescape_value_buf.size());
                    break;
                case 't':
                    if(strncmp(value, "true", 4) == 0) {
                        array.template putNum<bool>(true);
                        value += 4;
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                case 'f':
                    if(strncmp(value, "false", 5) == 0) {
                        array.template putNum<bool>(false);
                        value += 5;
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                case 'n':
                    if(strncmp(value, "null", 4) == 0) {
                        array.putNull();
                        value += 4;
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                case 'u':
                    if(strncmp(value, "unhex", 5) == 0) {
                        value += 5;
                        expect(&value, '(');
                        expect(&value, '"');
                        value = readbinary(value, _unescape_value_buf);
                        array.putBinary(&_unescape_value_buf[0], _unescape_value_buf.size());
                        expect(&value, ')');
                        break;
                    } else {
                        throw_json_data("unexpected char");
                    }
                default:
                    value = array.putNum(value);
            }
            skip_white(&value);
            if(*value == ',') {
                value++;
                continue;
            } else if(*value == ']') {
                value++;
                break;
            } else {
                throw_json_data("unexpected end");
            }
        }
        _recursive--;
        return value;
    }
    const char* readname(const char* value, std::vector<char> & buf) {
        buf.clear();
        while(true) {
            switch(*value) {
                case ' ':
                case '\t':
                case '\r':
                case '\n':
                    buf.push_back(0);
                    return ++value;
                case ':':
                    buf.push_back(0);
                    return value;
                default:
                    if(*value >= '0' && *value <= '9'
                            || *value >= 'a' && *value <= 'z'
                            || *value >= 'A' && *value <= 'Z'
                            || *value == '_') {
                        if(_encoding) {
                            if(*value < 0) {
                                buf.push_back(*value++);
                                buf.push_back(*value++);
                            } else {
                                buf.push_back(*value++);
                            }
                        } else {
                            buf.push_back(*value++);
                        }
                    } else {
                        throw_json_data("unexpected char");
                    }
            }
        }
    }
    const char* readbinary(const char* value, std::vector<char> & buf) {
        buf.clear();
        while(true) {
            switch(*value) {
                case ' ':
                case '\t':
                case '\r':
                case '\n':
                    ++value;
                    break;
                case '"':
                    return ++value;
                default:
                    buf.push_back((unhex(value++) << 4) + unhex(value++));
            }
        }
        return value;
    }
    char unhex(const char* value) {
        if(*value >= '0' && *value <= '9') {
            return *value - '0';
        } else if(*value >= 'a' && *value <= 'f') {
            return *value - 'a' + 10;
        } else if(*value >= 'A' && *value <= 'F') {
            return *value - 'A' + 10;
        } else {
            throw_json_data("unexpected char");
        }
    }
    void skip_white(const char** value) {
        while(**value == '\t' ||** value == '\n'
                ||** value == ' ' ||** value == '\r') {
            (*value) ++;
        }
    }
    void expect(const char** value, char c) {
        skip_white(value);
        if(**value != c) {
            throw_json_data("unexpected char");
        }
        (*value) ++;
    }
};

}










#endif  //__JSONPARSER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
