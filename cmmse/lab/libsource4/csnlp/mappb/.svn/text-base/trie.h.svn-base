#ifndef _TRIE_H_
#define _TRIE_H_

#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<assert.h>
using namespace std;

struct trie_node
{
    trie_node **chunk_nodes;//子节点数组，这个初始化时是多大？
    int child_size;//子节点个数
    int chunk_size;//hash大小
    trie_node *next;//下一个节点
    int preq;//频率
    char key;//字符
};

struct trie_tree
{
    trie_node root_node;
};

trie_node *init_trie_node();

trie_tree *init_trie_tree();

void free_trie_node(trie_node *node);

void free_trie_tree(trie_tree *tree);

int realloc_chunk_nodes(trie_node *node);

trie_node *add_node(trie_node *node, char key);

trie_node *add_word(trie_tree *tree, const char *word);

trie_node *find_word(trie_tree *tree, char *word);

//void process(trie_tree *tree, vector<string>, const char *query, vector<string> &v_addr);

void foreach_trie_node(trie_node *node, char *buffer, int buffer_offset);

void foreach_trie_tree(trie_tree *tree);

#endif
