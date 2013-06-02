#ifndef _PROTOCOL_H
#define _PROTOCOL_H
#include <stdint.h>

namespace compack {

namespace protocol {

typedef signed char mc_int8_t;
typedef short mc_int16_t;
typedef int mc_int32_t;
typedef long long mc_int64_t;
typedef unsigned char mc_uint8_t;
typedef unsigned short mc_uint16_t;
typedef unsigned int mc_uint32_t;
typedef unsigned long long mc_uint64_t;

enum mc_pack_type_id {
    INVALID = 0x0,
    OBJECT = 0x10,
    ARRAY = 0x20,
    ISOARRAY = 0x30,
    OBJECTISOARRAY = 0x40,
    STRING = 0x50,
    BINARY = 0x60,
    INT_8 = 0x11,
    INT_16 = 0x12,
    INT_32 = 0x14,
    INT_64 = 0x18,
    UINT_8 = 0x21,
    UINT_16 = 0x22,
    UINT_32 = 0x24,
    UINT_64 = 0x28,
    BOOL = 0x31,
    FLOAT = 0x44,
    DOUBLE = 0x48,
    DATE = 0x58,
    MCPACK_NULL = 0x61,
    SHORT_ITEM = 0x80,
    FIXED_ITEM = 0xf,
    DELETED_ITEM = 0x70
};

#ifdef _MSC_VER
#pragma pack(push)
#pragma pack(1)
#endif
struct long_pack {
    unsigned int item_count;
}
#ifdef __GNUC__
__attribute__((__packed__));
#else
;
#endif
struct isoarray {
    unsigned char type;
}
#ifdef __GNUC__
__attribute__((__packed__));
#else
;
#endif
struct short_item {
    unsigned char type;
    unsigned char name_len;
    inline unsigned int name_offset() const {
        return sizeof(short_item);
    }
    inline unsigned int size() const {
        return sizeof(short_item) + name_len + (type & FIXED_ITEM);
    }
    inline unsigned int content_len() const {
        return (type & FIXED_ITEM);
    }
    inline unsigned int content_offset() const {
        return sizeof(short_item) + name_len;
    }
}
#ifdef __GNUC__
__attribute__((__packed__));
#else
;
#endif
struct short_vitem {
    unsigned char type;
    unsigned char name_len;
    unsigned char _content_len;
    inline unsigned int name_offset() const {
        return sizeof(short_vitem);
    }
    inline unsigned int content_len() const {
        return _content_len;
    }
    inline unsigned int size() const {
        return sizeof(short_vitem) + name_len + _content_len;
    }
    inline unsigned int content_offset() const {
        return sizeof(short_vitem) + name_len;
    }
}
#ifdef __GNUC__
__attribute__((__packed__));
#else
;
#endif
struct long_vitem {
    unsigned char type;
    unsigned char name_len;
    unsigned int _content_len;
    inline unsigned int name_offset() const {
        return sizeof(long_vitem);
    }
    inline unsigned int content_len() const {
        return _content_len;
    }
    inline unsigned int size() const {
        return sizeof(long_vitem) + name_len + _content_len;
    }
    inline unsigned int content_offset() const {
        return sizeof(long_vitem) + name_len;
    }
}
#ifdef __GNUC__
__attribute__((__packed__));
#else
;
#endif
#ifdef _MSC_VER
#pragma pack(pop)
#endif

template<typename T>
struct Mcpack_type_id_traits {
};

template<>
struct Mcpack_type_id_traits<int8_t> {
    static const int TYPE_ID = INT_8;
};

template<>
struct Mcpack_type_id_traits<int16_t> {
    static const mc_pack_type_id TYPE_ID = INT_16;
};

template<>
struct Mcpack_type_id_traits<int32_t> {
    static const mc_pack_type_id TYPE_ID = INT_32;
};

template<>
struct Mcpack_type_id_traits<int64_t> {
    static const mc_pack_type_id TYPE_ID = INT_64;
};

template<>
struct Mcpack_type_id_traits<uint8_t> {
    static const mc_pack_type_id TYPE_ID = UINT_8;
};

template<>
struct Mcpack_type_id_traits<uint16_t> {
    static const mc_pack_type_id TYPE_ID = UINT_16;
};

template<>
struct Mcpack_type_id_traits<uint32_t> {
    static const mc_pack_type_id TYPE_ID = UINT_32;
};

template<>
struct Mcpack_type_id_traits<uint64_t> {
    static const mc_pack_type_id TYPE_ID = UINT_64;
};

template<>
struct Mcpack_type_id_traits<float> {
    static const mc_pack_type_id TYPE_ID = FLOAT;
};

template<>
struct Mcpack_type_id_traits<double> {
    static const mc_pack_type_id TYPE_ID = DOUBLE;
};

template<>
struct Mcpack_type_id_traits<bool> {
    static const mc_pack_type_id TYPE_ID = BOOL;
};

}
}

#endif
