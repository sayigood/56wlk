
/**
 * @file compack.h
 * @brief
 *
 **/




#ifndef  __COMPACK_H_
#define  __COMPACK_H_


#include <compack/buffer.h>
#include <compack/iterator.h>
#include <compack/reader.h>
#include <compack/writer.h>
#include <compack/exception.h>
#include <compack/jsonformatter.h>
#include <compack/jsonparser.h>
#include "compack/bsonparser.h"
#include "compack/bsonformatter.h"
//#include <compack/mc_pack_core.h>

namespace compack {

namespace buffer {
typedef Buffer buffer;
typedef ObjectWriter<buffer> Object;
typedef ArrayWriter<buffer> Array;
typedef IsoArrayWriter<buffer, uint8_t> IsoArrayUint8;
typedef IsoArrayWriter<buffer, uint16_t> IsoArrayUint16;
typedef IsoArrayWriter<buffer, uint32_t> IsoArrayUint32;
typedef IsoArrayWriter<buffer, uint64_t> IsoArrayUint64;
typedef IsoArrayWriter<buffer, int8_t> IsoArrayInt8;
typedef IsoArrayWriter<buffer, int16_t> IsoArrayInt16;
typedef IsoArrayWriter<buffer, int32_t> IsoArrayInt32;
typedef IsoArrayWriter<buffer, int64_t> IsoArrayInt64;
typedef ObjectIsoArrayWriter<buffer> ObjectIsoArray;
typedef compack::Reader<buffer> Reader;
typedef compack::JsonParser<buffer> JsonParser;
typedef compack::BsonParser<buffer> BsonParser;
typedef compack::JsonFormatter<buffer> JsonFormatter;
typedef compack::BsonFormatter<buffer> BsonFormatter;
}

namespace autobuffer {
typedef AutoBuffer buffer;
typedef ObjectWriter<buffer> Object;
typedef ArrayWriter<buffer> Array;
typedef IsoArrayWriter<buffer, uint8_t> IsoArrayUint8;
typedef IsoArrayWriter<buffer, uint16_t> IsoArrayUint16;
typedef IsoArrayWriter<buffer, uint32_t> IsoArrayUint32;
typedef IsoArrayWriter<buffer, uint64_t> IsoArrayUint64;
typedef IsoArrayWriter<buffer, int8_t> IsoArrayInt8;
typedef IsoArrayWriter<buffer, int16_t> IsoArrayInt16;
typedef IsoArrayWriter<buffer, int32_t> IsoArrayInt32;
typedef IsoArrayWriter<buffer, int64_t> IsoArrayInt64;
typedef ObjectIsoArrayWriter<buffer> ObjectIsoArray;
typedef compack::Reader<buffer> Reader;
typedef compack::JsonParser<buffer> JsonParser;
typedef compack::BsonParser<buffer> BsonParser;
typedef compack::JsonFormatter<buffer> JsonFormatter;
typedef compack::BsonFormatter<buffer> BsonFormatter;
}

namespace writevbuffer {
typedef WritevBuffer<AutoBuffer> buffer;
typedef ObjectWriter<buffer> Object;
typedef ArrayWriter<buffer> Array;
typedef IsoArrayWriter<buffer, uint8_t> IsoArrayUint8;
typedef IsoArrayWriter<buffer, uint16_t> IsoArrayUint16;
typedef IsoArrayWriter<buffer, uint32_t> IsoArrayUint32;
typedef IsoArrayWriter<buffer, uint64_t> IsoArrayUint64;
typedef IsoArrayWriter<buffer, int8_t> IsoArrayInt8;
typedef IsoArrayWriter<buffer, int16_t> IsoArrayInt16;
typedef IsoArrayWriter<buffer, int32_t> IsoArrayInt32;
typedef IsoArrayWriter<buffer, int64_t> IsoArrayInt64;
typedef ObjectIsoArrayWriter<buffer> ObjectIsoArray;
typedef compack::Reader<buffer> Reader;
typedef compack::JsonParser<buffer> JsonParser;
typedef compack::BsonParser<buffer> BsonParser;
typedef compack::JsonFormatter<buffer> JsonFormatter;
typedef compack::BsonFormatter<buffer> BsonFormatter;
}

}

#endif  //__COMPACK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
