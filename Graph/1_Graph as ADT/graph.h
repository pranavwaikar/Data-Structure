
#ifndef _GRAPH_H
#define _GRAPH_H

#define    TRUE     0
#define    False    1
#define    SUCCESS  0
#define    FAILURE -1


typedef int vertex_t;

struct adj_node
{
	vertex_t vertex;
	struct adj_node *prev,*next;
};

typedef struct adj_node adj_node_t;

struct vert_node
{
	vertex_t vertex;
	adj_node_t *head;
	struct vert_node *next,*prev;
};

typedef struct vert_node vert_node_t;

struct graph
{
	vert_node_t *head;
	int nr_of_vertices;
	int nr_of_edges;
};

typedef struct graph graph_t;

typedef int result_t;

//Export routines
graph_t * create_graph();
vertex_t add_vertex(graph_t *g);
result_t add_edge(graph_t *g,vertex_t v1,vertex_t v2);
result_t delete_vertex(graph_t *g,vertex_t v1);
result_t delete_edge(graph_t *g,vertex_t v1,vertex_t v2);
result_t print_graph(graph_t *g);
result_t delete_graph(graph_t **g);

//internal routines
vertex_t get_next_vertex_number();
vert_node_t *search_vert_node(graph_t *g,vertex_t v1);
adj_node_t *search_adj_node(graph_t *g,vertex_t v1,vertex_t v2);
vert_node_t *add_vert_node(graph_t *g,vertex_t v1);
adj_node_t *add_adj_node(graph_t *g,vert_node_t *v1,vertex_t v2);
result_t  remove_adj_node(graph_t *g,vertex_t v1,vertex_t v2);
result_t  remove_vert_node(graph_t *g,vertex_t v1);
//internal link management routines
result_t add_link_vert(vert_node_t *beg,vert_node_t *mid,vert_node_t *end);
result_t add_link_adj(adj_node_t *beg,adj_node_t *mid,adj_node_t *end);
result_t remove_link_adj(adj_node_t *beg,adj_node_t *mid,adj_node_t *end);
result_t remove_link_vert(vert_node_t *beg,vert_node_t *mid,vert_node_t *end);
//node init routines
vert_node_t *get_vert_node(vertex_t v1);
adj_node_t *get_adj_node(vertex_t v1);
//memory allocation routines
void *xcalloc(int nr_of_elements,int size_per_element);


#endif