#include "graph.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
	graph_t *gra=create_graph();

	vertex_t v1,v2,v3;

	v1=add_vertex(gra);
	v2=add_vertex(gra);
	v3=add_vertex(gra);

	add_edge(gra,v1,v2);
	add_edge(gra,v1,v3);
	add_edge(gra,v2,v3);

	print_graph(gra);

	delete_edge(gra,v1,v2);
	printf("delete_edge\n\n");
	print_graph(gra);

	delete_vertex(gra,v2);
	printf("delete_v2\n\n");
	print_graph(gra);

	delete_graph(&gra);
	printf("\nGRAPH delete\n");

	return 0;
}
