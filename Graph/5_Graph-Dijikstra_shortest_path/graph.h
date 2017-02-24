
#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE         0
#define FALSE        1 
#define SUCCESS      0
#define FAILURE     -1
#define END_OF_FILE -2
#define END_OF_LIST -1
#define ALLOWED      2
#define NOT_ALLOWED  3
#define EXISTS       4
#define WHITE        5
#define GREY         6
#define BLACK        7

typedef int color_t;
typedef double cost_t;
typedef int vertex_t;

struct adj_node
{
	vertex_t vertex;
	color_t  color;
	vertex_t pread;
	cost_t weight;
	struct adj_node *prev,*next;
};

typedef struct adj_node adj_node_t;

struct vert_node
{
	vertex_t vertex;
	color_t color;
	vertex_t pread;
	cost_t distance;
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
typedef graph_t queue_t;

typedef int result_t;

//struct for saving in to the files
struct graph_node
{
	vertex_t vertex;
};
typedef struct graph_node graph_node_t;

struct graph_capsule
{
	int nr_of_vertices;
	int nr_of_edges;
};
typedef struct graph_capsule graph_capsule_t;


//Export routines
graph_t * create_graph();
vertex_t add_vertex(graph_t *g);
result_t add_edge(graph_t *g,vertex_t v1,vertex_t v2);
result_t delete_vertex(graph_t *g,vertex_t v1);
result_t delete_edge(graph_t *g,vertex_t v1,vertex_t v2);
result_t print_graph(graph_t *g);
result_t delete_graph(graph_t **g);
result_t save_graph(graph_t *g,char *file_name);
graph_t *construct_graph(char *file_name);
vertex_t *traverse_bfs(graph_t *g);
vertex_t *traverse_dfs(graph_t *g);
int length(graph_t *g);
vertex_t *shortest_path(graph_t *g,vertex_t v_source,vertex_t v_dest);
result_t add_weight(graph_t *g,vertex_t v1,vertex_t v2,cost_t weight);


//internal routines
vertex_t get_next_vertex_number();
vert_node_t *search_vert_node(graph_t *g,vertex_t v1);
adj_node_t *search_adj_node(graph_t *g,vertex_t v1,vertex_t v2);
vert_node_t *add_vert_node(graph_t *g,vertex_t v1);
adj_node_t *add_adj_node(graph_t *g,vert_node_t *v1,vertex_t v2);
result_t  remove_adj_node(graph_t *g,vertex_t v1,vertex_t v2);
result_t  remove_vert_node(graph_t *g,vertex_t v1);
result_t validate_insertion(graph_t *g,vertex_t v1,vertex_t v2);

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
//internal save routines
int create_capsule(graph_t *g,char *file_name);
result_t save_all_nodes(graph_t *g,int fd);
//internal graph construct routines
int get_file_handle(char *file_name);
graph_capsule_t *get_capsule(int fd);
vertex_t get_graph_vertex(int fd);
//internal routines for traverse_bfs
result_t reset_graph(graph_t *g);
result_t set_node(vert_node_t *vert,vert_node_t *pvert,color_t color);
//internal queue routines
queue_t *create_queue();
result_t enqueue(queue_t *q,vert_node_t *v1);
vertex_t dequeue(queue_t *q);
result_t is_empty(queue_t *q);
//internal DFS routines
result_t dfs_visit(graph_t *g,vert_node_t *req_node,vertex_t *v,int i);
//internal shortest path algorithm
result_t add_weight(graph_t *g,vertex_t v1,vertex_t v2,cost_t weight);
vertex_t *get_shortest_path(graph_t *g,vertex_t v_source,vertex_t v_dest);
result_t relax(vert_node_t *req_node,vert_node_t *node,cost_t weight);
vert_node_t *extract_min(queue_t *q,graph_t *g);
queue_t *push_all_vertex_in_queue(graph_t *g);
result_t init_source(graph_t *g,vertex_t v_source);


#endif