#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	vertex_t *ver;
	int i;
	graph_t *gra=create_graph();
	graph_t *gra2;

	vertex_t v1,v2,v3;

	v1=add_vertex(gra);
	v2=add_vertex(gra);
	v3=add_vertex(gra);

	add_edge(gra,v1,v2);
	add_edge(gra,v1,v3);
	add_edge(gra,v2,v3);

	print_graph(gra);
	save_graph(gra,"mygraph.graph");
	printf("graph saved \n");
	
	printf("\ngraph constructing \n");
	gra2=construct_graph("mygraph.graph");
	print_graph(gra2);
	
	ver=traverse_bfs(gra);
	printf("\ntraverse_bfs=\n");
	for(i=0;i<gra->nr_of_vertices;i++)
	{
		printf("%d  ",*(ver+i));
	}
	printf("\n");
	delete_edge(gra,v1,v2);
	printf("delete_edge\n\n");
	print_graph(gra);

	delete_vertex(gra,v2);
	printf("delete_v2\n\n");
	print_graph(gra);

	printf("\nGRAPH delete\n");
	delete_graph(&gra);
	

	return 0;
}
