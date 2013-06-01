#include "trie.h"

trie_node *init_trie_node()
{
    trie_node *node = (trie_node *)malloc(sizeof(trie_node));
    if(!node)
    {
        fprintf(stderr,"Malloc trie node failed.\n");
		return NULL;
    }
    bzero(node, sizeof(trie_node));
    return node;
}

trie_tree *init_trie_tree()
{
    trie_tree *tree = (trie_tree *)malloc(sizeof(trie_tree));
    if(!tree)
    {
		fprintf(stderr,"Malloc trie node failed.\n");
		return NULL;
    }
    bzero(tree, sizeof(trie_tree));
    return tree;
}

void free_trie_node(trie_node *node)
{
    assert(node);

    trie_node *next_node;
    if(node->chunk_size)
    {
        for(int i = 0; i < node->chunk_size; ++i)
        {
            trie_node *cur_node = node->chunk_nodes[i];
            while(cur_node)
            {
                next_node = cur_node->next;
                free_trie_node(cur_node);
                cur_node = next_node;
            }
        }
        free(node->chunk_nodes);
    }

    free(node);
}

void free_trie_tree(trie_tree *tree)
{
    assert(tree);

    trie_node *node = &(tree->root_node);
    trie_node *next_node;
    if(node->chunk_size)
    {
        for(int i = 0; i < node->chunk_size; ++i)
        {
            trie_node *cur_node = node->chunk_nodes[i];
            while(cur_node)
            {
                next_node = cur_node->next;
                free_trie_node(cur_node);
                cur_node = next_node;
            }
        }
        free(node->chunk_nodes);
    }

    free(tree);
}

int realloc_chunk_nodes(trie_node *node)
{
    assert(node);

    trie_node **chunk_nodes;
    trie_node *cur_node;
    trie_node *next_node;
    int chunk_size;
    int offset;
    
    if(node->chunk_size)
    {
        chunk_size = (node->chunk_size) << 1;
    }
    else
    {
        chunk_size = 8;
    }
    chunk_nodes = (trie_node **)malloc(sizeof(trie_node **) * chunk_size);
    if(!chunk_nodes)
    {
		fprintf(stderr,"Realloc chunk_nodes failed.\n");
		return -1;
    }
    memset(chunk_nodes, 0, sizeof(trie_node **) * chunk_size);
    if(node->chunk_size)
    {
        for(int i = 0; i < node->chunk_size; ++i)
        {
            cur_node = node->chunk_nodes[i];
            while(cur_node)
            {
                offset = (unsigned char)cur_node->key % chunk_size;
                next_node = cur_node->next;
                cur_node->next = chunk_nodes[offset];
                chunk_nodes[offset] = cur_node;
                cur_node = next_node;
            }
        }
        free(node->chunk_nodes);
    }
    node->chunk_nodes = chunk_nodes;
    node->chunk_size = chunk_size;

	return 0;
}

trie_node *add_node(trie_node *node, char key)
{
    assert(node);

    int offset;
    trie_node *new_node = init_trie_node();
    new_node->key = key;
    if(node->chunk_size <= (node->child_size << 4 / 3))
    {
        int ret = realloc_chunk_nodes(node);
		if(ret<0)
		{
			fprintf(stderr,"realloc_chunk_nodes failed;\n");
			return NULL;
		}
    }
    offset = (unsigned char)key % node->chunk_size;
    new_node->next = node->chunk_nodes[offset];
    node->chunk_nodes[offset] = new_node;
    ++(node->child_size);
    return new_node;
}

trie_node *add_word(trie_tree *tree, const char *word)
{
    assert(tree);
    assert(word);

    trie_node *cur_node = &(tree->root_node);
    trie_node *chunk_node;
    int offset;
    while(*word)
    {
        if(!(cur_node->chunk_size))//这个一直是8?
        {
            break;
        }
        offset = (unsigned char)*word % cur_node->chunk_size;
        chunk_node = cur_node->chunk_nodes[offset];
        while(chunk_node)
        {
            if(chunk_node->key == *word)
            {
                break;
            }
            chunk_node = chunk_node->next;
        }
        if(chunk_node)
        {
            ++word;
            cur_node = chunk_node;
        }
        else
        {
            break;
        }
    }

    while(*word)
    {
        cur_node = add_node(cur_node, *word);
        ++word;
    }

    return cur_node;
}

trie_node *find_word(trie_tree *tree, char *word)
{
    assert(tree);
    assert(word);

    trie_node *cur_node = &(tree->root_node);
    trie_node *chunk_node;
    int offset;
    while(*word)
    {
        if(!(cur_node->chunk_size))
        {
            break;
        }
        offset = (unsigned char)*word % cur_node->chunk_size;
        chunk_node = cur_node->chunk_nodes[offset];
        while(chunk_node)
        {
            if(chunk_node->key == *word)
            {
                break;
            }
            chunk_node = chunk_node->next;
        }
        if(chunk_node)
        {
            ++word;
            cur_node = chunk_node;
        }
        else
        {
            break;
        }
    }

    if(*word)
    {
        return NULL;
    }
    else
    {
        return cur_node;
    }
}

/*void process(trie_tree *tree, vector<string> v_seg_word, const char *query, vector<string> &v_addr)
{
    assert(tree);
    assert(query);

    for(vector<string>::iterator iter=v_seg_word.begin(); iter<v_seg_word.end(); iter++)
    {
        v_addr.push_back(*iter);
    }

    trie_node *root_node = &(tree->root_node);
    trie_node *cur_node = &(tree->root_node);

    trie_node *chunk_node;
    int offset;
    char *p = (char *)query;
    char *p_end = (char *)query;
    while(*p_end)
    {
        if(!(cur_node->chunk_size))
        {
            if(cur_node!=root_node)
            {
                cur_node=root_node;
                continue;
            }
            break;
        }
        offset = (unsigned char)*p_end % cur_node->chunk_size;
        chunk_node = cur_node->chunk_nodes[offset];
        while(chunk_node)
        {
            if(chunk_node->key == *p_end)
            {
                if(chunk_node->preq == 1)
                {
                    string addr;
                    addr.assign(p,p_end-p+1);
                    v_addr.push_back(addr);
                }
                break;
            }
            chunk_node = chunk_node->next;
        }
        if(chunk_node)
        {
            ++p_end;
            cur_node = chunk_node;
        }
        else
        {
            p++;
            p_end=p;
            cur_node = root_node;
        }
    }
}*/

void foreach_trie_node(trie_node *node, char *buffer, int buffer_offset)
{
    assert(node && buffer);
    assert(buffer_offset >= 0);

    char *cur_buffer = buffer + buffer_offset;
    trie_node *cur_node;
    *cur_buffer = node->key;
    if(node->preq)
    {
        *(cur_buffer+1) = '\0';
        //to-do: buffer is current word.
        fprintf(stdout, "%s %d\n", buffer, node->preq);
    }

    if(node->chunk_size)
    {
        for(int i = 0; i < node->chunk_size; ++i)
        {
            cur_node = node->chunk_nodes[i];
            while(cur_node)
            {
                foreach_trie_node(cur_node, buffer, buffer_offset + 1);
                cur_node = cur_node->next;
            }
        }
    }
}

void foreach_trie_tree(trie_tree *tree)
{
    assert(tree);

    char buffer[256];
    trie_node *root_node = &(tree->root_node);
    if(root_node->chunk_size)
    {
        for(int i = 0; i < root_node->chunk_size; ++i)
        {
            trie_node *cur_node = root_node->chunk_nodes[i];
            while(cur_node)
            {
                foreach_trie_node(cur_node, buffer, 0);
                cur_node = cur_node->next;
            }
        }
    }
}
