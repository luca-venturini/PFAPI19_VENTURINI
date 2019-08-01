#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 35
#define MAX_RELATION 30
#define NAME 256

struct list_t;
struct primary_tree_t;


int rr = 1;
typedef struct list_t list;
typedef struct primary_tree_t tree;
typedef struct relation_tree_t relation_tree;
typedef struct max_l_t max_list;
int counter = 0;
struct max_l_t{
  struct max_l_t* p;
  tree* id;
  struct max_l_t* left;
  struct max_l_t* right;
  char color;
};

struct list_t{
  char relation[NAME];
  short unsigned num_relation;
  max_list* max_lista;
  struct list_t* next;
};

struct relation_tree_t{
  struct relation_tree_t* p;
  char color;
  tree* id;
  struct relation_tree_t* left;
  struct relation_tree_t* right;
};

struct primary_tree_t{
  struct primary_tree_t* p;
  char name[NAME];
  relation_tree* relation[MAX_RELATION];
  char *relation_name[MAX_RELATION];
  short unsigned occorrenze[MAX_RELATION];
  char color;
  struct primary_tree_t* left;
  struct primary_tree_t* right;
};

max_list* cerca_in_max_albero(max_list* m_lista, tree* z);
int rimuovi(max_list* id_max_lista, max_list** root, list* elem, list** lista);
int ricarica_lista(tree* root, list* elem, tree* z);
void stampa_id_relazioni_test(relation_tree* rel_name, int *co);

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


/*** inizio funzioni per addent ***/
void left_rotate(tree** root, tree* x){
  tree* y;
  y = x->right;
  x->right = y->left;

  if(y->left != NULL)
    y->left->p = x;
  y->p = x->p;
  if(x->p == NULL)
    *root = y;
  else if(x == x->p->left)
    x->p->left = y;
  else
    x->p->right = y;
  y->left = x;
  x->p = y;
}

void left_rotate_relation(relation_tree** root, relation_tree* x){
  relation_tree* y;
  y = x->right;
  x->right = y->left;

  if(y->left != NULL)
    y->left->p = x;
  y->p = x->p;
  if(x->p == NULL)
    *root = y;
  else if(x == x->p->left)
    x->p->left = y;
  else
    x->p->right = y;
  y->left = x;
  x->p = y;
}

void left_rotate_maxlist(max_list** root, max_list* x){
  max_list* y;
  y = x->right;
  x->right = y->left;

  if(y->left != NULL)
    y->left->p = x;
  y->p = x->p;
  if(x->p == NULL)
    *root = y;
  else if(x == x->p->left)
    x->p->left = y;
  else
    x->p->right = y;
  y->left = x;
  x->p = y;
}

void right_rotate(tree** root, tree* x){
  tree* y;
  y = x->left;
  x->left = y->right;

  if(y->right != NULL)
    y->right->p = x;
  y->p = x->p;
  if(x->p == NULL)
    *root = y;
  else if(x == x->p->left)
    x->p->left = y;
  else
    x->p->right = y;
  y->right = x;
  x->p = y;
}

void right_rotate_relation(relation_tree** root, relation_tree* x){
  relation_tree* y;
  y = x->left;
  x->left = y->right;

  if(y->right != NULL)
    y->right->p = x;
  y->p = x->p;
  if(x->p == NULL)
    *root = y;
  else if(x == x->p->left)
    x->p->left = y;
  else
    x->p->right = y;
  y->right = x;
  x->p = y;
}

void right_rotate_maxlist(max_list** root, max_list* x){
  max_list* y;
  y = x->left;
  x->left = y->right;

  if(y->right != NULL)
    y->right->p = x;
  y->p = x->p;
  if(x->p == NULL)
    *root = y;
  else if(x == x->p->left)
    x->p->left = y;
  else
    x->p->right = y;
  y->right = x;
  x->p = y;
}

void rb_insert_fixup(tree** root, tree* z){
  tree* y;
  tree* x;
  while(z->p != NULL && z->p->p != NULL && z->p->color == 1){
    if(z->p == z->p->p->left){
      y = z->p->p->right;
      if(y != NULL && y->color == 1){
        z->p->color = 0;
        y->color = 0;
        z->p->p->color = 1;
        z = z->p->p;
      }
      else{
        if(z == z->p->right){
          z = z->p;
          left_rotate(root, z);
        }
        z->p->color = 0;
        z->p->p->color = 1;
        right_rotate(root, z->p->p);
      }
    }
    else{
      y = z->p->p->left;
      if(y != NULL && y->color == 1){
        z->p->color = 0;
        y->color = 0;
        z->p->p->color = 1;
        z = z->p->p;
      }
      else{
        if(z == z->p->left){
          z = z->p;
          right_rotate(root, z);
        }
        z->p->color = 0;
        z->p->p->color = 1;
        left_rotate(root, z->p->p);
      }
    }
  }
  (*root)->color = 0;
}


void rb_insert_fixup_relation(relation_tree** root, relation_tree* z){
  relation_tree* y;
  relation_tree* x;
  while(z->p != NULL && z->p->p != NULL && z->p->color == 1){
    if(z->p == z->p->p->left){
      y = z->p->p->right;
      if(y != NULL && y->color == 1){
        z->p->color = 0;
        y->color = 0;
        z->p->p->color = 1;
        z = z->p->p;
      }
      else{
        if(z == z->p->right){
          z = z->p;
          left_rotate_relation(root, z);
        }
        z->p->color = 0;
        z->p->p->color = 1;
        right_rotate_relation(root, z->p->p);
      }
    }
    else{
      y = z->p->p->left;
      if(y != NULL && y->color == 1){
        z->p->color = 0;
        y->color = 0;
        z->p->p->color = 1;
        z = z->p->p;
      }
      else{
        if(z == z->p->left){
          z = z->p;
          right_rotate_relation(root, z);
        }
        z->p->color = 0;
        z->p->p->color = 1;
        left_rotate_relation(root, z->p->p);
      }
    }
  }
  (*root)->color = 0;
}

void rb_insert_fixup_maxlist(max_list** root, max_list* z){
  max_list* y;
  max_list* x;
  while(z->p != NULL && z->p->p != NULL && z->p->color == 1){
    if(z->p == z->p->p->left){
      y = z->p->p->right;
      if(y != NULL && y->color == 1){
        z->p->color = 0;
        y->color = 0;
        z->p->p->color = 1;
        z = z->p->p;
      }
      else{
        if(z == z->p->right){
          z = z->p;
          left_rotate_maxlist(root, z);
        }
        z->p->color = 0;
        z->p->p->color = 1;
        right_rotate_maxlist(root, z->p->p);
      }
    }
    else{
      y = z->p->p->left;
      if(y != NULL && y->color == 1){
        z->p->color = 0;
        y->color = 0;
        z->p->p->color = 1;
        z = z->p->p;
      }
      else{
        if(z == z->p->left){
          z = z->p;
          right_rotate_maxlist(root, z);
        }
        z->p->color = 0;
        z->p->p->color = 1;
        left_rotate_maxlist(root, z->p->p);
      }
    }
  }
  (*root)->color = 0;
}

int hashing(char rel[]){
  int tot = 0;
  for (int i = 0; rel[i] != '\0'; i++){
    tot = tot + rel[i];
  }
  return ((int) (tot%773));
}

int add_entity(char* name, tree** primary_tree){
  tree* y = NULL;
  tree* x = *primary_tree;
  int lungh;
  while(x != NULL){
    lungh = strcmp(name, x->name);
    y = x;
    if (lungh == 0){
      return 0;
    }
    if (lungh < 0)
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
  else if (strcmp(name, y->name) < 0)
    y->left = z;
  else
    y->right = z;
  z->color = 1;
  rb_insert_fixup(primary_tree, z);
  return 1;
}


/*** inizio funzioni per addrel ***/
tree* find_id(char id[], tree* x){
  if(x == NULL){
    return NULL;
  }
  int lungh = strcmp(id, x->name);
  if( lungh == 0){
    return x;
  }
  if(lungh < 0)
    return find_id(id, x->left);
  else
    return find_id(id, x->right);
}

list* find_relation_global(char rel[], list** lista){

  list* temp;
  list* temp_prev;
  temp_prev = NULL;
  temp = *lista;
  while( temp != NULL && strcmp(temp->relation, rel) < 0){
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
  n->max_lista = NULL; /********** agginuta riga per evitare memory leak**/
  if(temp != NULL)
    n->next = temp;
  else
    n->next = NULL;

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
  int lungh;
  while(x != NULL){
    lungh = strcmp (id->name, x->id->name);
    y = x;
    if (lungh == 0){
      return 0;
    }
    if (lungh < 0)
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
  else if (strcmp(id->name, y->id->name) < 0)
    y->left = z;
  else
    y->right = z;
  z->color = 1;
  rb_insert_fixup_relation(tr, z);
  return 1;
}

int add_to_maxlist(tree* id, max_list** position){
  max_list* y = NULL;
  max_list* x = *position;
  int lungh;
  while(x != NULL){
    lungh = strcmp (id->name, x->id->name);
    y = x;
    if (lungh == 0){
      return 0;
    }
    if (lungh < 0)
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
  else if (strcmp(id->name, y->id->name) < 0)
    y->left = z;
  else
    y->right = z;
  z->color = 1;
  rb_insert_fixup_maxlist(position, z);
  return 1;
}

void clear_maxlist(max_list* position){
  if(position == NULL)
    return;
  clear_maxlist(position->left);
  clear_maxlist(position->right);
  free(position);
}

void add_in_relation_tree(tree* id1, tree* id2, char rel[], list** position){
  int i = 0;
  int j = -1;
  int k = -1;
  int pos = 0;
  while (i<MAX_RELATION && id2->relation_name[i] != NULL){
    if(strcmp(id2->relation_name[i], "deleted") == 0 && j == -1)
      j = i;
    if(  strcmp(id2->relation_name[i], rel) == 0 && k == -1){
      k = i;
      break;
    }
    i++;
  }
  if( j != -1)
    i = j;
  if (k != -1)
    i = k;
  if (id2->relation_name[i] == NULL || j == i){ /**** modifica pericolosa*/
    (id2->relation_name)[i] = realloc((id2->relation_name)[i], sizeof(char)*(strlen(rel)+1));
    strcpy((id2->relation_name)[i], rel);
    pos = i;
  }
  if(id2->relation_name[i] != NULL && k == i){  /**** modifica pericolosa*/
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
      clear_maxlist(((*position)->max_lista));
      (*position)->max_lista=NULL;
      add_to_maxlist(id2, (&(*position)->max_lista));
    }
  }
}

void trova_id(tree** id1, tree** id2, char name1[], char name2[], tree* root){
  while(root != NULL){
    if( strcmp(name2, root->name) < 0 )
      root = root->left;
    else if( strcmp(name1, root->name) > 0 )
      root = root->right;
    else{
      *id1 = find_id(name1, root);
      if (*id1 != NULL)
        *id2 = find_id(name2, root);
      return;
    }
  }
  *id1 = NULL;
  *id2 = NULL;
}
int add_relation(char name1[], char name2[], char rel[], list** lista_relazioni, tree* primary_tree){
  tree *id1, *id2;
  if( strcmp(name1, name2) < 0 )
    trova_id(&id1, &id2, name1, name2, primary_tree);
  else
    trova_id(&id2, &id1, name2, name1, primary_tree);

  if(id1 == NULL)
    return 0;

  if(id2 == NULL)
    return 0;

  list* position;
  position = find_relation_global(rel, lista_relazioni);
  add_in_relation_tree(id1, id2, rel, &position);
  return 1;
}

/*** inizio funzioni per report ***/

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

relation_tree* find_id_in_relation(tree* element, relation_tree* root){
  while(root != NULL){
    if(strcmp(root->id->name, element->name) == 0)
      return root;
    if(compare( element->name, root->id->name ) == 0)
      root = root->left;
    else
      root = root->right;
  }
  return NULL;
}

int cancella_id_da_relazioni(relation_tree** root, tree* element){
  relation_tree* y;
  relation_tree* z;
  z = find_id_in_relation(element, *root);
  if (z==NULL)
    return -1;
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
  if(*root == NULL){
    return 1;
  }
  return 0;
}

void cancella_in_sottoalberi(tree* root, tree* tr, tree* z, list **lista){
  if (tr != NULL){
    cancella_in_sottoalberi(root, tr->left, z, lista);
    cancella_in_sottoalberi(root, tr->right, z, lista);
    int res, result;
    for(int i = 0; i<MAX_RELATION; i++){
      if(tr->relation[i] != NULL && strcmp(tr->relation_name[i], "deleted") != 0){
        result = cancella_id_da_relazioni(&(tr->relation[i]), z);
        if (result != -1){
          tr->occorrenze[i]--;
          list* temp = *lista;
          list* prev = NULL;
          while(temp != NULL){
            if(tr->occorrenze[i] == (temp->num_relation-1) && strcmp (tr->relation_name[i], temp->relation)==0 ){
              max_list* id_max_lista = cerca_in_max_albero(temp->max_lista, tr);
              res = rimuovi(id_max_lista, &(temp->max_lista), temp, lista);
              if (res == 0){
                res = ricarica_lista(root, temp, NULL);
              }
              if(res == 2){
                if(prev == NULL){
                  *lista = temp->next;
                }
                else{
                  prev->next = temp->next;
                }
                free(temp);
                if(prev != NULL)
                  temp = prev;
                else
                  temp = *lista;
              }
            }
            prev = temp;
            temp = temp->next;
          }
        }
        if(result == 1){
          free(tr->relation_name[i]);
          tr->relation_name[i] = malloc(sizeof(char)*8);
          strcpy(tr->relation_name[i],"deleted");
          tr->occorrenze[i] = 0;
          tr->relation[i] = NULL;
        }
      }
    }
  }
}

max_list* cerca_in_max_albero(max_list* m_lista, tree* z){
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
  if ( (*root == NULL) && (elem->num_relation != 1) )
    return 0;
  if(*root != NULL)
    return 1;
  /** se l'elemento eliminato era l'unico ad avere tale relazione, elimino la reazione dalla lista **/
  if(*root == NULL && elem->num_relation == 1){
    return 2;
  }

}

int max2(int a, int b){
  if(a>b)
    return a;
  return b;
}

int max3(int a, int b, int c){
  if(a>=b){
    if(a>=c)
      return a;
    return c;
  }
  if(b>=a){
    if(b>=c)
      return b;
    return c;
  }
}

int find_new_max(tree* root, char rel[], tree* z){
  int tot, a, b;
  if(root == NULL)
    return 0;
  a = find_new_max(root->left, rel, z);
  b = find_new_max(root->right, rel, z);
  for(int i = 0; i<MAX_RELATION && root->relation_name[i] != NULL; i++){
    if ( (strcmp(root->relation_name[i], rel) == 0) && (z == NULL) ){
      return max3(a, b, root->occorrenze[i]);
    }
    if ( (strcmp(root->relation_name[i], rel) == 0) && root != z ){
      return max3(a, b, root->occorrenze[i]);
    }
  }
  return max2(a, b);
}

void ripopola(tree* root, list* elem, tree* z, int new_max){
  if(root != NULL){
    ripopola(root->left, elem, z, new_max);
    ripopola(root->right, elem, z, new_max);
    for(int i = 0; i<MAX_RELATION && root->relation_name[i] != NULL; i++){
      if( strcmp(root->relation_name[i], "deleted") != 0){
        if ((strcmp(root->relation_name[i], elem->relation) == 0 ) && root->occorrenze[i] == new_max && root != z){
          elem->num_relation = new_max;
          add_to_maxlist(root, &(elem->max_lista));
        }
      }
    }
  }
}

int ricarica_lista(tree* root, list* elem, tree* z){
  int new_max=0;
  new_max = find_new_max(root, elem->relation, z);
  if (new_max == 0)
    return 2;
  ripopola(root, elem, z, new_max);
  return 0;
}

void aggiorna_lista_massimi(tree* root, list** lista, tree* z){
  list* temp = *lista;
  list* prev = NULL;
  int  i = 1;
  int res;
  while(temp != NULL){
    max_list* id_max_lista = cerca_in_max_albero(temp->max_lista, z);
    rr++;
    if (id_max_lista != NULL){
      res = rimuovi(id_max_lista, &(temp->max_lista), temp, lista);
      if (res == 0){
        res = ricarica_lista(root, temp, z);
      }
      if(res == 2){
        if(prev == NULL){
          *lista = temp->next;
        }
        else{
          prev->next = temp->next;
        }
        free(temp);
        if(prev != NULL)
          temp = prev;
        else
          temp = *lista;
      }
    }
    i++;
    prev = temp;
    temp = temp->next;
  }
}

void stampa_id_relazioni_test(relation_tree* rel_name, int* co){
  if(rel_name != NULL){
    stampa_id_relazioni_test(rel_name->left, co);
    printf("%s |", rel_name->id->name);
    *co = *co +1;
    stampa_id_relazioni_test(rel_name->right, co);
  }
}

void stampa_sottoalberi_test(tree* t){
  if (t != NULL){
    stampa_sottoalberi_test(t->left);
    printf("\n>>>principal %s:\n", t->name);
    for(int i=0; i<MAX_RELATION && t->relation_name[i] != NULL; i++){
      printf("\n--%s--%d\n", t->relation_name[i], t->occorrenze[i]);
      int kk = 0;
      stampa_id_relazioni_test(t->relation[i], &kk);
      if(kk != t->occorrenze[i]){
        printf("valore: %d ", kk);
        printf("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
        exit(0);
      }
    }
    stampa_sottoalberi_test(t->right);
  }
}

relation_tree* cerca(relation_tree* root, tree* z){
  while(root != NULL){
    if(strcmp(root->id->name, z->name) == 0)
      return root;
    if(compare( z->name, root->id->name ) == 0)
      root = root->left;
    else
      root = root->right;
  }
  return NULL;
}

void ispection(tree* root, tree* z){
  relation_tree* pp;
  if(root != NULL){

    for(int i = 0; i<MAX_RELATION && root->relation_name[i] != NULL; i++){
      pp = cerca(root->relation[i], z);
      if(pp != NULL)
        printf("eee %s eee\n", pp->id->name);
    }
    ispection(root->left, z);
    ispection(root->right, z);
  }
}

void rb_delete_fixup(tree** root, tree* x){
  tree *w;
  while(x != *root && (x == NULL || x->color == 0) ){
    if(x == x->p->left){
      w = x->p->right;
      if(w != NULL && w->color == 1){
        w->color = 0;
        x->p->color = 1;
        left_rotate(root, x->p);
        w = x->p->right;
      }
      if(w != NULL && (w->left==NULL || w->left->color == 0) && (w->right==NULL || w->right->color == 0) ){
        w->color = 1;
        x = x->p;
      }
      else{
        if(w != NULL && (w->right == NULL || w->right->color == 0) ){
          if (w->left != NULL)
            w->left->color = 0;
          w->color = 1;
          right_rotate(root, w);
          w = x->p->right;
        }
        if(w != NULL)
          w->color = x->p->color;
        x->p->color = 0;
        if(w != NULL && w->right != NULL)
          w->right->color = 0;
        left_rotate(root, x->p);
        x = (*root);
      }
    }
    else{
      w = x->p->left;
      if(w != NULL && w->color == 1){
        w->color = 0;
        x->p->color = 1;
        right_rotate(root, x->p);
        w = x->p->left;
      }
      if(w != NULL && (w->left==NULL || w->left->color == 0) && (w->right==NULL || w->right->color == 0) ){
        w->color = 1;
        x = x->p;
      }
      else{
        if(w != NULL && (w->left == NULL || w->left->color == 0) ){
          if (w->right != NULL)
            w->right->color = 0;
          w->color = 1;
          left_rotate(root, w);
          w = x->p->left;
        }
        if(w != NULL)
          w->color = x->p->color;
        x->p->color = 0;
        if(w != NULL && w->left != NULL)
          w->left->color = 0;
        right_rotate(root, x->p);
        x = (*root);
      }
    }
  }
  if(x != NULL)
    x->color = 0;
}


/*eliminare il sottoalbero legato al nodo che cancello***/
void elimina_sottoalbero(relation_tree* x){
  if(x == NULL)
    return;
  elimina_sottoalbero(x->left);
  elimina_sottoalbero(x->right);
  free(x);
}

void cancella_sottoalbero(tree* x){
  /*
  for(int i = 0; i<MAX_RELATION; i++){
    free(x->relation_name[i]);
    x->relation_name[i]=NULL;
  }*/
  for(int i = 0; i<MAX_RELATION; i++){
    elimina_sottoalbero(x->relation[i]);
    x->relation[i] = NULL;
  }

}

/*** fine ***/

void del_entity(char name[], tree** root, list** lista){
  tree* z = find_id(name, *root);
  tree* y;
  tree* x;
  if (z == NULL)
    return;
  y = z;
  char original_color = y->color;
  if (z->left == NULL){
    x = z->right;
    transplant(root, z, z->right);
  }
  else if(z->right == NULL){
    x = z->left;
    transplant(root, z, z->left);
  }
  else{
    y = tree_minimum(z->right);
    original_color = y->color;
    x = y->right;
    if (y->p == z){
      if(x != NULL){
        x->p = y;
      }
    }
    else{
      transplant(root, y, y->right);
      y->right = z->right;
      y->right->p = y;
    }
    transplant(root, z, y);
    y->left = z->left;
    y->left->p = y;
    y->color = z->color;
  }
  if(original_color == 0){
    //rb_delete_fixup(root, x);
  }
  cancella_in_sottoalberi(*root, *root, z, lista);
  aggiorna_lista_massimi(*root, lista, z);
  for(int i = 0; i<MAX_RELATION && z->relation_name[i] != NULL; i++){
    free(z->relation_name[i]);
    z->relation_name[i] = NULL;
  }
  cancella_sottoalbero(z);
  free(z);
}

void print_tree(tree* tre){
  if(tre != NULL){
    print_tree(tre->left);
    printf("%s\n", tre->name);
    print_tree(tre->right);
  }
}
/*** inizio funzioni per delrel ***/
relation_tree* cerca_in_albero_relazioni(char element[], relation_tree* root){
  while(root != NULL){
    if(strcmp(root->id->name, element) == 0)
      return root;
    if(compare( element, root->id->name ) == 0)
      root = root->left;
    else
      root = root->right;
  }
  return NULL;
}

int clean_up(relation_tree** root, relation_tree* z){
  relation_tree* y;
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
  if(*root == NULL){
    return 1;
  }
  return 0;
}

void del_relation(char name1[], char name2[], char relation[], tree* t, list** lista){
  tree* id2 = find_id(name2, t);
  tree* t_id1 = find_id(name1, t);
  if (id2 == NULL || t_id1 == NULL)
    return;
  int i;
  for (i = 0; i<MAX_RELATION && id2->relation_name[i] != NULL; i++){
    if (strcmp(id2->relation_name[i], relation) == 0)
      break;
  }
  relation_tree* id1 = cerca_in_albero_relazioni(name1, id2->relation[i]);
  if (id1 == NULL)
    return;
  int result;
  result = clean_up(&(id2->relation[i]), id1);
  if (id2->occorrenze[i] == 1){
    free(id2->relation_name[i]);
    id2->relation_name[i] = malloc(sizeof(char)*8);
    strcpy(id2->relation_name[i], "deleted");
    id2->relation[i] = NULL;
  }
  id2->occorrenze[i]--;

  list* temp;
  temp = *lista;
  list* prev = NULL;
  while(temp != NULL){
    if (strcmp(temp->relation, relation)==0 )
      break;
    prev = temp;
    temp = temp->next;
  }
  if(temp->num_relation == ((id2->occorrenze[i])+1) ){
    max_list* id_max_lista = cerca_in_max_albero(temp->max_lista, id2);
    int res = rimuovi(id_max_lista, &(temp->max_lista), temp, lista);
    if (res == 0){
      res = ricarica_lista(t, temp, NULL);
    }
    if(res == 2){
      if(prev == NULL){
        *lista = temp->next;
      }
      else{
        prev->next = temp->next;
      }
      free(temp);
      if(prev != NULL)
        temp = prev;
      else
        temp = *lista;
    }
  }
}


int main(){
  char name1[NAME];
  char name2[NAME];
  char relation[NAME];
  tree* primary_tree = NULL;
  char *relation_name[MAX_RELATION] = {NULL};
  char line[LINE_LENGTH];

  list* lista_relazioni;
  lista_relazioni = NULL;
  int cc;

  while(1){

    cc = scanf("%s", line);
    if (line[0] == 'a' && line[5] == 't'){
      cc = scanf("%s", name1);
      add_entity(name1, &primary_tree);
    }
    else if (line[0] == 'a' && line[5] == 'l'){
      cc = scanf("%s %s %s", name1, name2, relation);
      add_relation(name1, name2, relation, &lista_relazioni, primary_tree);
    }
    else if (line[0] == 'd' && line[5] == 't'){
      cc = scanf("%s", name1);
      del_entity(name1, &primary_tree, &lista_relazioni);
    }
    else if (line[0] == 'd' && line[5] == 'l'){
      cc = scanf("%s", name1);
      cc = scanf("%s", name2);
      cc = scanf("%s", relation);
      del_relation(name1, name2, relation, primary_tree, &lista_relazioni);
    }
    else if (line[0] == 'r'){
      if (lista_relazioni==NULL){
        printf("none\n");
      }else{
        mostra(lista_relazioni);
      }
    }
    else if (line[0] == 'e'){
      exit(0);
    }
  }
  return 0;
}
