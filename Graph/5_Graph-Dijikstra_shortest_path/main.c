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
	
	printf("\ntraverse_bfs=\n");
	ver=traverse_bfs(gra);
	for(i=0;i<length(gra);i++)
	{
		printf("%d  ",*(ver+i));
	}
	printf("\n");

	printf("\ntraverse_dfs=\n");
	ver=traverse_dfs(gra);
	
	for(i=0;i<length(gra);i++)
	{
		printf("%d  ",*(ver+i));
	}
	printf("\n");


	add_weight(gra,v1,v2,3);
	add_weight(gra,v1,v3,2);
	add_weight(gra,v2,v3,5);

	ver=shortest_path(gra,v1,v3);
	printf("\nshortest_path=\n");
	for(i=0;i<shortest_path_length(ver);i++)
	{
		printf("%d  ",*(ver+i));
	}

	delete_edge(gra,v1,v2);
	printf("\ndelete_edge\n");
	print_graph(gra);

	delete_vertex(gra,v2);
	printf("\ndelete_v2\n");
	print_graph(gra);

	printf("\nGRAPH delete\n");
	delete_graph(&gra);
	
	free(ver);

	return 0;
}
