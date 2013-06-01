/***************************************************************************
 * 
 * Copyright (c) 2007 Baidu.com, Inc. All Rights Reserved
 * $Id: unitest.cpp,v 1.18 2008/09/04 06:51:09 zhouzr Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file unitest.cpp
 * @author zhouzr(zhouzerui@baidu.com)
 * @date 2007/12/27 10:48:54
 * @version $Revision: 1.18 $ 
 * @brief unit test for mc_datapack
 *  
 **/

#include <stdint.h>
#include "mc_pack.h"

#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

typedef struct _internal_item_t {
	char tag[4];
	int type;
	u_int size;
	u_int offset;
	char data[1];		  /* field_name + '\0' + data       */
} internal_item_t;

const u_int DEF_PACK_SIZE = 8192;
const char *ARR_NAME = "mc_array";
const char *OBJ_NAME = "mc_object";

class mytest : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(mytest);
	CPPUNIT_TEST(packvalid_1);
	CPPUNIT_TEST(pack_create_1);
	CPPUNIT_TEST(array_create_1);
	CPPUNIT_TEST(array_create_2);
	CPPUNIT_TEST(object_create_1);
	CPPUNIT_TEST(object_create_2);
	CPPUNIT_TEST(int_put_1);
	CPPUNIT_TEST(int_put_2);
	CPPUNIT_TEST(int_put_3);
	CPPUNIT_TEST(int_put_4);
	CPPUNIT_TEST(str_put_1);
	CPPUNIT_TEST(int_get_1);
	CPPUNIT_TEST(int_get_2);
	CPPUNIT_TEST(int_get_3);
	CPPUNIT_TEST(int_get_4);
	CPPUNIT_TEST(int_get_5);
	CPPUNIT_TEST(int_get_6);
	CPPUNIT_TEST(int_get_7);
	CPPUNIT_TEST(int_get_8);
	CPPUNIT_TEST(str_get_1);
	CPPUNIT_TEST(get_size_1);
	CPPUNIT_TEST(get_count_1);
	CPPUNIT_TEST(array_1);
	CPPUNIT_TEST(array_2);
	CPPUNIT_TEST(array_3);
	//CPPUNIT_TEST(remove_1);
	//CPPUNIT_TEST(remove_2);
	//CPPUNIT_TEST(remove_3);
	//CPPUNIT_TEST(remove_4);
	CPPUNIT_TEST(array_get_1);
	CPPUNIT_TEST(array_get_2);
	CPPUNIT_TEST(object_get_1);
	CPPUNIT_TEST(object_get_2);
	CPPUNIT_TEST(raw_put_get_1);
	CPPUNIT_TEST(raw_put_get_2);
	CPPUNIT_TEST(raw_put_get_3);
	CPPUNIT_TEST(traverse_1);
	CPPUNIT_TEST(traverse_2);
	CPPUNIT_TEST(pack_close_1);
	CPPUNIT_TEST(traverse_3);
	CPPUNIT_TEST(pack_reopen_1);
	CPPUNIT_TEST(perror_1);
	CPPUNIT_TEST(ptype_1);
	CPPUNIT_TEST(dead_cycle_1);
	CPPUNIT_TEST(pack_copy_1);
	CPPUNIT_TEST(pack_copy_2);
	CPPUNIT_TEST(pack_copy_3);
	CPPUNIT_TEST(pack_copy_4);
	CPPUNIT_TEST(cmp_str_1);
	CPPUNIT_TEST(cmp_str_2);
	CPPUNIT_TEST(put_nstr_1);
	CPPUNIT_TEST(putf_1);
	CPPUNIT_TEST(putf_2);
	CPPUNIT_TEST(getf_1);
	CPPUNIT_TEST(getf_2);
	CPPUNIT_TEST(pack2str_1);
	CPPUNIT_TEST(pack2str_2);
	CPPUNIT_TEST(get_item_1);
	CPPUNIT_TEST(reset_1);
	CPPUNIT_TEST(creatf_1);
	CPPUNIT_TEST(text2pack_1);
	CPPUNIT_TEST(text2pack_2);
	CPPUNIT_TEST(text2pack_3);
	CPPUNIT_TEST(text2pack_4);
	CPPUNIT_TEST(text2pack_5);
	CPPUNIT_TEST(foreach_1);
	CPPUNIT_TEST(get_from_item_1);
	CPPUNIT_TEST(put_item_1);
	CPPUNIT_TEST_SUITE_END();

	private:
		char buffer[DEF_PACK_SIZE];

	public:
		void setUp(void) {
			//memset(buffer, 0, sizeof(buffer));
		}

		void tearDown(void) {
		}

		void packvalid_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));
			CPPUNIT_ASSERT(! mc_pack_valid(pp, 1));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));
			CPPUNIT_ASSERT(mc_pack_valid(ap, 0));
		}

		void pack_create_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(pp->item_count == 0);
			CPPUNIT_ASSERT(pp->bytes_left == DEF_PACK_SIZE - pp->cur_pos);
		}

		void array_create_1(void) {
		  /* succeed       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			internal_item_t *t = (internal_item_t *)ap;
			CPPUNIT_ASSERT(t->type == MC_PT_ARR);
			CPPUNIT_ASSERT(t->size > sizeof(internal_item_t));
			CPPUNIT_ASSERT(t->offset != 0);

		  /* succ for recursive pointer       */
			ap = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));
			ap = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

		  /* fail for recursive pointer       */
			ap = mc_pack_put_array(ap, "abc");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) < 0);
			ap = mc_pack_put_array(ap, "abc");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) < 0);

		  /* fail for invalid name       */
			ap = mc_pack_put_array(pp, "a,b");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == MC_PE_BAD_NAME);
			ap = mc_pack_put_array(pp, "a:b");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == MC_PE_BAD_NAME);

		  /* fail for duplicate       */
			ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == MC_PE_ALREADY_EXIST);

		}

		void array_create_2(void) {
		  /* succeed       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

		  /* in array       */
			mc_pack_t *ap1 = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap1) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap1, DEF_PACK_SIZE));

			mc_pack_t *ap2 = mc_pack_put_array(ap, "1");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap2) < 0);

			ap2 = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap2) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap2, DEF_PACK_SIZE));
		}

		void object_create_1(void) {
		  /* succeed       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			internal_item_t *t = (internal_item_t *)ap;
			CPPUNIT_ASSERT(t->type == MC_PT_OBJ);
			CPPUNIT_ASSERT(t->size > sizeof(internal_item_t));
			CPPUNIT_ASSERT(t->offset != 0);

		  /* succeed for recursive pointer       */
			ap = mc_pack_put_object(ap, "abc");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));
			ap = mc_pack_put_object(ap, "abc");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

		  /* fail for invalid name       */
			ap = mc_pack_put_object(pp, "a,b");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) < 0);
			ap = mc_pack_put_object(pp, "a:b");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) < 0);

		  /* fail for duplicate       */
			ap = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) < 0);

		}

		void object_create_2(void) {
		  /* succeed       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

		  /* in array       */
			mc_pack_t *op1 = mc_pack_put_object(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op1) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op1, DEF_PACK_SIZE));

			mc_pack_t *op2 = mc_pack_put_object(ap, "1");
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op2) < 0);

			op2 = mc_pack_put_object(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op2) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op2, DEF_PACK_SIZE));
		}

		void int_put_1(void) {		  /* for mc_pack_put_int32       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, "int32", 1234);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
			
			ret = mc_pack_put_int32(pp, "int32", 12345);		  /* fail for dup name       */
			CPPUNIT_ASSERT(0 > ret);

			ret = mc_pack_put_int32(pp, "int32:1", 12345);		  /* fail for invalid name       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_uint32(ap, "int32", 1234);		  /* fail for array       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, "int32", 1234);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
		}

		void int_put_2(void) {		  /* for mc_pack_put_uint32       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_uint32(pp, "int32", 1234);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
			
			ret = mc_pack_put_uint32(pp, "int32", 12345);		  /* fail for dup name       */
			CPPUNIT_ASSERT(0 > ret);

			ret = mc_pack_put_uint32(pp, "int32:1", 12345);		  /* fail for invalid name       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_uint32(ap, "int32", 1234);		  /* fail for array       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_uint32(op, "int32", 1234);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
		}

		void int_put_3(void) {		  /* for mc_pack_put_int64       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int64(pp, "int64", 123443543523455ll);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
			
			ret = mc_pack_put_int64(pp, "int64", 1234543543523455ll);		  /* fail for dup name       */
			CPPUNIT_ASSERT(0 > ret);

			ret = mc_pack_put_int64(pp, "int64:1", 1234543543523455ll);		  /* fail for invalid name       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_int64(ap, "int64", 123443543523455ll);		  /* fail for array       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int64(op, "int64", 123443543523455ll);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
		}

		void int_put_4(void) {		  /* for mc_pack_put_uint64       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_uint64(pp, "int64", 123434234234234ull);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
			
			ret = mc_pack_put_uint64(pp, "int64", 1234534234234234ull);		  /* fail for dup name       */
			CPPUNIT_ASSERT(0 > ret);

			ret = mc_pack_put_uint64(pp, "int64:1", 1234534234234234ull);		  /* fail for invalid name       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_uint64(ap, "int64", 123434234234234ull);		  /* fail for array       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_uint64(op, "int64", 123434234234234ull);		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
		}

		void str_put_1(void) {		  /* for mc_pack_put_str       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_str(pp, "string1", "hahhahsifhwiehf");		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
			
			ret = mc_pack_put_str(pp, "string1", "hello world");		  /* fail for dup name       */
			CPPUNIT_ASSERT(0 > ret);

			ret = mc_pack_put_str(pp, "string:1", "heihei");		  /* fail for invalid name       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_str(ap, "string2", "heihei");		  /* fail for array       */
			CPPUNIT_ASSERT(0 > ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_str(op, "string2", "拉拉拉拉");		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);
		}

		void int_get_1(void) {		  /* for mc_pack_get_int32       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, "int32", 1234);
			CPPUNIT_ASSERT(0 == ret);
			
			int result = 0;
			ret = mc_pack_get_int32(pp, "int32", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1234 == result);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, "int32", 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_int32(op, "int32", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(12345 == result);
		}

		void int_get_2(void) {		  /* for mc_pack_get_uint32       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_uint32(pp, "int32", 1234);
			CPPUNIT_ASSERT(0 == ret);
			
			u_int result = 0;
			ret = mc_pack_get_uint32(pp, "int32", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1234 == result);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_uint32(op, "int32", 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_uint32(op, "int32", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(12345 == result);
		}

		void int_get_3(void) {		  /* for mc_pack_get_int64       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int64(pp, "int64", 12347454375638764ll);
			CPPUNIT_ASSERT(0 == ret);
			
			mc_int64_t result = 0;
			ret = mc_pack_get_int64(pp, "int64", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(12347454375638764ll == result);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int64(op, "int64", 1234587485783455ll);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_int64(op, "int64", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1234587485783455ll == result);
		}

		void int_get_4(void) {		  /* for mc_pack_get_uint64       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_uint64(pp, "int64", 12347454375638764ull);
			CPPUNIT_ASSERT(0 == ret);
			
			mc_uint64_t result = 0;
			ret = mc_pack_get_uint64(pp, "int64", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(12347454375638764ull == result);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_uint64(op, "int64", 1234587485783455ull);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_uint64(op, "int64", &result);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1234587485783455ull == result);
		}

		void int_get_5(void) {		  /* for the new version mc_pack_get_int32      */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = 0;
			ret += mc_pack_put_int32(pp, "i32", 123);
			ret += mc_pack_put_uint32(pp, "u32", 123);
			ret += mc_pack_put_int64(pp, "i64", 123);
			ret += mc_pack_put_uint64(pp, "u64", 123);
			ret += mc_pack_put_str(pp, "str", "123");
			ret += mc_pack_put_str(pp, "-str", "-123");
			ret += mc_pack_put_int32(pp, "i32_max", INT_MAX);
			ret += mc_pack_put_uint32(pp, "u32_max", UINT_MAX);
			ret += mc_pack_put_int64(pp, "i64_max", LLONG_MAX);
			ret += mc_pack_put_uint64(pp, "u64_max", ULLONG_MAX);

			CPPUNIT_ASSERT(0 == ret);

			int32_t i32 = 0;

			ret = mc_pack_get_int32(pp, "i32", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i32);
			ret = mc_pack_get_int32(pp, "i32_max", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(INT_MAX == i32);

			ret = mc_pack_get_int32(pp, "u32", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i32);
			ret = mc_pack_get_int32(pp, "u32_max", &i32);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

			ret = mc_pack_get_int32(pp, "i64", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i32);
			ret = mc_pack_get_int32(pp, "i64_max", &i32);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

			ret = mc_pack_get_int32(pp, "u64", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i32);
			ret = mc_pack_get_int32(pp, "u64_max", &i32);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

			ret = mc_pack_get_int32(pp, "str", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i32);
			ret = mc_pack_get_int32(pp, "-str", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-123 == i32);

		}

		void int_get_6(void) {		  /* for the new version mc_pack_get_uint32      */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = 0;
			ret += mc_pack_put_int32(pp, "i32", 123);
			ret += mc_pack_put_uint32(pp, "u32", 123);
			ret += mc_pack_put_int64(pp, "i64", 123);
			ret += mc_pack_put_uint64(pp, "u64", 123);
			ret += mc_pack_put_str(pp, "str", "123");
			ret += mc_pack_put_str(pp, "-str", "-123");
			ret += mc_pack_put_int32(pp, "i32_max", INT_MAX);
			ret += mc_pack_put_uint32(pp, "u32_max", UINT_MAX);
			ret += mc_pack_put_int64(pp, "i64_max", LLONG_MAX);
			ret += mc_pack_put_uint64(pp, "u64_max", ULLONG_MAX);

			CPPUNIT_ASSERT(0 == ret);

			u_int32_t u32 = 0;

			ret = mc_pack_get_uint32(pp, "i32", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u32);
			ret = mc_pack_get_uint32(pp, "i32_max", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(INT_MAX == u32);

			ret = mc_pack_get_uint32(pp, "u32", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u32);
			ret = mc_pack_get_uint32(pp, "u32_max", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(UINT_MAX == u32);

			ret = mc_pack_get_uint32(pp, "i64", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u32);
			ret = mc_pack_get_uint32(pp, "i64_max", &u32);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

			ret = mc_pack_get_uint32(pp, "u64", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u32);
			ret = mc_pack_get_uint32(pp, "u64_max", &u32);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

			ret = mc_pack_get_uint32(pp, "str", &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u32);
			ret = mc_pack_get_uint32(pp, "-str", &u32);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

		}

		void int_get_7(void) {		  /* for the new version mc_pack_get_int64      */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = 0;
			ret += mc_pack_put_int32(pp, "i32", 123);
			ret += mc_pack_put_uint32(pp, "u32", 123);
			ret += mc_pack_put_int64(pp, "i64", 123);
			ret += mc_pack_put_uint64(pp, "u64", 123);
			ret += mc_pack_put_str(pp, "str", "123");
			ret += mc_pack_put_str(pp, "-str", "-123");
			ret += mc_pack_put_int32(pp, "i32_max", INT_MAX);
			ret += mc_pack_put_uint32(pp, "u32_max", UINT_MAX);
			ret += mc_pack_put_int64(pp, "i64_max", LLONG_MAX);
			ret += mc_pack_put_uint64(pp, "u64_max", ULLONG_MAX);

			CPPUNIT_ASSERT(0 == ret);

			mc_int64_t i64 = 0;

			ret = mc_pack_get_int64(pp, "i32", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i64);
			ret = mc_pack_get_int64(pp, "i32_max", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(INT_MAX == i64);

			ret = mc_pack_get_int64(pp, "u32", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i64);
			ret = mc_pack_get_int64(pp, "u32_max", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(UINT_MAX == i64);

			ret = mc_pack_get_int64(pp, "i64", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i64);
			ret = mc_pack_get_int64(pp, "i64_max", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(LLONG_MAX == i64);

			ret = mc_pack_get_int64(pp, "u64", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i64);
			ret = mc_pack_get_int64(pp, "u64_max", &i64);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

			ret = mc_pack_get_int64(pp, "str", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == i64);
			ret = mc_pack_get_int64(pp, "-str", &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-123 == i64);

		}

		void int_get_8(void) {		  /* for the new version mc_pack_get_uint64      */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = 0;
			ret += mc_pack_put_int32(pp, "i32", 123);
			ret += mc_pack_put_uint32(pp, "u32", 123);
			ret += mc_pack_put_int64(pp, "i64", 123);
			ret += mc_pack_put_uint64(pp, "u64", 123);
			ret += mc_pack_put_str(pp, "str", "123");
			ret += mc_pack_put_str(pp, "-str", "-123");
			ret += mc_pack_put_int32(pp, "i32_max", INT_MAX);
			ret += mc_pack_put_uint32(pp, "u32_max", UINT_MAX);
			ret += mc_pack_put_int64(pp, "i64_max", LLONG_MAX);
			ret += mc_pack_put_uint64(pp, "u64_max", ULLONG_MAX);

			CPPUNIT_ASSERT(0 == ret);

			mc_uint64_t u64 = 0;

			ret = mc_pack_get_uint64(pp, "i32", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u64);
			ret = mc_pack_get_uint64(pp, "i32_max", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(INT_MAX == u64);

			ret = mc_pack_get_uint64(pp, "u32", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u64);
			ret = mc_pack_get_uint64(pp, "u32_max", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(UINT_MAX == u64);

			ret = mc_pack_get_uint64(pp, "i64", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u64);
			ret = mc_pack_get_uint64(pp, "i64_max", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(LLONG_MAX == u64);

			ret = mc_pack_get_uint64(pp, "u64", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u64);
			ret = mc_pack_get_uint64(pp, "u64_max", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(ULLONG_MAX == u64);

			ret = mc_pack_get_uint64(pp, "str", &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(123 == u64);
			ret = mc_pack_get_uint64(pp, "-str", &u64);
			CPPUNIT_ASSERT(MC_PE_FOUND_BUT_TYPE == ret);

		}

		void str_get_1(void) {		  /* for mc_pack_get_str       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_str(pp, "string1", "hahhahsifhwiehf");		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);

			const char *result = mc_pack_get_str(pp, "string1");
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(result));
			CPPUNIT_ASSERT(0 == strcmp(result, "hahhahsifhwiehf"));
			
			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_str(op, "string2", "拉拉拉拉");		  /* succ       */
			CPPUNIT_ASSERT(0 == ret);

			result = mc_pack_get_str(op, "string2");
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(result));
			CPPUNIT_ASSERT(0 == strcmp(result, "拉拉拉拉"));
		}

		void get_size_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int size = mc_pack_get_size(pp);
			CPPUNIT_ASSERT(size > 0);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);		  /* fail for array       */
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));
			int new_size = mc_pack_get_size(ap);
			CPPUNIT_ASSERT(new_size < 0);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);		  /* fail for object       */
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));
			new_size = mc_pack_get_size(op);
			CPPUNIT_ASSERT(new_size < 0);

			new_size = mc_pack_get_size(pp);		  /* become bigger       */
			CPPUNIT_ASSERT(new_size > size);

			//fprintf(stderr, "\nDEBUG: size:%d bytes_left:%d total:%d\n", new_size, pp->bytes_left, DEF_PACK_SIZE);
		}

		void get_count_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int item_count = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(0 == item_count);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);		  /* succ for array       */
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));
			item_count = mc_pack_get_item_count(ap);
			CPPUNIT_ASSERT(item_count == 0);

			mc_pack_put_int32(ap, NULL, 1234);
			item_count = mc_pack_get_item_count(ap);
			CPPUNIT_ASSERT(item_count == 1);

			mc_pack_t *ap1 = mc_pack_put_array(ap, NULL);		  /* succ for 2nd array       */
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap1) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap1, DEF_PACK_SIZE));
			item_count = mc_pack_get_item_count(ap1);
			CPPUNIT_ASSERT(item_count == 0);

			mc_pack_put_int32(ap1, NULL, 1234);
			item_count = mc_pack_get_item_count(ap1);
			CPPUNIT_ASSERT(item_count == 1);

			item_count = mc_pack_get_item_count(ap);		  /* not include 2nd array's item       */
			CPPUNIT_ASSERT(item_count == 2);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);		  /* succ for object       */
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));
			item_count = mc_pack_get_item_count(op);
			CPPUNIT_ASSERT(item_count == 0);

			mc_pack_put_int32(op, "int32", 12345);
			item_count = mc_pack_get_item_count(op);
			CPPUNIT_ASSERT(item_count == 1);

			item_count = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(2 == item_count);		  /* not include array & object's item       */
		}

		void array_1(void) {		  /* fail       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			ret = mc_pack_put_uint32(pp, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			ret = mc_pack_put_int64(pp, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			ret = mc_pack_put_uint64(pp, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);		  /* succ for object       */
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			ret = mc_pack_put_uint32(op, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			ret = mc_pack_put_int64(op, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

			ret = mc_pack_put_uint64(op, NULL, 1234);
			CPPUNIT_ASSERT(ret < 0);

		}

		void array_2(void) {		  /* put       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(ap, NULL, 1234);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_int32(ap, NULL, 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_uint32(ap, NULL, 1234);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_uint32(ap, NULL, 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_int64(ap, NULL, 1234);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_int64(ap, NULL, 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_uint64(ap, NULL, 1234);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_uint64(ap, NULL, 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_str(ap, NULL, "string hello");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_str(ap, NULL, "string hello world");
			CPPUNIT_ASSERT(0 == ret);

		}

		void array_3(void) {		  /* get       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

		  /* int32       */
			int ret = mc_pack_put_int32(ap, NULL, -1234);
			CPPUNIT_ASSERT(0 == ret);

			int r_i32 = 0;
			ret = mc_pack_get_int32_arr(ap, 0, &r_i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-1234 == r_i32);

		  /* uint32       */
			ret = mc_pack_put_uint32(ap, NULL, 3000000000u);
			CPPUNIT_ASSERT(0 == ret);

			u_int r_u32;
			ret = mc_pack_get_uint32_arr(ap, 1, &r_u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(3000000000u == r_u32);

		  /* int64       */
			ret = mc_pack_put_int64(ap, NULL, -400000000000ll);
			CPPUNIT_ASSERT(0 == ret);

			mc_int64_t r_i64;
			ret = mc_pack_get_int64_arr(ap, 2, &r_i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-400000000000ll == r_i64);

		  /* uint64       */
			ret = mc_pack_put_uint64(ap, NULL, 400000000000ull);
			CPPUNIT_ASSERT(0 == ret);

			mc_uint64_t r_u64;
			ret = mc_pack_get_uint64_arr(ap, 3, &r_u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(400000000000ull == r_u64);

		  /* string       */
			ret = mc_pack_put_str(ap, NULL, "hello zerray");
			CPPUNIT_ASSERT(0 == ret);

			const char *r_string = NULL;
			r_string = mc_pack_get_str_arr(ap, 4);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(r_string));
			CPPUNIT_ASSERT(strcmp(r_string, "hello zerray") == 0);
		}

#if 0
		void remove_1(void) {		  /* single item       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(pp, "s1", "hello world");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(1 == ret);

			const char *r_string = NULL;
			r_string = mc_pack_get_str(pp, "s1");
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(r_string));
			CPPUNIT_ASSERT(strcmp(r_string, "hello world") == 0);

			ret = mc_pack_del_item(pp, "s1");
			CPPUNIT_ASSERT(0 == ret);
			
			r_string = mc_pack_get_str(pp, "s1");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(0 == ret);

		}

		void remove_2(void) {		  /* array       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(ap, NULL, "okokok");
			CPPUNIT_ASSERT(0 == ret);

			const char *r_string = NULL;
			r_string = mc_pack_get_str_arr(ap, 0);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(r_string));

			ret = mc_pack_put_str(ap, NULL, "okokok_0");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_2");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_3");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_4");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(1 == ret);
			ret = mc_pack_get_item_count(ap);
			CPPUNIT_ASSERT(6 == ret);

			ret = mc_pack_del_item(pp, ARR_NAME);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(0 == mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(0 == ret);

			r_string = mc_pack_get_str_arr(ap, 0);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str_arr(ap, 1);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str_arr(ap, 2);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str_arr(ap, 3);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str_arr(ap, 4);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));

			r_string = mc_pack_get_str(pp, "mc_array:0");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str(pp, "mc_array:1");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str(pp, "mc_array:2");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str(pp, "mc_array:3");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str(pp, "mc_array:4");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));
		}

		void remove_3(void) {		  /* object       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(op, "space_name", "Joey");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(op, "space_url", "joey");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(op, "op_username", "zhouzerui");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(1 == ret);
			ret = mc_pack_get_item_count(op);
			CPPUNIT_ASSERT(3 == ret);

			ret = mc_pack_del_item(op, "space_name");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_item_count(op);
			CPPUNIT_ASSERT(2 == ret);

			const char *r_string = NULL;
			r_string = mc_pack_get_str(op, "space_name");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str(op, "space_url");
			CPPUNIT_ASSERT(MC_PE_SUCCESS == MC_PACK_PTR_ERR(r_string));
			CPPUNIT_ASSERT(0 == strcmp(r_string, "joey"));

			mc_pack_del_item(pp, OBJ_NAME);
			r_string = mc_pack_get_str(op, "space_url");
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));
			r_string = mc_pack_get_str(op, "op_username");
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == MC_PACK_PTR_ERR(r_string));

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(0 == ret);

		}

		void remove_4(void) {		  /* del not found       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(pp, "s1", "hello world");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(1 == ret);

		  /* not found       */
			ret = mc_pack_del_item(pp, ",s1");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == ret);
			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(1 == ret);
			ret = mc_pack_del_item(pp, "s2");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == ret);
			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(1 == ret);

			const char *r_string = NULL;
			r_string = mc_pack_get_str(pp, "s1");
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(r_string));
			CPPUNIT_ASSERT(strcmp(r_string, "hello world") == 0);

			ret = mc_pack_del_item(pp, "s1");
			CPPUNIT_ASSERT(0 == ret);
			
			r_string = mc_pack_get_str(pp, "s1");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == MC_PACK_PTR_ERR(r_string));

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(0 == ret);

		}
#endif

		void array_get_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(ap, NULL, "okokok_0");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_2");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_3");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "okokok_4");
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap1 = mc_pack_get_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(ap1 == ap);

		  /* get wrong type       */
			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			mc_pack_t *op1 = mc_pack_get_array(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op1) < 0);
		}

		void array_get_2(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *ap1 = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap1) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap1, DEF_PACK_SIZE));
			mc_pack_t *ap2 = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap2) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap2, DEF_PACK_SIZE));
			mc_pack_t *ap3 = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap3) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap3, DEF_PACK_SIZE));

			mc_pack_t *ap11 = mc_pack_get_array_arr(ap, 0);
			CPPUNIT_ASSERT(ap1 == ap11);
			mc_pack_t *ap22 = mc_pack_get_array_arr(ap, 1);
			CPPUNIT_ASSERT(ap2 == ap22);
			mc_pack_t *ap33 = mc_pack_get_array_arr(ap, 2);
			CPPUNIT_ASSERT(ap3 == ap33);

		}

		void object_get_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(op, "space_name", "Joey");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(op, "space_url", "joey");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(op, "op_username", "zhouzerui");
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op1 = mc_pack_get_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(op1 == op);

		  /* get wrong type       */
			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *ap1 = mc_pack_get_object(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap1) < 0);
		}

		void object_get_2(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *op1 = mc_pack_put_object(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op1) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op1, DEF_PACK_SIZE));
			mc_pack_t *op2 = mc_pack_put_object(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op2) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op2, DEF_PACK_SIZE));
			mc_pack_t *op3 = mc_pack_put_object(ap, NULL);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op3) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op3, DEF_PACK_SIZE));

			mc_pack_t *op11 = mc_pack_get_object_arr(ap, 0);
			CPPUNIT_ASSERT(op1 == op11);
			mc_pack_t *op22 = mc_pack_get_object_arr(ap, 1);
			CPPUNIT_ASSERT(op2 == op22);
			mc_pack_t *op33 = mc_pack_get_object_arr(ap, 2);
			CPPUNIT_ASSERT(op3 == op33);
		}

		void raw_put_get_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			int ret = -1;
			const u_int size = 64;
			char bindata[size];
			const char *r_buffer;
			u_int r_size = size * 2;
			for (u_int i = 0; i < size; ++i) bindata[i] = i;
			ret = mc_pack_put_raw(pp, "bindata", bindata, size);
			CPPUNIT_ASSERT(0 == ret);
			r_buffer = (const char *)mc_pack_get_raw(pp, "bindata", &r_size);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(r_size == size);
			for (u_int i = 0; i < size; ++i)
				CPPUNIT_ASSERT(bindata[i] == r_buffer[i]);

		  /* fail for dup       */
			ret = mc_pack_put_raw(pp, "bindata", bindata, size);
			CPPUNIT_ASSERT(ret < 0);

		  /* in object       */
			ret = mc_pack_put_raw(op, "bindata", bindata, size);
			CPPUNIT_ASSERT(0 == ret);
			r_buffer = (const char *)mc_pack_get_raw(op, "bindata", &r_size);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(r_buffer));
			CPPUNIT_ASSERT(r_size == size);
			for (u_int i = 0; i < size; ++i)
				CPPUNIT_ASSERT(bindata[i] == r_buffer[i]);

		}

		void raw_put_get_2(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			int ret = -1;
			const u_int size = 64;
			char bindata[size];
			const char *r_buffer = NULL;
			u_int r_size = size * 2;
			for (u_int i = 0; i < size; ++i) bindata[i] = i;
			ret = mc_pack_put_raw(ap, NULL, bindata, size);
			CPPUNIT_ASSERT(0 == ret);
			r_buffer = (const char *)mc_pack_get_raw_arr(ap, 0, &r_size);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(r_size == size);
			for (u_int i = 0; i < size; ++i)
				CPPUNIT_ASSERT(bindata[i] == r_buffer[i]);
		}

		void raw_put_get_3(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			char empty[1];
			int ret = mc_pack_put_raw(pp, "emptyraw", empty, 0);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);

			u_int siz = 0;
			const void *r = mc_pack_get_raw(pp, "emptyraw", &siz);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(r));
			CPPUNIT_ASSERT(0 == ret);
		}

		void traverse_1(void) {
			const char *name[4] = {"username", "Spaceurl", "SpaceName", "SPACEDesc"};
			const char *value[4] = {"zhouzerui", "joey", "Joey", "How are you doing?"};
			const int value_int[4] = {255, 65536, 2000000000, -2000000000};
			const char *name_int[4] = {"0", "1", "2", "3"};

			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(pp, name[0], value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(pp, name[1], value[1]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_str(ap, NULL, value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[1]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[2]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, name[2], value_int[2]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_int32(op, name[3], value_int[3]);
			CPPUNIT_ASSERT(0 == ret);

			int res_int = 0;
			ret = mc_pack_get_int32(op, name[2], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[2]);
			ret = mc_pack_get_int32(op, name[3], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[3]);

			//fwrite(pp, mc_pack_get_size(pp), 1, stderr);

			int i = 0;
			int count = 0;
			mc_pack_item_t item;

		  /* traverse pack       */
			count = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(count == 4);
			for (i = 0; i < count + 1; ++i) {
				ret = mc_pack_get_by_idx(pp, i, &item);
				if (i < 2) {
					//fprintf(stderr, "DEBUG: value:%s item.value:%s\n", value[i], item.value);
					//fprintf(stderr, "DEBUG: name:%d value:%d\n", item.key, item.value);
					CPPUNIT_ASSERT(0 == ret);
					CPPUNIT_ASSERT(item.type == MC_IT_TXT);
					CPPUNIT_ASSERT(item.value_size == strlen(value[i]) + 1);
					CPPUNIT_ASSERT(strcmp(name[i], item.key) == 0);
					CPPUNIT_ASSERT(strcmp(value[i], item.value) == 0);
				}
				if (2 == i) {
					CPPUNIT_ASSERT(0 == ret);
					CPPUNIT_ASSERT(item.type == MC_PT_ARR);
					CPPUNIT_ASSERT(item.value_size == 3);
					CPPUNIT_ASSERT(strcmp(ARR_NAME, item.key) == 0);
					CPPUNIT_ASSERT(item.value == (char *)ap);
				}
				if (3 == i) {
					CPPUNIT_ASSERT(0 == ret);
					CPPUNIT_ASSERT(item.type == MC_PT_OBJ);
					CPPUNIT_ASSERT(item.value_size == 2);
					CPPUNIT_ASSERT(strcmp(OBJ_NAME, item.key) == 0);
					CPPUNIT_ASSERT(item.value == (char *)op);
				}
				if (4 == i) {
					//fprintf(stderr, "DEBUG: i=%d ret=%d\n", i, ret);
					CPPUNIT_ASSERT(0 != ret);
				}
			}

		  /* traverse array       */
			count = mc_pack_get_item_count(ap);
			CPPUNIT_ASSERT(count == 3);
			for (i = 0; i < count; ++i) {
				ret = mc_pack_get_by_idx(ap, i, &item);
				CPPUNIT_ASSERT(0 == ret);
				CPPUNIT_ASSERT(item.type == MC_IT_TXT);
				CPPUNIT_ASSERT(item.value_size == strlen(value[i]) + 1);
				char tmp[32];
				snprintf(tmp, 32, "mc_array:%s", name_int[i]);
				CPPUNIT_ASSERT(strcmp(tmp, item.key) == 0);
				CPPUNIT_ASSERT(strcmp(value[i], item.value) == 0);
			}

		  /* traverse object       */
			count = mc_pack_get_item_count(op);
			CPPUNIT_ASSERT(count == 2);
			for (i = 0; i < count; ++i) {
				ret = mc_pack_get_by_idx(op, i, &item);
				CPPUNIT_ASSERT(0 == ret);
				CPPUNIT_ASSERT(item.type == MC_IT_I32);
				CPPUNIT_ASSERT(item.value_size == sizeof(int));
				char tmp[32];
				snprintf(tmp, 32, "mc_object,%s", name[i + 2]);
				CPPUNIT_ASSERT(strcmp(tmp, item.key) == 0);
				CPPUNIT_ASSERT(value_int[i + 2] == *(int *)item.value);
			}

		}

		void traverse_2(void) {		  /* find first, next       */
			const char *name[4] = {"username", "Spaceurl", "SpaceName", "SPACEDesc"};
			const char *value[4] = {"zhouzerui", "joey", "Joey", "How are you doing?"};
			const int value_int[4] = {255, 65536, 2000000000, -2000000000};

			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = -1;
			//ret = mc_pack_put_str(pp, name[0], value[0]);
			//CPPUNIT_ASSERT(0 == ret);
			//ret = mc_pack_put_str(pp, name[1], value[1]);
			//CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_str(ap, NULL, value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[1]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[2]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, name[2], value_int[2]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_int32(op, name[3], value_int[3]);
			CPPUNIT_ASSERT(0 == ret);

			int res_int = 0;
			ret = mc_pack_get_int32(op, name[2], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[2]);
			ret = mc_pack_get_int32(op, name[3], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[3]);

			//fwrite(pp, mc_pack_get_size(pp), 1, stderr);

		  /* begin find       */
			mc_pack_item_t item;
			ret = mc_pack_first_item(pp, &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_PT_ARR == item.type);
			CPPUNIT_ASSERT(3 == item.value_size);
			CPPUNIT_ASSERT(strcmp(ARR_NAME, item.key) == 0);
			CPPUNIT_ASSERT((char *)ap == item.value);

			ret = mc_pack_next_item(&item, &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_PT_OBJ == item.type);
			CPPUNIT_ASSERT(2 == item.value_size);
			CPPUNIT_ASSERT(strcmp(OBJ_NAME, item.key) == 0);
			CPPUNIT_ASSERT((char *)op == item.value);

			ret = mc_pack_first_item(ap, &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_IT_TXT == item.type);
			CPPUNIT_ASSERT(strlen(value[0]) + 1 == item.value_size);
			CPPUNIT_ASSERT(strcmp("mc_array:0", item.key) == 0);
			CPPUNIT_ASSERT(strcmp(value[0], item.value) == 0);

			ret = mc_pack_next_item(&item, &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_IT_TXT == item.type);
			CPPUNIT_ASSERT(strlen(value[1]) + 1 == item.value_size);
			CPPUNIT_ASSERT(strcmp("mc_array:1", item.key) == 0);
			CPPUNIT_ASSERT(strcmp(value[1], item.value) == 0);

			ret = mc_pack_first_item(op, &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_IT_I32 == item.type);
			CPPUNIT_ASSERT(sizeof(int) == item.value_size);
			CPPUNIT_ASSERT(strcmp("mc_object,SpaceName", item.key) == 0);
			CPPUNIT_ASSERT(value_int[2] == *(int *)item.value);

			ret = mc_pack_next_item(&item, &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_IT_I32 == item.type);
			CPPUNIT_ASSERT(sizeof(int) == item.value_size);
			CPPUNIT_ASSERT(strcmp("mc_object,SPACEDesc", item.key) == 0);
			CPPUNIT_ASSERT(value_int[3] == *(int *)item.value);

		}

		void pack_close_1(void) {
			const char *name[4] = {"username", "Spaceurl", "SpaceName", "SPACEDesc"};
			const char *value[4] = {"zhouzerui", "joey", "Joey", "How are you doing?"};
			const int value_int[4] = {255, 65536, 2000000000, -2000000000};

			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(pp, name[0], value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(pp, name[1], value[1]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_str(ap, NULL, value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[1]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[2]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, name[2], value_int[2]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_int32(op, name[3], value_int[3]);
			CPPUNIT_ASSERT(0 == ret);

			int res_int = 0;
			ret = mc_pack_get_int32(op, name[2], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[2]);
			ret = mc_pack_get_int32(op, name[3], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[3]);

			ret = mc_pack_close(op);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == ret);
			ret = mc_pack_close(ap);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == ret);
			ret = mc_pack_close(NULL);
			CPPUNIT_ASSERT(MC_PE_BAD_PARAM == ret);
			ret = mc_pack_close(pp);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);
			CPPUNIT_ASSERT(0 == pp->bytes_left);
		}

		void traverse_3(void) {		  /* empty find first       */
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			int ret = -1;
			mc_pack_item_t item;
			ret = mc_pack_first_item(ap, &item);
			CPPUNIT_ASSERT(ret == MC_PE_NOT_FOUND);
			ret = mc_pack_first_item(op, &item);
			CPPUNIT_ASSERT(ret == MC_PE_NOT_FOUND);

		}

		void pack_reopen_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(2 == ret);

			ret = mc_pack_close(pp);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_int32(ap, NULL, 12345);
			CPPUNIT_ASSERT(MC_PE_NO_SPACE == ret);

			ret = mc_pack_reopen(ap, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == ret);
			ret = mc_pack_reopen(op, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == ret);

			ret = mc_pack_reopen(pp, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_int32(ap, NULL, 12345);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_get_item_count(pp);
			CPPUNIT_ASSERT(2 == ret);
			ret = mc_pack_get_item_count(ap);
			CPPUNIT_ASSERT(1 == ret);
		}

		void perror_1(void) {
			for (int i = 0; i >= -255; --i) {
				//fprintf(stderr, "%s\n", mc_pack_perror(i));
			}
		}
		void ptype_1(void) {
			for (int i = 0; i <= 255; ++i) {
				//fprintf(stderr, "%s\n", mc_pack_ptype(i));
			}
		}

		void dead_cycle_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			int ret = mc_pack_put_str(ap, NULL, "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "string2");
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_item_t item;
			mc_pack_item_t item1;
			ret = mc_pack_first_item(pp, &item);
			CPPUNIT_ASSERT(0 == ret);
			while (0 == ret) {
				if (MC_PT_ARR == item.type) {
					ret = mc_pack_first_item((mc_pack_t *)item.value, &item1);
					while (0 == ret) {
						//fprintf(stderr, "DEBUG: %s -> %s\n", item1.key, item1.value);
						ret = mc_pack_next_item(&item1, &item1);
					}
				}
				ret = mc_pack_next_item(&item, &item);
			}
		}

		void pack_copy_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, "goodday", 7654321);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_str(ap, NULL, "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, "string2");
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_str(op, "username", "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(op, "USERID", "string2");
			CPPUNIT_ASSERT(0 == ret);

			char buf[DEF_PACK_SIZE] = {0};
			mc_pack_t *pp1 = mc_pack_create(buf, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(mc_pack_valid(pp1, DEF_PACK_SIZE));

			ret = mc_pack_copy_item(pp, pp1, "goodday");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_copy_item(pp, pp1, ARR_NAME);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_copy_item(pp, pp1, OBJ_NAME);
			CPPUNIT_ASSERT(0 == ret);

			CPPUNIT_ASSERT(mc_pack_get_size(pp) == mc_pack_get_size(pp1));
			CPPUNIT_ASSERT(0 == memcmp(pp, pp1, mc_pack_get_size(pp)));
		}

		void pack_copy_2(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, "goodday", 7654321);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_str(op, "username", "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(op, "USERID", "string2");
			CPPUNIT_ASSERT(0 == ret);

			char buf[DEF_PACK_SIZE];
			mc_pack_t *pp1 = mc_pack_create(buf, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(mc_pack_valid(pp1, DEF_PACK_SIZE));

			mc_pack_t *op1 = mc_pack_put_object(pp1, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op1) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op1, DEF_PACK_SIZE));

			ret = mc_pack_copy_item(op, op1, "USERID");
			CPPUNIT_ASSERT(0 == ret);

			CPPUNIT_ASSERT(0 == strcmp("string2", mc_pack_get_str(op1, "USERID")));
		}

		void pack_copy_3(void) { // 2D+ array
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, "goodday", 7654321);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *sub = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(sub));
			ret = mc_pack_put_str(sub, NULL, "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string2");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string3");
			CPPUNIT_ASSERT(0 == ret);

			sub = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(sub));
			ret = mc_pack_put_str(sub, NULL, "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string2");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string3");
			CPPUNIT_ASSERT(0 == ret);

			char buf[DEF_PACK_SIZE] = {0};
			mc_pack_t *pp1 = mc_pack_create(buf, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(mc_pack_valid(pp1, DEF_PACK_SIZE));

			ret = mc_pack_copy_item(pp, pp1, "goodday");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_copy_item(pp, pp1, ARR_NAME);
			CPPUNIT_ASSERT(0 == ret);

			CPPUNIT_ASSERT(mc_pack_get_size(pp) == mc_pack_get_size(pp1));
			CPPUNIT_ASSERT(0 == memcmp(pp, pp1, mc_pack_get_size(pp)));
		}

		void pack_copy_4(void) { // copy_item_arr
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_int32(pp, "goodday", 7654321);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			mc_pack_t *sub = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(sub));
			ret = mc_pack_put_str(sub, NULL, "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string2");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string3");
			CPPUNIT_ASSERT(0 == ret);

			sub = mc_pack_put_array(ap, NULL);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(sub));
			ret = mc_pack_put_str(sub, NULL, "string1");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string2");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(sub, NULL, "string3");
			CPPUNIT_ASSERT(0 == ret);

			char buf[DEF_PACK_SIZE] = {0};
			mc_pack_t *pp1 = mc_pack_create(buf, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(mc_pack_valid(pp1, DEF_PACK_SIZE));

			ret = mc_pack_copy_item(pp, pp1, "goodday");
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap1 = mc_pack_put_array(pp1, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap1) == 0);
			ret = mc_pack_copy_item_arr(ap, ap1, 0);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_copy_item_arr(ap, ap1, 1);
			CPPUNIT_ASSERT(0 == ret);

			CPPUNIT_ASSERT(mc_pack_get_size(pp) == mc_pack_get_size(pp1));
			CPPUNIT_ASSERT(0 == memcmp(pp, pp1, mc_pack_get_size(pp)));
		}

		void cmp_str_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_str(pp, "goodday", "Hello World!");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_cmp_str(pp, "goodday", "Hello World!");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_cmp_str(pp, "goodday", "hello world!");
			CPPUNIT_ASSERT(1 == ret);
			ret = mc_pack_cmp_str(pp, "goodda", "Hello World!");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == ret);
		}

		void cmp_str_2(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_str(pp, "goodday", "Hello World!");
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_casecmp_str(pp, "goodday", "Hello World!");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_casecmp_str(pp, "goodday", "hello world!");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_casecmp_str(pp, "goodda", "Hello World!");
			CPPUNIT_ASSERT(MC_PE_NOT_FOUND == ret);
		}

		void put_nstr_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_nstr(pp, "goodday", "Hello World!", 5);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_cmp_str(pp, "goodday", "Hello");
			CPPUNIT_ASSERT(0 == ret);
		}

		void putf_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int arr[5] = {1, 2, 3, 4, 5};
			char raw[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

			int ret = mc_pack_putf(pp, "str:%s,int32:%d,uint32:%u,int64:%lld,uint64:%llu,arr:%p%c%u,raw:%p%u",
					"abc", -1, 2, -3ll, 4llu, arr, 'd', 5, raw, 8);
			CPPUNIT_ASSERT(5 == ret);

			ret = mc_pack_putf(pp, "%s:%d,%s:%d",
					"Name1", 1, "name2", 2);
			CPPUNIT_ASSERT(2 == ret);

			ret = mc_pack_cmp_str(pp, "str", "abc");
			CPPUNIT_ASSERT(0 == ret);

			int value = 0;
			ret = mc_pack_get_int32(pp, "int32", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-1 == value);
			ret = mc_pack_get_int32(pp, "Name1", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1 == value);
			ret = mc_pack_get_int32(pp, "name2", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(2 == value);
		}

		void putf_2(void) { // add raw
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			char raw[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

			int ret = mc_pack_putf(pp, "rawdata:%p%u", raw, 8);
			CPPUNIT_ASSERT(1 == ret);

			const void *praw = NULL;
			u_int raw_len = 0;

			praw = mc_pack_get_raw(pp, "rawdata", &raw_len);
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(praw));
			CPPUNIT_ASSERT(sizeof(raw) == raw_len);
			CPPUNIT_ASSERT(0 == memcmp(praw, raw, sizeof(raw)));
		}

		void getf_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int arr[5] = {1, 2, 3, 4, 5};
			char raw[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

			int ret = mc_pack_putf(pp, "str:%s,int32:%d,uint32:%u,int64:%lld,uint64:%llu,arr:%pd%u,raw:%p%u",
					"abc", -1, 2, -3ll, 4llu, arr, 5, raw, 8);
			CPPUNIT_ASSERT(5 == ret);

			ret = mc_pack_putf(pp, "%s:%d,%s:%d",
					"Name1", 1, "name2", 2);
			CPPUNIT_ASSERT(2 == ret);

			ret = mc_pack_cmp_str(pp, "str", "abc");
			CPPUNIT_ASSERT(0 == ret);

			int value[8];
			char raw_value[8];
			u_int arr_cnt = 5;
			u_int raw_cnt = 8;
			char *s = NULL;
			int *parr = value + 3;
			char *praw = raw_value;
			ret = mc_pack_getf(pp, "int32:%d,Name1:%d,name2:%d,str:%p,arr:%pd%u,raw:%p%u",
					value, value + 1, value + 2, &s, &parr, &arr_cnt, &praw, &raw_cnt);
			CPPUNIT_ASSERT(4 == ret);
			CPPUNIT_ASSERT(-1 == value[0]);
			CPPUNIT_ASSERT(1 == value[1]);
			CPPUNIT_ASSERT(2 == value[2]);
			ret = mc_pack_cmp_str(pp, "str", s);
			CPPUNIT_ASSERT(0 == ret);
		}

		void getf_2(void) { // add raw
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			char raw[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

			int ret = mc_pack_putf(pp, "rawdata:%p%u", raw, 8);
			CPPUNIT_ASSERT(1 == ret);

			char *praw = NULL;
			u_int raw_len = 0;

			ret = mc_pack_getf(pp, "rawdata:%p%u", &praw, &raw_len);
			CPPUNIT_ASSERT(1 == ret);
			CPPUNIT_ASSERT(sizeof(raw) == raw_len);
			CPPUNIT_ASSERT(0 == memcmp(praw, raw, sizeof(raw)));
		}

		void pack2str_1(void) {
			const char *name[4] = {"username", "Spaceurl", "SpaceName", "SPACEDesc"};
			const char *value[4] = {"zhouzerui", "joey", "Joey", "How are you doing?"};
			const int value_int[4] = {255, 65536, 2000000000, -2000000000};

			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = -1;
			ret = mc_pack_put_str(pp, name[0], value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(pp, name[1], value[1]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *ap = mc_pack_put_array(pp, ARR_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(ap) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(ap, DEF_PACK_SIZE));

			ret = mc_pack_put_str(ap, NULL, value[0]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[1]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_str(ap, NULL, value[2]);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *op = mc_pack_put_object(pp, OBJ_NAME);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(op) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(op, DEF_PACK_SIZE));

			ret = mc_pack_put_int32(op, name[2], value_int[2]);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_int32(op, name[3], value_int[3]);
			CPPUNIT_ASSERT(0 == ret);

			int res_int = 0;
			ret = mc_pack_get_int32(op, name[2], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[2]);
			ret = mc_pack_get_int32(op, name[3], &res_int);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(res_int == value_int[3]);

			char bin[] = "长江七号不好看";
			ret = mc_pack_put_raw(pp, "rawdata", bin, sizeof(bin));
			CPPUNIT_ASSERT(0 == ret);

			char out_buf[DEF_PACK_SIZE];
			ret = mc_pack_pack2str(pp, out_buf, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(0 == ret);
			//fprintf(stderr, "DEBUG: %s", out_buf);
		}

		void get_item_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int ret = mc_pack_put_str(pp, "goodday", "Hello World!");
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_item_t item;
			ret = mc_pack_get_item(pp, "goodday", &item);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(MC_IT_TXT == item.type);
			CPPUNIT_ASSERT(13 == item.value_size);
			CPPUNIT_ASSERT(0 == strcmp(item.key, "goodday"));
			CPPUNIT_ASSERT(0 == strcmp(item.value, "Hello World!"));
		}

		void pack2str_2(void) {
			FILE *fp = fopen("123", "r");
			char out_buf[640];
			mc_pack_t *pp = NULL;
			int ret;

			if (fp) {
				ret = fread(buffer, 1, DEF_PACK_SIZE, fp);
				pp = (mc_pack_t *)buffer;

				ret = mc_pack_pack2str(pp, out_buf, sizeof(out_buf));
				fprintf(stderr, "DEBUG: %s", out_buf);
				fprintf(stderr, "DEBUG: ret=%d", ret);
				CPPUNIT_ASSERT(0 == ret);

				fclose(fp);
			}

			fp = fopen("bk.suite", "r");
			if (fp) {
				ret = fread(out_buf, 1, sizeof(out_buf), fp);
				out_buf[ret] = 0;

				mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
				CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
				CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

				ret = mc_pack_text2pack(out_buf, pp);
				CPPUNIT_ASSERT(0 == ret);
				fclose(fp);
			}

			fp = fopen("creq_pack", "r");
			if (fp) {
				ret = fread(buffer, 1, sizeof(buffer), fp);
				buffer[ret] = 0;

				mc_pack_t *pp = (mc_pack_t *)buffer;
				CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
				CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

				ret = mc_pack_pack2text(pp, out_buf, sizeof(out_buf), 0);
				CPPUNIT_ASSERT(MC_PE_NO_SPACE == ret);
				fclose(fp);
			}
		}

		void reset_1(void) {
			mc_pack_t *pp = mc_pack_create(buffer, DEF_PACK_SIZE);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);
			CPPUNIT_ASSERT(mc_pack_valid(pp, DEF_PACK_SIZE));

			int old_size = 0;
			int ret = mc_pack_putf(pp, "str:%s,int32:%d,uint32:%u,int64:%lld,uint64:%llu",
					"abc", -1, 2, -3ll, 4llu);
			CPPUNIT_ASSERT(5 == ret);

			ret = mc_pack_putf(pp, "%s:%d,%s:%d",
					"Name1", 1, "name2", 2);
			CPPUNIT_ASSERT(2 == ret);

			old_size = mc_pack_get_size(pp);

			ret = mc_pack_reset(pp);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(0 == pp->item_count);
			CPPUNIT_ASSERT(0 == pp->internal_item_count);
			CPPUNIT_ASSERT(sizeof(mc_pack_t) == pp->cur_pos);
			CPPUNIT_ASSERT(DEF_PACK_SIZE - sizeof(mc_pack_t) == pp->bytes_left);

			ret = mc_pack_putf(pp, "str:%s,int32:%d,uint32:%u,int64:%lld,uint64:%llu",
					"abc", -1, 2, -3ll, 4llu);
			CPPUNIT_ASSERT(5 == ret);

			ret = mc_pack_putf(pp, "%s:%d,%s:%d",
					"Name1", 1, "name2", 2);
			CPPUNIT_ASSERT(2 == ret);

			ret = mc_pack_get_size(pp);
			CPPUNIT_ASSERT(old_size == ret);
		}

		void creatf_1(void) {
			mc_pack_t *pp = NULL;
			pp = mc_pack_creatf(buffer, sizeof(buffer), "str:%s,int32:%d,uint32:%u,int64:%lld,uint64:%llu",
					"abc", -1, 2, -3ll, 4llu);
			CPPUNIT_ASSERT(MC_PACK_PTR_ERR(pp) == 0);

			int ret = 0;
			int value = 0;
			ret = mc_pack_cmp_str(pp, "str", "abc");
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_get_int32(pp, "int32", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-1 == value);
			ret = mc_pack_get_int32(pp, "uint32", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(2 == value);
			ret = mc_pack_get_int32(pp, "int64", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(-3 == value);
			ret = mc_pack_get_int32(pp, "uint64", &value);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(4 == value);
		}

		void text2pack_1(void) {
			char *txt = "{cmd:send, uid:(uint32)10000,msg:(object){title:hello,body:world},int_array:[(int32)-1, (int32)0, (int32)1], rawdat:(raw)01000a0b, rawfile:(rawfile)./rawfile, units:[{content:江泽民, type:title, is_html:0}, {content:\"<a href=法轮功>\\\"bug\\\",</a>\", type:blog, is_html:1}, {content:hello, type:hello, is_html:0}]}";
			//char *txt = "{1:abc, cmd : send, uid:(uint32)10000,msg:(object){title:hello,body:world},int_array:[(int32)-1, (int32)0, (int32)1], rawdat:(raw)01000a0b, units:[{content:江泽民, type:title, is_html:0}, {content:\"<a href=法轮功>\\\"bug\\\",</a>\", type:blog, is_html:1}, {content:hello, type:hello, is_html:0}]}";
			//char *txt = "{cmd:\"save_offline_msg\", type:(uint32)0, uid:(uint32)61927614, body:{ to_uid:(uint64)61927614, type:(uint32)0, time:(uint64)1206533639, msg_id:(uint64)12345, msg_sid:(uint64)0, data:(rawfile)./1.xml } }";

			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret = mc_pack_text2pack(txt, pack);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);

			char out_buf[DEF_PACK_SIZE];
			mc_pack_pack2text(pack, out_buf, sizeof(out_buf), 0);
			//fprintf(stderr, "DEBUG: %s\n", out_buf);

			char buffer2[DEF_PACK_SIZE];
			mc_pack_t *pack2 = mc_pack_create(buffer2, sizeof(buffer2));
			CPPUNIT_ASSERT(mc_pack_valid(pack2, DEF_PACK_SIZE));

			ret = mc_pack_text2pack(out_buf, pack2);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);

			CPPUNIT_ASSERT(mc_pack_get_size(pack) == mc_pack_get_size(pack2));
			CPPUNIT_ASSERT(0 == memcmp(pack, pack2, mc_pack_get_size(pack)));
		}

		void text2pack_2(void) { // json
			char *txt = "{cmd:send, uid:(uint32)10000,msg:(object){title:hello,body:world},int_array:[(int32)-1, (int32)0, (int32)1], units:[{content:江泽民, type:title, is_html:0}, {content:\"<a href=法轮功>\\\"bug\\\",</a>\", type:blog, is_html:1}, {content:hello, type:hello, is_html:0}]}";

			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret = mc_pack_text2pack(txt, pack);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);

			char out_buf[DEF_PACK_SIZE];
			mc_pack_pack2text(pack, out_buf, sizeof(out_buf), 1);
			//fprintf(stderr, "DEBUG: %s\n", out_buf);

			char buffer2[DEF_PACK_SIZE];
			mc_pack_t *pack2 = mc_pack_create(buffer2, sizeof(buffer2));
			CPPUNIT_ASSERT(mc_pack_valid(pack2, DEF_PACK_SIZE));

			ret = mc_pack_text2pack(out_buf, pack2);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);

			char out_buf2[DEF_PACK_SIZE];
			mc_pack_pack2text(pack, out_buf2, sizeof(out_buf2), 1);
			CPPUNIT_ASSERT(0 == strcmp(out_buf, out_buf2));
		}

		void text2pack_3(void) {
			char *txt_bad = "{ cmd : send, msg : hello world, id : (int32)101010}";
			char *txt_good = "{ cmd : send, msg : \"hello world\", id : (int32)101010}";

			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret = mc_pack_text2pack(txt_bad, pack);
			CPPUNIT_ASSERT(ret > 0);

			mc_pack_reset(pack);
			ret = mc_pack_text2pack(txt_good, pack);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);

			char out_buf[DEF_PACK_SIZE];
			ret = mc_pack_pack2text(pack, out_buf, sizeof(out_buf), 0);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);
			CPPUNIT_ASSERT(0 == strcmp(out_buf, "{\"cmd\":\"send\", \"msg\":\"hello world\", \"id\":(int32)101010}"));
			ret = mc_pack_pack2text(pack, out_buf, sizeof(out_buf), 1);
			CPPUNIT_ASSERT(MC_PE_SUCCESS == ret);
			CPPUNIT_ASSERT(0 == strcmp(out_buf, "{\"cmd\":\"send\", \"msg\":\"hello world\", \"id\":101010}"));
		}

		void text2pack_4(void) {
			char *txt = "{cmd:(abc)abc}";
			char *txt1 = "{cmd:haha}{cmd:ok}";
			char *txt2 = "{cmd:haha}";

			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret = mc_pack_text2pack(txt, pack);
			CPPUNIT_ASSERT(6 == ret);

			mc_pack_reset(pack);
			ret = mc_pack_text2pack(txt1, pack);
			CPPUNIT_ASSERT(6 == ret);

			mc_pack_close(pack);
			ret = mc_pack_text2pack(txt2, pack);
			CPPUNIT_ASSERT(MC_PE_NO_SPACE == ret);

		}

		void text2pack_5(void) {
			char *txt = "{raw:(raw)}";
			char *txt1 = "{raw:(raw), raw1:(raw)1223}";
			char *txt2 = "{rawarr:[(raw), (raw), (raw)]}";
			char o1[64];
			char o2[64];
			int ret;

			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			ret = mc_pack_text2pack(txt, pack);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_pack2text(pack, o1, sizeof(o1), 0);
			CPPUNIT_ASSERT(0 == ret);
			mc_pack_reset(pack);
			ret = mc_pack_text2pack(o1, pack);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_pack2text(pack, o2, sizeof(o2), 0);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(0 == strcmp(o1, o2));

			mc_pack_reset(pack);
			ret = mc_pack_text2pack(txt1, pack);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_pack2text(pack, o1, sizeof(o1), 0);
			CPPUNIT_ASSERT(0 == ret);
			mc_pack_reset(pack);
			ret = mc_pack_text2pack(o1, pack);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_pack2text(pack, o2, sizeof(o2), 0);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(0 == strcmp(o1, o2));

			mc_pack_reset(pack);
			ret = mc_pack_text2pack(txt2, pack);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_pack2text(pack, o1, sizeof(o1), 0);
			CPPUNIT_ASSERT(0 == ret);
			mc_pack_reset(pack);
			ret = mc_pack_text2pack(o1, pack);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_pack2text(pack, o2, sizeof(o2), 0);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(0 == strcmp(o1, o2));
		}

		static int foreach_cb_1(const mc_pack_item_t *pitem, void *arg) {
			return 2;
		}
		static int foreach_cb_2(const mc_pack_item_t *pitem, void *arg) {
			return -3;
		}
		static int foreach_cb_3(const mc_pack_item_t *pitem, void *arg) {
			return 0;
		}
		static int foreach_cb_4(const mc_pack_item_t *pitem, void *arg) {
			//fprintf(stderr, "DEBUG: key[%s]\n", pitem->key);
			//fprintf(stderr, "DEBUG: arg[%s]\n", (char *)arg);
			return 1;
		}

		void foreach_1(void) {
			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret = mc_pack_foreach(pack, NULL, NULL);
			CPPUNIT_ASSERT(MC_PE_BAD_PARAM == ret);

			ret = mc_pack_foreach((mc_pack_t *)&ret, foreach_cb_1, NULL);
			CPPUNIT_ASSERT(MC_PE_BAD_TYPE == ret);

			ret = mc_pack_text2pack("{hh:{a:haha, b:hehe, c:hoho}, aa:[1,2,3,4], ab:cd, integer:(int32)123}", pack);
			CPPUNIT_ASSERT(0 == ret);

			int cnt = pack->internal_item_count;
			ret = mc_pack_foreach(pack, foreach_cb_1, NULL);
			CPPUNIT_ASSERT(cnt * 2 == ret);
			ret = mc_pack_foreach(pack, foreach_cb_2, NULL);
			CPPUNIT_ASSERT(-3 == ret);
			ret = mc_pack_foreach(pack, foreach_cb_3, NULL);
			CPPUNIT_ASSERT(0 == ret);
			char *arg = "this is argument";
			ret = mc_pack_foreach(pack, foreach_cb_4, arg);
			CPPUNIT_ASSERT(cnt == ret);
		}

		void get_from_item_1(void) {
			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret;
			ret = mc_pack_put_int32(pack, "i32", 1);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_uint32(pack, "u32", 1);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_int64(pack, "i64", 1);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_uint64(pack, "u64", 1);
			CPPUNIT_ASSERT(0 == ret);

			int i32 = 0;
			u_int u32 = 0;
			mc_int64_t i64 = 0;
			mc_uint64_t u64 = 0;
			mc_pack_item_t item;
			ret = mc_pack_get_item(pack, "i32", &item);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_get_int32_from_item(&item, &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1 == i32);
			ret = mc_pack_get_item(pack, "u32", &item);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_get_uint32_from_item(&item, &u32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1 == i32);
			ret = mc_pack_get_item(pack, "i64", &item);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_get_int64_from_item(&item, &i64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1 == i64);
			ret = mc_pack_get_item(pack, "u64", &item);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_get_uint64_from_item(&item, &u64);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1 == u64);
		}

		void put_item_1(void) {
			mc_pack_t *pack = mc_pack_create(buffer, sizeof(buffer));
			CPPUNIT_ASSERT(mc_pack_valid(pack, DEF_PACK_SIZE));

			int ret = 0;
			ret = mc_pack_put_int32(pack, "i32", 1);
			CPPUNIT_ASSERT(0 == ret);

			mc_pack_t *sub = mc_pack_put_array(pack, "arr");
			CPPUNIT_ASSERT(0 == MC_PACK_PTR_ERR(sub));
			ret = mc_pack_put_int32(sub, NULL, 2);
			CPPUNIT_ASSERT(0 == ret);

			char buf[DEF_PACK_SIZE];
			mc_pack_t *pack1 = mc_pack_create(buf, sizeof(buf));

			mc_pack_item_t item;
			ret = mc_pack_first_item(pack, &item);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_put_item(pack1, &item);
			CPPUNIT_ASSERT(0 == ret);

			int i32 = 0;
			ret = mc_pack_get_int32(pack1, "i32", &i32);
			CPPUNIT_ASSERT(0 == ret);
			CPPUNIT_ASSERT(1 == i32);

			ret = mc_pack_next_item(&item, &item);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_item(pack1, &item);
			CPPUNIT_ASSERT(0 == ret);

			ret = mc_pack_first_item(sub, &item);
			CPPUNIT_ASSERT(0 == ret);
			ret = mc_pack_put_item(pack1, &item);
			CPPUNIT_ASSERT(0 == ret);

			item.key = mc_pack_get_subkey(item.key);
			ret = mc_pack_put_item(pack1, &item);
			CPPUNIT_ASSERT(0 != ret);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(mytest);

int main(void)
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

	runner.addTest( registry.makeTest() );
	runner.run();

	return 0;
}


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
