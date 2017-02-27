#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "graph.h"



//creates a graph and returns the handle
graph_t * create_graph()
{
	graph_t *g=NULL;
	g=(graph_t *)xcalloc(1,sizeof(graph_t));
	if(g==NULL)
	{
		fprintf(stderr, "\nfailed to create graph.\n");
		exit(EXIT_FAILURE);
	}
	g->nr_of_vertices=0;
	g->nr_of_edges=0;

	g->head=NULL;
	g->head=get_vert_node(-1);
	if(g->head==NULL)
	{
		fprintf(stderr, "\nfailed to create vertex head.\n");
		exit(EXIT_FAILURE);
	}

	return (g);

}

//add vertex and returns the handle
vertex_t add_vertex(graph_t *g)
{
	int vert_number;
	vert_node_t *vert=NULL;

	vert_number=g->nr_of_vertices + 1;
	vert=add_vert_node(g,vert_number);
	if(vert==NULL)
	{
		fprintf(stderr, "\nfailed to add vertex node.\n");
		exit(EXIT_FAILURE);
	}

	g->nr_of_vertices = g->nr_of_vertices +1;

	return (vert_number);
}

//add the edge and retun SUCCESS or FAILURE
result_t add_edge(graph_t *g,vertex_t v1,vertex_t v2)
{
int res;
	vert_node_t *v1_node=NULL,*v2_node=NULL;
	adj_node_t *v1_adj=NULL,*v2_adj=NULL;
	
	res=validate_insertion(g,v1,v2);
	
	if(res!=ALLOWED)
		return (FAILURE);

	v1_node=search_vert_node(g,v1);
	v2_node=search_vert_node(g,v2);
	if (v1_node==NULL || v2_node==NULL)
	{
		fprintf(stderr, "\nVertex not found!\n");
	}

	v1_adj=add_adj_node(g,v2_node,v1);
	v2_adj=add_adj_node(g,v1_node,v2);

	g->nr_of_edges= g->nr_of_edges + 1;

	
	return (SUCCESS);
}

//prints the graph
result_t print_graph(graph_t *g)
{
	vert_node_t *vert_head=g->head,*vert_run=vert_head->next;
	adj_node_t  *adj_head,*adj_run;

	while(vert_run != vert_head)
	{
		adj_head=vert_run->head;
		adj_run=adj_head->next;
		printf("%d",vert_run->vertex);
		while(adj_run !=adj_head)
		{
			printf("<--->%d",adj_run->vertex);
			adj_run=adj_run->next;
		}
		vert_run=vert_run->next;
		printf("\n");
	}

	return (SUCCESS);
}

//delete the edge
result_t delete_edge(graph_t *g,vertex_t v1,vertex_t v2)
{
	result_t chk;
	chk=remove_adj_node(g,v1,v2);
	if(chk==FAILURE)
	{
		fprintf(stderr, "\nfailed to delete edge\n" );
		exit(EXIT_FAILURE);
	}
	g->nr_of_edges=g->nr_of_edges-1;

	return (SUCCESS);
}

//delete the vertex
result_t delete_vertex(graph_t *g,vertex_t v1)
{
	result_t chk;
	chk=remove_vert_node(g,v1);

	if(chk==FAILURE)
	{
		fprintf(stderr, "\nfailed to delete vertex\n" );
		exit(EXIT_FAILURE);
	}
	g->nr_of_vertices=g->nr_of_vertices-1;

	return (SUCCESS);

}

//deleting total graph
result_t delete_graph(graph_t **g)
{
	vert_node_t *headv=(*g)->head,*run=headv->next;

	while(run!=headv)
	{
		delete_vertex(*g,run->vertex);
		run=run->next;
	}
	free(headv);
	free(*g);
	*g=NULL;

	return (SUCCESS);
}

//save the graph
result_t save_graph(graph_t *g,char *file_name)
{
	int fd,err;

	fd=create_capsule(g,file_name);
	if(fd==-1)
	{
		fprintf(stderr,"\nFailed to create capsule while saving\n");
		exit(EXIT_FAILURE);
	}
	err=save_all_nodes(g,fd);
	if(err!=SUCCESS)
	{
		fprintf(stderr,"\nFailed while saving\n");
		exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}

//construct graph
graph_t *construct_graph(char *file_name)
{
	int fd,i;
	graph_t *g=NULL;
	graph_capsule_t *caps=NULL;
	vertex_t v_read;
	
	fd=get_file_handle(file_name);
	if(fd==-1)
	{
		fprintf(stderr,"\nFailed to open the file:%s\n",file_name);
		exit(EXIT_FAILURE);
	}
	caps=get_capsule(fd);
	if(fd==-1)
	{
		fprintf(stderr,"\nFailed to read the capsule from  file:%s\n",file_name);
		exit(EXIT_FAILURE);
	}
	g=create_graph();
	if(g==NULL)
	{
		fprintf(stderr,"\nFailed to create graph\n");
		exit(EXIT_FAILURE);
	}
	vertex_t v[caps->nr_of_vertices];
	for(i=0;i<caps->nr_of_vertices;i++)
	{
		v[i]=add_vertex(g);
		//saved all the vertex handles in an array
	}
	
	i=0;
	while((v_read=get_graph_vertex(fd))!=END_OF_FILE)
	{
		if(v_read!=v[i])
		{
			fprintf(stderr,"\nSomething went wrong while reading\n");
			exit(EXIT_FAILURE);
		}
			while((v_read=get_graph_vertex(fd))!=END_OF_LIST)
			{
				add_edge(g,v[i],v_read);
			}
			i=i+1;
		
	}
	
	if(g->nr_of_edges!=caps->nr_of_edges)
	{
		fprintf(stderr,"\nSomething went wrong while creating\n");
		exit(EXIT_FAILURE);
	}
	return (g);
}

//returns total no of vertices ie length of graph
int  length(graph_t *g)
{
	return (g->nr_of_vertices);
}



//list manipulation routines


//check if edge is valid or not
result_t validate_insertion(graph_t *g,vertex_t v1,vertex_t v2)
{
	adj_node_t *adj1=NULL,*adj2=NULL;
	if(v1==v2)
		return (NOT_ALLOWED);
	
	adj1=search_adj_node(g,v1,v2);
	adj2=search_adj_node(g,v2,v1);
	if(adj1==NULL && adj2==NULL)
		return (ALLOWED);
	else	
		return (EXISTS);	
}


//remove the vertex and also delete all associated edges from both list of both vertex
result_t  remove_vert_node(graph_t *g,vertex_t v1)
{
	result_t chk;
	vert_node_t *vert;
	adj_node_t *heada,*run;

	vert=search_vert_node(g,v1);
	heada=vert->head;
	run=heada->next;
	while(run!=heada)
	{
		delete_edge(g,v1,run->vertex);	
		run=run->next;
	}
	free(heada);

	chk=remove_link_vert(vert->prev,vert,vert->next);
	if(chk==FAILURE)
	{
		fprintf(stderr, "\nfailed to delete vertex\n" );
		exit(EXIT_FAILURE);
	}

	return (SUCCESS);
}

//remove the edge from both list of both vertex
result_t  remove_adj_node(graph_t *g,vertex_t v1,vertex_t v2)
{
	adj_node_t *v1_adj=NULL,*v2_adj=NULL;
	result_t chk1,chk2;

	v1_adj=search_adj_node(g,v1,v2);
	v2_adj=search_adj_node(g,v2,v1);
	if(v1_adj==NULL || v2_adj==NULL)
	{
		fprintf(stderr, "\nfailed to find vertex node.\n");
		exit(EXIT_FAILURE);
	}
	//for v2 in list of v1
	chk1=remove_link_adj(v1_adj->prev,v1_adj,v1_adj->next);
	//for v1 in list of v2
	chk2=remove_link_adj(v2_adj->prev,v2_adj,v2_adj->next);

	if(chk1==SUCCESS && chk2==SUCCESS)
		return (SUCCESS);
	else
		return (FAILURE);
}

//returns pointer to vert_node
vert_node_t *search_vert_node(graph_t *g,vertex_t v1)
{
	vert_node_t *head=NULL,*run=NULL;

	head=g->head;
	run=head->next;

	while(run != head)
	{
		if(run->vertex==v1)
		{
			return (run);
		}
		run=run->next;
	}
	return (NULL);
}


//return pointer to adj_node from vertex v1 of the vertex v2
adj_node_t *search_adj_node(graph_t *g,vertex_t v1,vertex_t v2)
{
	vert_node_t *vert=NULL;
	adj_node_t *head=NULL,*run=NULL;

	vert=search_vert_node(g,v1);
	if(vert==NULL)
	{
		fprintf(stderr, "\nfailed to find vertex node.\n");
		exit(EXIT_FAILURE);
	}

	head=vert->head;
	run=head->next;
	while(run != head)
	{
		if(run->vertex == v2)
		{
			return (run);
		}
		run=run->next;
	}
	return (NULL);
}

//create and insert new vertex into list and return its pointer
vert_node_t *add_vert_node(graph_t *g,vertex_t v1)
{
	vert_node_t *head=g->head,*last=head->prev;
	result_t chk;
	vert_node_t *new=get_vert_node(v1);

	chk=add_link_vert(last,new,head);
	if (chk==SUCCESS)
		return (new);
	else
		return (NULL);

}

//create new adj_node in list of vertex v1 and returns the pointer
adj_node_t *add_adj_node(graph_t *g,vert_node_t *v1,vertex_t v2)
{
	adj_node_t *head=v1->head,*last=head->prev;
	result_t chk;
	adj_node_t *new=get_adj_node(v2);

	chk=add_link_adj(last,new,head);
	if (chk==SUCCESS)
		return (new);
	else
		return (NULL);
}



//node init routines

//create vertex node with default values
vert_node_t *get_vert_node(vertex_t v1)
{
	vert_node_t *temp=NULL;
	temp=(vert_node_t *)xcalloc(1,sizeof(vert_node_t));
	if(temp==NULL)
	{
		fprintf(stderr, "\nfailed to create vertex node.\n");
		exit(EXIT_FAILURE);
	}
	temp->vertex=v1;
	temp->color=WHITE;
	temp->pread=-1;
	temp->distance=-1;
	temp->next=temp;
	temp->prev=temp;
	temp->head=NULL;
	temp->head=get_adj_node(-1);
	if(temp->head==NULL)
	{
		fprintf(stderr, "\nfailed to create adjecent node.\n");
		exit(EXIT_FAILURE);
	}

	return (temp);
}

//create adj_node  with default values
adj_node_t *get_adj_node(vertex_t v1)
{
	adj_node_t *temp=NULL;
	temp=(adj_node_t *)xcalloc(1,sizeof(adj_node_t));
	if(temp==NULL)
	{
		fprintf(stderr, "\nfailed to create adjecent node.\n");
		exit(EXIT_FAILURE);
	}
	temp->vertex=v1;
	temp->color=WHITE;
	temp->pread=-1;
	temp->weight=-1;
	temp->prev=temp;
	temp->next=temp;

	return (temp);
}

//internal link management routines

//insert vertex into list
result_t add_link_vert(vert_node_t *beg,vert_node_t *mid,vert_node_t *end)
{
	mid->next=end;
	mid->prev=beg;
	beg->next=mid;
	end->prev=mid;

	return (SUCCESS);
}

//insert edge into adj_list
result_t add_link_adj(adj_node_t *beg,adj_node_t *mid,adj_node_t *end)
{
	mid->next=end;
	mid->prev=beg;
	beg->next=mid;
	end->prev=mid;

	return (SUCCESS);
}

//remove edge from adj_list
result_t remove_link_adj(adj_node_t *beg,adj_node_t *mid,adj_node_t *end)
{
	beg->next=end;
	end->prev=beg;
	free(mid);

	return (SUCCESS);
}

//remove vertex from list
result_t remove_link_vert(vert_node_t *beg,vert_node_t *mid,vert_node_t *end)
{
	beg->next=end;
	end->prev=beg;
	free(mid);

	return (SUCCESS);
}

//save routines

//create file and save the capsule
int create_capsule(graph_t *g,char *file_name)
{
	int fd,err;
	
	fd=open(file_name,O_CREAT|O_RDWR|O_TRUNC|O_EXCL,S_IRWXU|S_IRWXG|S_IROTH);
	if(fd==-1)
	{
		fprintf(stderr,"\nFailed to create the file:%s\n",file_name);
		exit(EXIT_FAILURE);
	}
	graph_capsule_t *caps=(graph_capsule_t *)xcalloc(1,sizeof(graph_capsule_t));
	caps->nr_of_edges    =g->nr_of_edges;
	caps->nr_of_vertices =g->nr_of_vertices;
	
	err=write(fd,caps,sizeof(caps));
	if(fd==-1)
	{
		fprintf(stderr,"\nFailed to write capsule:%s\n",file_name);
		exit(EXIT_FAILURE);
	}
	
	return (fd);
	
}
//save all the nodes
result_t save_all_nodes(graph_t *g,int fd)
{
	int err;
	graph_node_t *node=(graph_node_t *)xcalloc(1,sizeof(graph_node_t));
	vert_node_t *vert_head=g->head,*vert_run=vert_head->next;
	adj_node_t  *adj_head,*adj_run;

	while(vert_run != vert_head)
	{
		//writing the vertex ie vert_node_t
		node->vertex=vert_run->vertex;
		err=write(fd,node,sizeof(graph_node_t));
		if(err==-1)
		{
			fprintf(stderr,"\nfailed to write vert_node_t\n");
			exit(EXIT_FAILURE);
		}
		adj_head=vert_run->head;
		adj_run=adj_head->next;
		while(adj_run !=adj_head)
		{
			node->vertex=adj_run->vertex;
			err=write(fd,node,sizeof(graph_node_t));
			if(err==-1)
			{
				fprintf(stderr,"\nfailed to write adj_node_t\n");
				exit(EXIT_FAILURE);
			}
			adj_run=adj_run->next;
		}
			//end of adj_list
		node->vertex=END_OF_LIST;
		err=write(fd,node,sizeof(graph_node_t));
		if(err==-1)
		{
			fprintf(stderr,"\n failed to write END_OF_LIST\n");
			exit(EXIT_FAILURE);
		}
		vert_run=vert_run->next;	
	}
	//end of file
	node->vertex=END_OF_FILE;
	err=write(fd,node,sizeof(graph_node_t));
	if(err==-1)
	{
		fprintf(stderr,"\n failed to write END_OF_FILE\n");
		exit(EXIT_FAILURE);
	}

	return (SUCCESS);
}
//graph construct routines

//open file and return fd
int get_file_handle(char *file_name)
{
	int fd;
	fd=open(file_name,O_RDWR);
	if(fd==-1)
	{
		fprintf(stderr,"\n failed to open :%s\n",file_name);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

//read header of file ie.capsule
graph_capsule_t *get_capsule(int fd)
{
int err;
	graph_capsule_t *caps=(graph_capsule_t *)xcalloc(1,sizeof(graph_capsule_t));
	
	err=read(fd,caps,sizeof(graph_capsule_t));
	if(err==-1)
	{
		fprintf(stderr,"\n failed to read capsule \n");
		exit(EXIT_FAILURE);
	}
	return (caps);
}

//read next vertex
vertex_t get_graph_vertex(int fd)
{
int err;
vertex_t vert;
graph_node_t *node=(graph_node_t *)xcalloc(1,sizeof(graph_node_t));	

	err=read(fd,node,sizeof(graph_node_t));
	if(err==-1)
	{
		fprintf(stderr,"\n failed to read graph_node\n");
		exit(EXIT_FAILURE);
	}
	
	vert=node->vertex;
	
	return (vert);
}
//BFS traversal

//Queue manipulation routines

//return TRUE if queue is empty
result_t is_empty(queue_t *q)
{
	if( (q->nr_of_vertices) == 0)
		return (TRUE);
	else
		return (FALSE);
}

queue_t *create_queue()
{
	queue_t *q;
	q=create_graph();
	
	return (q);
}

result_t enqueue(queue_t *q,vert_node_t *v1)
{
	vert_node_t *vert=NULL;

	vert=add_vert_node(q,v1->vertex);
	if(vert==NULL)
	{
		fprintf(stderr, "\nfailed to add node in queue.\n");
		exit(EXIT_FAILURE);
	}

	q->nr_of_vertices = q->nr_of_vertices +1;
	return (SUCCESS);
}

vertex_t dequeue(queue_t *q)
{

	vert_node_t *head=q->head;
	vert_node_t *first_node=head->next;
	vertex_t ver=first_node->vertex;
	
	result_t chk;
	chk=remove_vert_node(q,ver);

	if(chk==FAILURE)
	{
		fprintf(stderr, "\nfailed to delete from queue\n" );
		exit(EXIT_FAILURE);
	}
	q->nr_of_vertices=q->nr_of_vertices-1;



	return (ver);
}

//set color,distance,pred of vertex
result_t set_node(vert_node_t *vert,vert_node_t *pvert,color_t color)
{
	vert->color=color;
	vert->distance=pvert->distance+1;
	vert->pread=pvert->vertex;
	
	return (SUCCESS);
}

//init the other variables in a node
result_t reset_graph(graph_t *g)
{
	vert_node_t *vert_head=g->head,*vert_run=vert_head->next;

	while(vert_run != vert_head)
	{
		vert_run->color=WHITE;
		vert_run->pread=-1;
		vert_run->distance=-1;

		vert_run=vert_run->next;
	}

	return (SUCCESS);
}
vertex_t *traverse_bfs(graph_t *g)
{
	int i=0;
	result_t res;
	vertex_t *v=(vertex_t *)xcalloc(g->nr_of_vertices,sizeof(vertex_t));
	vertex_t ver;
	queue_t *q;
	vert_node_t *vert_head=g->head,*vert_run=vert_head->next,*req_node=NULL;
	adj_node_t  *adj_head,*adj_run;
	
	res=reset_graph(g);
	if (res!=SUCCESS)
	{
		fprintf(stderr, "\nreset graph failed.\n");
		exit(EXIT_FAILURE);
	}
	res=set_node(vert_run,vert_run,GREY);
	if (res!=SUCCESS)
	{
		fprintf(stderr, "\nreset graph failed.\n");
		exit(EXIT_FAILURE);
	}
	
	q=create_queue();
	enqueue(q,vert_run);
	while( (is_empty(q)) == FALSE)
	{
		ver=dequeue(q);
		v[i]=ver;
		i++;
		
		vert_run=search_vert_node(g,ver);
			adj_head=vert_run->head;
			adj_run=adj_head->next;
			
			while(adj_run !=adj_head)
			{
				req_node=search_vert_node(g,adj_run->vertex);

				if(req_node->color==WHITE)
				{
					set_node(req_node,vert_run,GREY);
					enqueue(q,req_node);
				}
				adj_run=adj_run->next;
			}
			set_node(req_node,vert_run->next,BLACK);
			vert_run=vert_run->next;
	}
	
	return (v);
}

//DFS routines
vertex_t *traverse_dfs(graph_t *g)
{
	 vertex_t *v=(vertex_t *)xcalloc(g->nr_of_vertices,sizeof(vertex_t));
	static int i=0;
	result_t res;

	vert_node_t *head=g->head,*run=head->next;

	res=reset_graph(g);
	if (res!=SUCCESS)
	{
		fprintf(stderr, "\nreset graph failed.\n");
		exit(EXIT_FAILURE);
	}

	while(run!=head)
	{
		if (run->color == WHITE)
		{
			dfs_visit(g,run,v,i);
		}
		run=run->next;
	}

	return (v);
}

//internal DFS routines
result_t dfs_visit(graph_t *g,vert_node_t *req_node,vertex_t *v,int i)
{
	
	v[i]=req_node->vertex;
	i++;

	vert_node_t *vnode=NULL;
	
	adj_node_t *head=req_node->head,*run=head->next;

	req_node->color=GREY;

	while(run!=head)
	{
		vnode=search_vert_node(g,run->vertex);
		if (vnode->color==WHITE)
		{
			vnode->pread=req_node->vertex;
			dfs_visit(g,vnode,v,i);
		}
		run=run->next;
	}
	req_node->color=BLACK;

	return (SUCCESS);
}

//memory allocation routines

//initialize and check allocated memory
void *xcalloc(int nr_of_elements,int size_per_element)
{
	void *temp=NULL;
	temp=calloc(nr_of_elements,size_per_element);
	if(temp==NULL)
	{
		fprintf(stderr, "\ncalloc failed.\n");
		exit(EXIT_FAILURE);
	}
	return temp;
}


vertex_t *shortest_path(graph_t *g,vertex_t v_source,vertex_t v_dest)
{
	adj_node_t *a_head,*a_run;
	vert_node_t *req_node,*node;
	vertex_t *v;

	queue_t *q;
	init_source(g,v_source);

	q=push_all_vertex_in_queue(g);

	while((is_empty(q))==FALSE)
	{
		req_node=extract_min(q,g);

		a_head=req_node->head;
		a_run=a_head->next;

		while(a_run!=a_head)
		{
			node=search_vert_node(g,a_run->vertex);
			relax(req_node,node,a_run->weight);
			a_run=a_run->next;
		}
	}

	v=get_shortest_path(g,v_source,v_dest);

	return (v);
}

result_t init_source(graph_t *g,vertex_t v_source)
{

	vert_node_t *vert_head=g->head,*vert_run=vert_head->next;

	while(vert_run != vert_head)
	{
		vert_run->color=WHITE;
		vert_run->pread=-1;
		vert_run->distance=99999.9999;

		vert_run=vert_run->next;
	}
	vert_node_t *vnode=NULL;

	vnode=search_vert_node(g,v_source);
	if (vnode==NULL)
	{
		fprintf(stderr, "\nVertex not found\n");
		exit(EXIT_FAILURE);

	}

	vnode->distance=0;

	return (SUCCESS);
}

queue_t *push_all_vertex_in_queue(graph_t *g)
{

	queue_t *q=NULL;
	vert_node_t *head=g->head,*run=head->next;

	q=create_queue();
	if (q==NULL)
	{
		fprintf(stderr, "\nCreate queue failed.\n");
		exit(EXIT_FAILURE);

	}

	while(run!=head)
	{
		enqueue(q,run);
		run=run->next;
	}

	return (q);
}

vert_node_t *extract_min(queue_t *q,graph_t *g)
{

	vert_node_t *head=q->head,*run=head->next;
	vert_node_t *g_head=g->head,*g_run=g_head->next;
	vert_node_t *min=search_vert_node(g,run->vertex);

	while(run!=head)
	{
		g_run=search_vert_node(g,run->vertex);

		if ( (min->distance > g_run->distance) )
		{
			min=g_run;
		}
		run=run->next;
	}
	//deleting min vertex from queue
	delete_vertex(q,min->vertex);
	return (min);
}

result_t relax(vert_node_t *req_node,vert_node_t *node,cost_t weight)
{

	if (node->distance > (req_node->distance + weight))
	{
		node->distance=req_node->distance + weight;
		node->pread=req_node->vertex;
	}

	return (SUCCESS);
}

vertex_t *get_shortest_path(graph_t *g,vertex_t v_source,vertex_t v_dest)
{
	int i=0;
	vertex_t *v=xcalloc(2,sizeof(vertex_t));
	vert_node_t *src=search_vert_node(g,v_source);
	vert_node_t *run=search_vert_node(g,v_dest);

	v[i++]=v_source;
	while(run->pread !=v_source)
	{
		v=realloc(v,sizeof(v)+sizeof(vertex_t));
		v[i++]=run->pread;
		run=search_vert_node(g,run->pread);
	}
	v[i++]=v_dest;

	return (v);
}

result_t add_weight(graph_t *g,vertex_t v1,vertex_t v2,cost_t weight)
{
	adj_node_t *v1_adj=NULL,*v2_adj=NULL;
	
	v1_adj=search_adj_node(g,v1,v2);
	v2_adj=search_adj_node(g,v2,v1);

	v1_adj->weight=weight;
	v2_adj->weight=weight;

	return (SUCCESS);
}

int shortest_path_length(vertex_t *v1)
{
	return (sizeof(v1) / sizeof(vertex_t));
}

//bellman ford
vertex_t *bellman_ford_path(graph_t *g,vertex_t v_source,vertex_t v_dest)
{
	vert_node_t *vhead=g->head,*vrun=vhead->next,*vreq;
	adj_node_t *ahead,*arun;
	vertex_t *v;
	
	init_source(g,v_source);
	
	while(vrun!=vhead->prev)
	{
		ahead=vrun->head;
		arun=ahead->next;
		while(arun!=ahead)
		{
			vreq=search_vert_node(g,arun->vertex);
			relax(vrun,vreq,arun->weight);
			arun=arun->next;
		}
		vrun=vrun->next;
	}
	
	v=get_shortest_path(g,v_source,v_dest);
	
	return (v);
}