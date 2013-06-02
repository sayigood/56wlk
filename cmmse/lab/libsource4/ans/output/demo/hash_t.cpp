#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define _THREAD_SAFE
#include "hash_map.h"
#include <iostream>
using namespace std;


#define BASE (1024*1024)

unsigned int fun( const int & key )
{
	return key%BASE;
}



template<class key_t, class value_t>
void list( ans::hash_map<key_t,value_t> &hash, int flag=0 )
{
	cout<<"walk"<<endl;
#ifdef _HASH_MAP_TEST
	hash.walk();
#endif
	cout<<"block walk"<<endl;
#ifdef _HASH_MAP_TEST
	hash.walk_block(flag);
#endif
	cout<<"free walk"<<endl;
#ifdef _HASH_MAP_TEST
	hash.walk_free();
#endif

	cout<<"end"<<endl<<endl;
}


void test1()
{
	cout<<"start test1"<<endl;
	ans::hash_map<int,int> hash;
	hash.create( BASE, fun );
	
	int xcount = 0;
	for( int i=0; i<BASE*3; i++ ){
		if( hash.set( i, i ) == 0 )
			xcount++;
	}
	cout<<xcount<<endl;
	cout<<hash.size()<<endl;
#ifdef _HASH_MAP_TEST
	hash.walk_block();
#endif
	
	xcount = 0;
	for( int i=0; i<BASE*3; i++ ){
		if( hash.erase( i ) == 0 )
			xcount++;
	}
	cout<<xcount<<endl;
	cout<<hash.size()<<endl;
#ifdef _HASH_MAP_TEST
	hash.walk_block();
#endif
	xcount = 0;	
	for( int i=0; i<BASE*4; i++ ){
		if( hash.set( i, i ) == 0 )
			xcount++;
	}
	cout<<xcount<<endl;
	cout<<hash.size()<<endl;
#ifdef _HASH_MAP_TEST
	hash.walk_block();
#endif
	hash.savedata( "hello" );

	ans::hash_map<int,int> hash2;
	hash2.create( BASE,fun );
	hash2.loaddata("hello");
	cout<<hash2.size()<<endl;
#ifdef _HASH_MAP_TEST
	hash2.walk_block();
#endif
	hash2.savedata("world");
	cout<<"end test1"<<endl;
}

void test()
{
	cout<<"Rand_MAX "<<RAND_MAX<<endl;
	cout<<ans::HASH_SUCCESS<<" "
		<<ans::HASH_VAL_EXIST<<" "
		<<ans::HASH_VAL_NO_EXIST<<" "
		<<ans::HASH_ACLLOMEM_ERROR<<endl;
}


ans::hash_map<int,int> hash1;


void * test2_get( void * )
{
	cout<<"start read"<<endl;
	for(int i=0;i<BASE;i++)
		hash1.get(i,0);
	cout<<"end read"<<endl;
	return 0;
}

void * test2_set( void * )
{
	cout<<"start set"<<endl;
	for(int i=0;i<BASE;i++){
		hash1.set(i,i);
	}
	cout<<"end set"<<endl;
	return NULL;	
}

void * test2_set2( void * )
{
	cout<<"start set2"<<endl;
	for(int i=BASE;i<BASE*2;i++){
		hash1.set(i,i);
	}
	cout<<"end set"<<endl;
	return NULL;	
}

void * test2_erase( void * )
{
	cout<<"start erase"<<endl;
	for(int i=0;i<BASE;i++)
		hash1.erase(i);
	cout<<"end erase"<<endl;
	return NULL;
}


#define TEST2NUM 10
void test2()
{
	cout<<"start test2"<<endl;
	hash1.create(BASE,fun);
	pthread_t pget[TEST2NUM];
	pthread_t pset[TEST2NUM];
	
	for(int i=0;i<TEST2NUM;i++){
		pthread_create( &pget[i], 0, test2_get, 0);
		pthread_create( &pset[i], 0, test2_set2,0);
	}
	cout<<"start join"<<endl;
	for( int i=0;i<TEST2NUM;i++){
		pthread_join( pget[i], 0 );
		pthread_join( pset[i], 0 );
	}
	
	cout<<hash1.size()<<endl;
	hash1.savedata("h");
	cout<<"start to clean"<<endl;
	hash1.clear();
	cout<<"hash clean done"<<endl;
	cout<<"end test2"<<endl;
}

void test3()
{
	cout<<"start test3"<<endl;
	ans::hash_map<int,int> hash;
	hash.create( BASE/10, fun );
	hash.loaddata("h");
	cout<<hash.size()<<endl;
	int t=2;
	int val=rand();
	//list(hash);
	cout<<hash.get(val,&t)<<endl;
	cout<<t<<endl;
	hash.erase(val);
	t=0;
	cout<<hash.get(val,&t)<<endl;
	cout<<t<<endl;
	cout<<"end test3"<<endl;
}

//for 64 os
void test4()
{
	ans::hash_map<int, int> hash;
	int ret = hash.create(100000000, fun);
	if (ret != 0) {
		cout<<"create err"<<endl;
		return;
	}
	for (int i=0; i<100000000; ++i) {
		int ret = hash.set(i, i);
		cout<<"["<<i<<"]"<<"ret="<<ret<<endl;
	}
	hash.clear();
}

int main()
{
	//test();
	//test1();
	//test2();
	//test3();
	test4();
	return 0;
}
