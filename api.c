/*controllare se quando elimino una entità che è unica si elimina anche relazione associata*/

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
  while (i<MAX_RELATION && id2->occorrenze[i] != 0 && strcmp(id2->relation_name[i], rel) != 0){
      i++;
  }
  if (id2->relation_name[i] == NULL || strcmp(id2->relation_name[i], "deleted") == 0){
    free((id2->relation_name)[i]);
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
    while(i<MAX_RELATION && t->occorrenze[i] != 0 && (strcmp(t->relation_name[i], relation) != 0) )
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

/*** inizio funzioni per delent ***/
tree* tree_minimum(tree* x){
  while(x->left != NULL){
    x = x->left;
  }
  return x;
}

tree* tree_successor(tree* x){
  if(x->right != NULL)
    return tree_minimum(x->right);
  tree* y = x->p;
  while(y != NULL && x == y->right){
    x = y;
    y = y->p;
  }
  return y;
}

void transplant(tree **root, tree* u, tree* v){
  if (u->p == NULL){
    *root = v;
  }
  else if(u == u->p->left)
    u->p->left = v;
  else
    u->p->right = v;
  if (v != NULL){
    v->p = u->p;
  }
}


relation_tree* tree_minimum_rel(relation_tree* x){
  while(x->left != NULL){
    x = x->left;
  }
  return x;
}

relation_tree* tree_successor_rel(relation_tree* x){
  if(x->right != NULL)
    return tree_minimum_rel(x->right);
  relation_tree* y = x->p;
  while(y != NULL && x == y->right){
    x = y;
    y = y->p;
  }
  return y;
}

void transplant_rel(relation_tree **root, relation_tree* u, relation_tree* v){
  if (u->p == NULL){
    *root = v;
  }
  else if(u == u->p->left)
    u->p->left = v;
  else
    u->p->right = v;
  if (v != NULL){
    v->p = u->p;
  }
}

relation_tree* find_id_in_relation(char id[], relation_tree* x){
  if(x == NULL){
    return NULL;
  }
  if( strcmp(id, x->id->name) == 0){
    return x;
    printf("find!\n");
  }
  if(compare(id, x->id->name)==0)
    return find_id_in_relation(id, x->left);
  else
    return find_id_in_relation(id, x->right);
}

int cancella_id_da_relazioni(relation_tree** root, tree* element){
  relation_tree* y;
  relation_tree* z;
  z = find_id_in_relation(element->name, *root);
  if (z==NULL)
    return 0;
  if (z->left == NULL){
    transplant_rel(root, z, z->right);
  }
  else if(z->right == NULL){
    transplant_rel(root, z, z->left);
  }
  else{
    y = tree_minimum_rel(z->right);
    if (y->p != z){
      transplant_rel(root, y, y->right);
      y->right = z->right;
      y->right->p = y;
    }
    transplant_rel(root, z, y);
    y->left = z->left;
    y->left->p = y;
  }
  free(z);
  if(*root==NULL)
    return 1;
  return 0;
}

void cancella_in_sottoalberi(tree* tr, tree* z){
  if (tr != NULL){
    cancella_in_sottoalberi(tr->left, z);
    cancella_in_sottoalberi(tr->right, z);
    int res;

    for(int i = 0; i<MAX_RELATION && tr->relation[i] != NULL; i++){
      if(strcmp(tr->relation_name[i], "deleted") != 0){
        res = cancella_id_da_relazioni(&(tr->relation[i]), z);
        if(res == 1){
          free(tr->relation_name[i]);
          //tr->relation_name[i] = NULL;
          tr->relation_name[i] = malloc(sizeof(char)*8);
          strcpy(tr->relation_name[i],"deleted");
          tr->occorrenze[i]=0;
        }
      }
    }
  }
}

max_list* cerca_in_max_lista(max_list* m_lista, tree* z){
  max_list* y = NULL;
  max_list* x = m_lista;
  while(x != NULL){
    y = x;
    if (strcmp (z->name, x->id->name) == 0){
      return x;
    }
    if (compare(z->name, x->id->name) == 0)
      x = x->left;
    else
      x = x->right;
  }
  return NULL;
}


max_list* tree_minimum_max_list(max_list* x){
  while(x->left != NULL){
    x = x->left;
  }
  return x;
}

max_list* tree_successor_max_list(max_list* x){
  if(x->right != NULL)
    return tree_minimum_max_list(x->right);
  max_list* y = x->p;
  while(y != NULL && x == y->right){
    x = y;
    y = y->p;
  }
  return y;
}

void transplant_max_list(max_list **root, max_list* u, max_list* v){
  if (u->p == NULL){
    *root = v;
  }
  else if(u == u->p->left)
    u->p->left = v;
  else
    u->p->right = v;
  if (v != NULL){
    v->p = u->p;
  }
}


int rimuovi(max_list* id_max_lista, max_list** root, list* elem, list** lista){
  max_list* y;
  max_list* z;
  z = id_max_lista;
  if (z->left == NULL){
    transplant_max_list(root, z, z->right);
  }
  else if(z->right == NULL){
    transplant_max_list(root, z, z->left);
  }
  else{
    y = tree_minimum_max_list(z->right);
    if (y->p != z){
      transplant_max_list(root, y, y->right);
      y->right = z->right;
      y->right->p = y;
    }
    transplant_max_list(root, z, y);
    y->left = z->left;
    y->left->p = y;
  }
  free(z);
  if(*root == NULL)
    return 0;
  else
    return 1;
  /** se il l'elemento eliminato era l'unico ad avere tale relazione, eliimino la reazione dalla lista **/
  if(*root == NULL && (elem)->num_relation == 1){
    list* temp = *lista;
    list* prev = NULL;
    while(temp != elem){
      prev = temp;
      temp = temp->next;
    }
    printf("prova: %s %s\n\n", temp->relation, elem->relation);
    if(prev == NULL){
      *lista = temp->next;
    }
    else{
      prev->next = temp->next;
    }
    printf("1- prev->next: %s\n", prev->next->relation);
    printf("2- temp->next: %s\n", temp->next->relation);
    free(temp);
    return 1;
  }

}

int max2(int a, int b){
  if(a>b)
    return a;
  return b;
}

int max3(int a, int b, int c){
  if(a>b){
    if(a>c)
      return a;
    return c;
  }
  if(b>a){
    if(b>c)
      return b;
    return c;
  }
}


int find_new_max(tree* root, char rel[], tree* z){
  int act;
  if (root != NULL){
    for(int i = 0; i<MAX_RELATION && root->occorrenze[i] != 0; i++){
      if ((strcmp(root->relation_name[i], rel) == 0) && (strcmp(root->relation_name[i], z->name) != 0) ){
        act = root->occorrenze[i];
      }
    }
    if(root->left != NULL && root->right != NULL){
      return max3(act, find_new_max(root->left, rel, z), find_new_max(root->right, rel, z));
    }
    if(root->left != NULL && root->right == NULL){
      return max2(act, find_new_max(root->left, rel, z));
    }
    if(root->left == NULL && root->right != NULL){
      return max2(act, find_new_max(root->right, rel, z));
    }
    if(root->left == NULL && root->right == NULL){
      for(int i = 0; i<MAX_RELATION && root->occorrenze[i] != 0; i++){
        if ( (strcmp(root->relation_name[i], rel) == 0) && (strcmp(root->relation_name[i], z->name) != 0) ){
          return root->occorrenze[i];
        }
      }
    }
  }
}

void ripopola(tree* root, list* elem, tree* z, int new_max){
  if(root != NULL){
    ripopola(root->left, elem, z, new_max);
    ripopola(root->right, elem, z, new_max);
    for(int i = 0; i<MAX_RELATION && root->occorrenze[i] != 0; i++){
      if ((strcmp(root->relation_name[i], elem->relation) == 0 ) && root->occorrenze[i] == new_max && root != z){
        elem->num_relation = new_max;
        add_to_maxlist(root, &(elem->max_lista));
      }
    }
  }
}

void ricarica_lista(tree* root, list* elem, tree* z){
  int new_max;
  new_max = find_new_max(root, elem->relation, z);
  ripopola(root, elem, z, new_max);
}

void aggiorna_lista_massimi(tree* root, list** lista, tree* z){
  list* temp = *lista;
  int  i = 1;
  int res;
  while(temp != NULL){
    max_list* id_max_lista = cerca_in_max_lista(temp->max_lista, z);

    if(id_max_lista != NULL)
      printf("numero: %d -- name_ent => %s\n", i, id_max_lista->id->name);
    if (id_max_lista != NULL){
      res = rimuovi(id_max_lista, &(temp->max_lista), temp, lista);
      if (res == 0){
        ricarica_lista(root, temp, z);
      }
    }
    i++;
    temp = temp->next;
  }
}

void stampa_id_relazioni_test(relation_tree* rel_name){
  if(rel_name != NULL){
    stampa_id_relazioni_test(rel_name->left);
    printf("%s |", rel_name->id->name);
    stampa_id_relazioni_test(rel_name->right);
  }
}

void stampa_sottoalberi_test(tree* t){
  if (t != NULL){
    stampa_sottoalberi_test(t->left);
    printf("\n>>>principal %s:\n", t->name);
    for(int i=0; i<MAX_RELATION && t->relation_name[i] != NULL; i++){
      printf("\n--%s\n", t->relation_name[i]);
      stampa_id_relazioni_test(t->relation[i]);
    }
    stampa_sottoalberi_test(t->right);
  }
}

void del_entity(char name[], tree** root, list** lista){
  tree* z = find_id(name, *root);
  tree* y;
  if (z->left == NULL){
    transplant(root, z, z->right);
  }
  else if(z->right == NULL){
    transplant(root, z, z->left);
  }
  else{
    y = tree_minimum(z->right);
    if (y->p != z){
      transplant(root, y, y->right);
      y->right = z->right;
      y->right->p = y;
    }
    transplant(root, z, y);
    y->left = z->left;
    y->left->p = y;
  }
  cancella_in_sottoalberi(*root, z);
  aggiorna_lista_massimi(*root, lista, z);
  free(z);
}

void print_tree(tree* tre){
  if(tre != NULL){
    print_tree(tre->left);
    printf("%s\n", tre->name);
    print_tree(tre->right);
  }
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
      del_entity(name1, &primary_tree, &lista_relazioni);
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
      else{
        printf("======\n");
        //print_tree(primary_tree);
        stampa_sottoalberi_test(primary_tree);
        printf("\n======\n");
        //mostra(lista_relazioni);
      }
    }
    if (strncmp(line, "end", 3) == 0){
      //print_tree(primary_tree);
      exit(0);
    }
  }
  return 0;
}
