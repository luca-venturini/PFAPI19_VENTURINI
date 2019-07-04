#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 10
#define MAX_RELATION 30
#define NAME 256

struct list_t;
struct primary_tree_t;

typedef struct list_t list;
typedef struct primary_tree_t tree;
typedef struct relation_tree_t relation_tree;
typedef struct max_l_t max_list;

struct max_l_t{
  struct max_l_t* p;
  tree* id;
  struct max_l_t* left;
  struct max_l_t* right;
};

struct list_t{
  char relation[NAME];
  int num_relation;
  max_list* max_lista;
  struct list_t* next;
};

struct relation_tree_t{
  struct relation_tree_t* p;
  tree* id;
  struct relation_tree_t* left;
  struct relation_tree_t* right;
};

struct primary_tree_t{
  struct primary_tree_t* p;
  char name[NAME];
  relation_tree* relation[MAX_RELATION];
  char *relation_name[MAX_RELATION];
  int occorrenze[MAX_RELATION];
  struct primary_tree_t* left;
  struct primary_tree_t* right;
};

int compare(char a[], char b[]){
  int i = 0;
  while(a[i] != '\0' && b[i] != '\0'){
    if (a[i]>b[i])
      return 1;
    if (a[i]<b[i])
      return 0;
    i++;
  }
  if (a[i] != '\0')
    return 1;
  return 0;
}

void stampa(tree* t){
  if (t != NULL){
    stampa(t->left);
    printf("nome: %s\n", t->name);
    stampa(t->right);
  }
}


/*** inizio funzioni per addent ***/
int add_entity(char* name, tree** primary_tree){


  tree* y = NULL;
  tree* x = *primary_tree;
  while(x != NULL){
    y = x;
    if (strcmp (name, x->name) == 0){
      return 0;
    }
    if (compare(name, x->name) == 0)
      x = x->left;
    else
      x = x->right;
  }
  tree* z;
  z = malloc(sizeof(tree));
  z->left = NULL;
  z->right = NULL;
  for (int i = 0; i<MAX_RELATION; i++){
    z->occorrenze[i] = 0;
    z->relation[i] = NULL;
    z->relation_name[i] = NULL;
  }
  strcpy(z->name, name);
  z->p = y;
  if (y == NULL)
    (*primary_tree) = z;
  else if (compare(name, y->name) == 0)
    y->left = z;
  else
    y->right = z;
  return 1;
}


/*** inizio funzioni per addrel ***/
tree* find_id(char id[], tree* x){
  if(x == NULL){
    return NULL;
  }
  if( strcmp(id, x->name) == 0){
    return x;
    printf("find!\n");
  }
  if(compare(id, x->name)==0)
    return find_id(id, x->left);
  else
    return find_id(id, x->right);
}

list* find_relation_global(char rel[], list** lista){

  list* temp;
  list* temp_prev;
  temp_prev = NULL;
  temp = *lista;
  while( temp != NULL && compare(temp->relation, rel) == 0 && strcmp(temp->relation, rel) != 0){
    temp_prev = temp;
    temp = temp->next;
  }
  if (temp != NULL &&  strcmp(temp->relation, rel) == 0){
    return temp;
  }
  list* n;
  n = malloc(sizeof(list));
  strcpy(n->relation, rel);
  n->num_relation = 0;
  n->next = temp;

  if (temp_prev == NULL)
    *lista = n;
  else{
    temp_prev->next = n;
  }
  return n;
}

int relation_tree_insert(relation_tree** tr, tree* id){


  relation_tree* y = NULL;
  relation_tree* x = *tr;
  while(x != NULL){
    y = x;
    if (strcmp (id->name, x->id->name) == 0){
      return 0;
    }
    if (compare(id->name, x->id->name) == 0)
      x = x->left;
    else
      x = x->right;
  }
  relation_tree* z;
  z = malloc(sizeof(relation_tree));
  z->left = NULL;
  z->right = NULL;
  z->id = id;
  z->p = y;
  if (y == NULL)
    *tr = z;
  else if (compare(id->name, y->id->name) == 0)
    y->left = z;
  else
    y->right = z;
  return 1;
}

int add_to_maxlist(tree* id, max_list** position){
    max_list* y = NULL;
  max_list* x = *position;
  while(x != NULL){
    y = x;
    if (strcmp (id->name, x->id->name) == 0){
      return 0;
    }
    if (compare(id->name, x->id->name) == 0)
      x = x->left;
    else
      x = x->right;
  }
  max_list* z;
  z = malloc(sizeof(max_list));
  z->left = NULL;
  z->right = NULL;
  z->id = id;
  z->p = y;
  if (y == NULL)
    *position = z;
  else if (compare(id->name, y->id->name) == 0)
    y->left = z;
  else
    y->right = z;
  return 1;
}

int clear_maxlist(max_list** position){
  if(*position != NULL){
    clear_maxlist(&((*position)->left));
    clear_maxlist(&((*position)->right));
    free(*position);
  }
}

void add_in_relation_tree(tree* id1, tree* id2, char rel[], list** position){
  int i = 0;
  int pos = 0;
  while (i<MAX_RELATION && id2->relation_name[i] != NULL && strcmp(id2->relation_name[i], rel) != 0){
      i++;
  }
  if (id2->relation_name[i] == NULL){
    (id2->relation_name)[i] = malloc(sizeof(char)*(strlen(rel)+1));
    strcpy((id2->relation_name)[i], rel);
    pos = i;
  }
  if(id2->relation_name[i] != NULL && strcmp(id2->relation_name[i], rel) == 0){
    pos = i;
  }
  int res = relation_tree_insert( &(id2->relation[i]), id1);

  if (res == 1){
    id2->occorrenze[pos]++;
    if (id2->occorrenze[pos]==(*position)->num_relation){
      add_to_maxlist(id2, (&(*position)->max_lista));
    }
    if (id2->occorrenze[pos]>(*position)->num_relation){
      (*position)->num_relation = id2->occorrenze[pos];
      clear_maxlist((&(*position)->max_lista));
      (*position)->max_lista=NULL;
      add_to_maxlist(id2, (&(*position)->max_lista));
    }
  }
}

int add_relation(char name1[], char name2[], char rel[], list** lista_relazioni, tree* primary_tree){
  tree* id1 = find_id(name1, primary_tree);
  tree* id2 = find_id(name2, primary_tree);
  if ( (id1 == NULL) || (id2 == NULL) ){
    return 0;
  }
  list* position;
  position = find_relation_global(rel, lista_relazioni);
  add_in_relation_tree(id1, id2, rel, &position);
  return 1;
}

/*** inizio funzioni per report ***/
void stampa_relation(relation_tree* t){
  if (t != NULL){
    stampa_relation(t->left);
    printf("||%s||", t->id->name);
    stampa_relation(t->right);
  }
}

void stampa_relazioni(char relation[], tree* t){
  if (t != NULL){
    stampa_relazioni(relation, t->left);
    printf("\nnome id2: %s\n", t->name);
    int i = 0;
    while(i<MAX_RELATION && t->relation_name[i] != NULL && (strcmp(t->relation_name[i], relation) != 0) )
      i++;
    stampa_relation(t->relation[i]);
    stampa_relazioni(relation, t->right);
  }
}

void report(list* lista, tree* primary_tree){
  while(lista != NULL){
    printf("\nrelazione: %s\n\n", lista->relation);
    stampa_relazioni(lista->relation, primary_tree);
    lista = lista->next;
  }
}

void stampa_albero(max_list* t){
  if (t != NULL){
    stampa_albero(t->left);
    printf("%s ", t->id->name);
    stampa_albero(t->right);
  }
}

void mostra(list* lista){
  while(lista != NULL){
    printf("%s ", lista->relation);
    stampa_albero(lista->max_lista);
    printf("%d; ", lista->num_relation);
    lista=lista->next;
  }
  printf("\n");
}


int main(){
  char line[LINE_LENGTH];
  char name1[NAME];
  char name2[NAME];
  char relation[NAME];
  tree* primary_tree = NULL;
  char *relation_name[MAX_RELATION] = {NULL};

  list* lista_relazioni;
  lista_relazioni = NULL;
  int cc;

  while(1){

    cc = scanf("%s", line);
    if (strncmp(line, "addent", 6) == 0){

      cc = scanf("%s", name1);
      add_entity(name1, &primary_tree);
    }
    if (strncmp(line, "addrel", 6) == 0){
      cc = scanf("%s", name1);
      cc = scanf("%s", name2);
      cc = scanf("%s", relation);
      add_relation(name1, name2, relation, &lista_relazioni, primary_tree);
    }
    if (strncmp(line, "delent", 6) == 0){
      char name1[NAME];
      cc = scanf("%s", name1);
      //del_entity(name1);
    }
    if (strncmp(line, "delrel", 6) == 0){
      cc = scanf("%s", name1);
      cc = scanf("%s", name2);
      cc = scanf("%s", relation);
      //del_relation(name1, name2, relation);
    }
    if (strncmp(line, "report", 6) == 0){
      if (lista_relazioni==NULL)
        printf("none\n");
      else
        mostra(lista_relazioni);
    }
    if (strncmp(line, "end", 3) == 0){
      exit(0);
    }
  }
  return 0;
}
