/**
 * @file   demo.cpp
 * @brief  
 * 
 */

#include <iostream>
#include "compack/compack.h"
#include "compack/bsonparser.h"

using namespace std;
using namespace compack::buffer;
int test_1(){
    char buf[1024*1024];
    buffer bufwrap(buf,sizeof(buf));
    int ret1=-1;
    int ret2=-1;
    try{
        Object obj(&bufwrap);
        obj.putString("name","lover").putNum<int>("age",20);
        {
            //需要使用析构函数释放.
            Object obj2(&obj,"fuck");
            obj2.putString("name","zhangyan").putNum<int>("age",23);
            ret2=obj2.size();
            printf("inner pack size:%d\n",ret2);
        }
        ret1=obj.size();
        printf("outter pack size:%d\n",ret1);
    }catch(bsl::Exception e){
        cerr<<e.all()<<endl;
    }

    Reader reader(bufwrap);
    assert(ret1==(int)reader.size());
    
    compack::ObjectIterator it;
    assert(reader.find("fuck",it)==true);
    printf("offset:%u\n",it.offset());    
    Reader reader2(bufwrap,it);
    assert(ret2==(int)reader2.size());
    assert(reader2.find("age",it)==true);
    printf("offset:%u\n",it.offset());
    return 0;
}

int test_2(){
    //char * json = "{a	: 1, b : unhex	(\"11 22	ab\")}";
    char buf[1024*1024];
    char *json="{\"obj\":1}";
    buffer bufwrap(buf,sizeof(buf));
    BsonParser bp;
    bp.parse(json, &bufwrap);
    BsonFormatter bf;
    bf.format(bufwrap, &std::cout);
    std::cout<< std::endl;
    return 0;
}

int test_3(){
    //char * json = "{a	: 1, b : unhex	(\"11 22	ab\")}";
    char buf[1024*1024];
    char *json="{'obj':'1\\'\\''}";
    buffer bufwrap(buf,sizeof(buf));
    BsonParser bp;
    bp.parse(json, &bufwrap);
    BsonFormatter bf;
    bf.format(bufwrap, &std::cout);
    std::cout<< std::endl;
    return 0;
}

int main(){
    test_1();
    test_2();
    test_3();
    return 0;
}
