#ifndef __MC_TYPE_H_
#define __MC_TYPE_H_

// structure defined here 

struct memory_cache_index_node
{
    // the 64-bit signature  
    unsigned int sign1;
    unsigned int sign2;
    // the the hash chain 
    unsigned int next;
    // the LRU link chain
    unsigned int left;
    unsigned int right;
};
typedef struct memory_cache_index_node mc_indexnode;

struct memory_cache_hashtable
{
    unsigned int hashnum;
    unsigned int * table;
};
typedef struct memory_cache_hashtable mc_hashtable;

struct memory_cache_cache
{
    unsigned int   capacity;    // the capacity of the cache
    unsigned int   unitsize;    // the size of cache unit 
    mc_hashtable   hashtable;   // the hash indexer
    mc_indexnode*  pnodelist;   // the index of the nodes 
    char*          pinfolist;   // the info cached
    unsigned int   rs_head;     // the head of reclaimed stack
    unsigned int   in_curpos;   // the first unused node
    unsigned int   totalcount;  // the node count in the cache
    unsigned int   head;        // the LRU bi-chain's head,the newest one
    unsigned int   tail;        // the LRU bi-chain's tail, the oldest one
};
typedef struct memory_cache_cache mc_cache;
/*
 * mc_modifyitem的最终返回值是传入的函数指针的返回值，
 * 注意这个函数不要返回mc_modifyitem的出错号等数值
 * 如果函数指针的返回值与mc_cache本身定义的有重复，
 * 外部调用就无法分清到底是哪个函数出了问题。
 */
typedef int(* modfunc )(void* pdst,int dstsize,void* opinfo);






#endif
