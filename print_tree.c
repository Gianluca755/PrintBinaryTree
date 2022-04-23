/*

Archived Sources: http://web.archive.org/web/20090617110918/http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

http://web.archive.org/web/20071224095835/http://www.openasthra.com:80/wp-content/uploads/2007/12/binary_trees1.c

Stackoverflow: https://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console

GitHub gist_1: https://gist.github.com/ximik777/e04e5a9f0548a2f41cb09530924bdd9a

GitHub gist_2: https://gist.github.com/ryangraham/0e3c9af79e4901dee1ce956a07cbccd2

GitHub repo_1: https://github.com/edmlin/PrintAsciiTree

GitHub repo_2: https://github.com/nivpgir/ascii_tree_print

*/


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEIGHT 1000
#define INFINITY (1<<20) // Take 1 and shift 20 places, == to 1048576. MAX_INT ??


//////////////////////// Structures ///////////////////////

typedef struct Tree Tree;

struct Tree
{
  int element; // Value of the node
  Tree* left;  // Left subtree
  Tree* right; // Right subtree
};


typedef struct Asciinode_struct Asciinode;

struct Asciinode_struct
{
  Asciinode* left;
  Asciinode* right;

  // Length of the edge from this node to its children.
  int edge_length;
  int height;
  int lablen; // label's length: chars needed for printing the node's value

  /* Specify the location of the child node with respect to the parent.
     -1 = the node is a left child, 1 the node is a right child,
     0 = the node is the root of the tree.
     Better use enum. */
  int parent_dir;

  char label[11]; // Max supported unit32 in dec, 10 digits max
};


//////////////// !! Global variables !! ///////////////////

int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];

// Adjust gap between left and right nodes.
int gap = 3;

/* Used for printing next node in the same level,
   this is the x coordinate of the next char printed */
int print_next;


///////////////////// Tree functions //////////////////////

Tree* insert(int value, Tree* t);
Tree* delete(int value, Tree* t);
Tree* make_empty(Tree* t); // Delete the tree.
Tree* find(int elem, Tree* t);
Tree* find_min(Tree* t);
Tree* find_max(Tree* t);



///////////////////// Ascii Tree functions ////////////////
Asciinode* build_ascii_tree(Tree* t);
Asciinode* build_ascii_tree_recursive(Tree* t); // Auxiliary function
void free_ascii_tree(Asciinode* node);
void compute_lprofile(Asciinode* node, int x, int y);
void compute_rprofile(Asciinode* node, int x, int y);
void compute_edge_lengths(Asciinode* node);
void print_level(Asciinode* node, int x, int level);
void print_ascii_tree(Tree* t);


//////////////////// Other functions //////////////////////
int min (int x, int y);
int max (int x, int y);


//////////////////////// Main /////////////////////////////
int main(void)
{
  Tree* root = NULL;

  printf("\nAfter inserting element 10..\n");
  root = insert(10, root);
  print_ascii_tree(root);

  printf("\nAfter inserting element 5..\n");
  root = insert(5, root);
  print_ascii_tree(root);

  printf("\nAfter inserting element 15..\n");
  root = insert(15, root);
  print_ascii_tree(root);

  printf("\nAfter inserting elements 9, 13..\n");
  root = insert(9, root);
  root = insert(13, root);
  print_ascii_tree(root);

  printf("\nAfter inserting elements 2, 6, 12, 14..\n");
  root = insert(2, root);
  root = insert(6, root);
  root = insert(12, root);
  root = insert(14, root);
  print_ascii_tree(root);


  printf("\n\nAfter deleting a node (14) with no child..\n");
  root = delete(14, root);
  print_ascii_tree(root);

  printf("\n\nAfter deleting a node (13) with left child..\n");
  root = delete(13, root);
  print_ascii_tree(root);

  printf("\n\nAfter deleting a node (5) with left and right children..\n");
  root = delete(5, root);
  print_ascii_tree(root);

  printf("\n\nAfter deleting a node (10, root node) with left and right children..\n");
  root = delete(10, root);
  print_ascii_tree(root);

  make_empty(root);
}



///////////////////// Tree functions //////////////////////

// Recursive erase and deallocate the tree.
Tree* make_empty(Tree* t)
{
  if (t != NULL) {
    make_empty(t->left);
    make_empty(t->right);
    free(t);
  }

  return NULL;
}


// If the tree is ordered find the min, leftmost leaf.
Tree* find_min(Tree* t)
{
  if (t == NULL) {
    return NULL;
  }
  else if (t->left == NULL) {
    return t;
  }
  else {
    return find_min(t->left);
  }
}


// If the tree is ordered find the min, leftmost leaf.
Tree* find_max(Tree* t)
{
  if (t == NULL) {
    return NULL;
  }
  else if (t->right == NULL) {
    return t;
  }
  else {
    return find_max(t->right);
  }
}


// Rind node with value elem in ordered binary tree t.
Tree* find(int elem, Tree* t)
{
  if (t == NULL) {
    return NULL;
  }

  if (elem < t->element) {
    return find(elem, t->left);
  }
  else if (elem > t->element) {
    return find(elem, t->right);
  }
  else {
    return t;
  }
}


// Insert value into the tree t, a duplicate will be discarded.
// Return a pointer to the resulting tree.
Tree* insert(int value, Tree* t)
{

  if (t == NULL) {
    Tree* new_node = (Tree*) malloc (sizeof (Tree));

    if (new_node == NULL) {
      fprintf(stderr, "Failed memory allocation in insert function.\n");
      exit(EXIT_FAILURE);
    }

    new_node->element = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
  }


  if (value < t->element) {
    t->left = insert(value, t->left);
  }
  if (value > t->element) {
    t->right = insert(value, t->right);
  }

  // In case of duplicate value, no insert.
  return t;
}


// Deletes node from a binary ordered tree.
// Return a pointer to the resulting tree or NULL.
Tree* delete(int value, Tree* t)
{
  Tree* tmp = NULL;

  if (t == NULL){ // Base case of when empty leaf or the value is not in the tree.
    return NULL;
  }
  if (value < t->element) { // Recusion on left subtree.
    t->left = delete(value, t->left);
  }
  else if (value > t->element) { // Recusion on right subtree.
    t->right = delete(value, t->right);
  }
  else if (t->left != NULL && t->right != NULL) { // Node located and it has 2 children.
    tmp = find_min(t->right);  // Find smaller element after this node (value).
    t->element = tmp->element; // Overwrite value in current node
    t->right = delete(tmp->element, t->right);
  }
  else { // Node located but it doesn't have 2 children.
    tmp = t; // t will be used as return value.
    if (t->right != NULL) { // It has only right subtree.
      t = t->right;
    }
    else if (t->left != NULL) { // It has only only left subtree.
      t = t->left;
    }
    else {      // The node is a leaf.
      t = NULL; // The return value of the function.
    }
    free(tmp);
  }

  return t;
}


///////////////////// Ascii Tree functions ////////////////

// Create ascii tree from normal binary tree.
Asciinode* build_ascii_tree(Tree* t)
{
  if (t == NULL) {
    return NULL;
  }

  Asciinode* node = build_ascii_tree_recursive(t);
  node->parent_dir = 0; // Set the node as root of the tree.

  return node;
}

// Auxiliary function for building the ascii tree.
Asciinode* build_ascii_tree_recursive(Tree* t)
{
  if (t == NULL) {
    return NULL;
  }

  Asciinode* node = malloc(sizeof(Asciinode));
  if (node == NULL) {
    fprintf(stderr, "Failed memory allocation in build_ascii_tree_recursive function.\n");
    exit(EXIT_FAILURE);
  }

  node->left = build_ascii_tree_recursive(t->left);
  node->right = build_ascii_tree_recursive(t->right);

  if (node->left != NULL) {
    node->left->parent_dir = -1;
  }

  if (node->right != NULL) {
    node->right->parent_dir = 1;
  }

  sprintf(node->label, "%d", t->element);
  node->lablen = strlen(node->label);

  return node;
}

// Free all the nodes of the given ascii tree.
void free_ascii_tree(Asciinode* node)
{
  if (node == NULL) {
    return;
  }
  free_ascii_tree(node->left);
  free_ascii_tree(node->right);
  free(node);
}

/* The following function fills in the lprofile array for the given tree.
   It assumes that the center of the label of the root of this tree
   is located at a position (x,y).  It assumes that the edge_length
   fields have been computed for this tree. */
void compute_lprofile(Asciinode* node, int x, int y)
{
  int isleft;
  if (node == NULL) {
    return;
  }

  isleft = (node->parent_dir == -1);
  lprofile[y] = min(lprofile[y], x - ( (node->lablen - isleft) / 2) );

  if (node->left != NULL) {
    for (int i = 1; i <= node->edge_length && y+i < MAX_HEIGHT; i++) {
      lprofile[y+i] = min(lprofile[y+i], x-i);
    }
  }

  compute_lprofile(node->left, x - node->edge_length -1, y + node->edge_length +1);
  compute_lprofile(node->right, x + node->edge_length +1, y + node->edge_length +1);
}

void compute_rprofile(Asciinode* node, int x, int y)
{
  int notleft;

  if (node == NULL) {
    return;
  }

  notleft = (node->parent_dir != -1);
  rprofile[y] = max(rprofile[y], x + ( (node->lablen - notleft) / 2));

  if (node->right != NULL) {
    for (int i = 1; i <= node->edge_length && y+i < MAX_HEIGHT; i++) {
      rprofile[y+i] = max(rprofile[y+i], x+i);
    }
  }
  compute_rprofile(node->left, x - node->edge_length -1, y + node->edge_length +1);
  compute_rprofile(node->right, x + node->edge_length +1, y + node->edge_length +1);
}

// This function fills in the edge_length and height fields of the specified tree.
void compute_edge_lengths(Asciinode* node)
{
  int h, hmin, delta;

  if (node == NULL) {
    return;
  }

  compute_edge_lengths(node->left);
  compute_edge_lengths(node->right);

  // First fill in the edge_length of node.
  if (node->right == NULL && node->left == NULL) {
    node->edge_length = 0;
  }
  else {
    if (node->left != NULL) {
      for (int i = 0; i < node->left->height && i < MAX_HEIGHT; i++) {
        rprofile[i] = -INFINITY;
      }
      compute_rprofile(node->left, 0, 0);
      hmin = node->left->height;
    }
    else {
      hmin = 0;
    }
    if (node->right != NULL) {
      for (int i = 0; i < node->right->height && i < MAX_HEIGHT; i++) {
        lprofile[i] = INFINITY;
      }
      compute_lprofile(node->right, 0, 0);
      hmin = min(node->right->height, hmin);
    }
    else {
      hmin = 0;
    }
    delta = 4;
    for (int i = 0; i < hmin; i++) {
      delta = max(delta, gap + 1 + rprofile[i] - lprofile[i]);
    }

    // If the node has two children of height 1, then we allow the
    // two leaves to be within 1, instead of 2.
    if (((node->left != NULL && node->left->height == 1) ||
        (node->right != NULL && node->right->height == 1)) && delta > 4)
    {
      delta--;
    }

    node->edge_length = ( (delta+1) /2) - 1;
  }

  // Now fill in the height of node.
  h = 1;
  if (node->left != NULL) {
    h = max(node->left->height + node->edge_length + 1, h);
  }
  if (node->right != NULL) {
    h = max(node->right->height + node->edge_length + 1, h);
  }
  node->height = h;
}

// This function prints the given level of the given tree, assuming
// that the node has the given x coordinate.
void print_level(Asciinode* node, int x, int level)
{
  int i; // Index for the for loop, i needed outside for. Alternative possible.
  int isleft;

  if (node == NULL) {
    return;
  }

  isleft = (node->parent_dir == -1);

  if (level == 0) {
    for (i = 0; i < (x - print_next - ((node->lablen - isleft) / 2) ); i++) {
      printf(" ");
    }
    print_next += i; // Global variable
    printf("%s", node->label);
    print_next += node->lablen;
  }
  else if (node->edge_length >= level) {
    if (node->left != NULL) {
      for (i = 0; i < (x - print_next - (level)); i++) {
        printf(" ");
      }
      print_next += i;
      printf("/");
      print_next++;
    }
    if (node->right != NULL) {
      for (i = 0; i < (x - print_next + (level)); i++) {
        printf(" ");
      }
      print_next += i;
      printf("\\");
      print_next++;
    }
  }
  else {
    print_level(node->left,
                x - node->edge_length -1,
                level - node->edge_length -1);
    print_level(node->right,
                x + node->edge_length +1,
                level - node->edge_length -1);
  }
}

// Print ascii tree for given Tree t
void print_ascii_tree(Tree* t)
{
  Asciinode* proot; // pointer of the root of the ascii tree
  int xmin = 0;

  if (t == NULL) {
    return;
  }

  proot = build_ascii_tree(t);
  compute_edge_lengths(proot);

  for (int i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
    lprofile[i] = INFINITY;
  }

  compute_lprofile(proot, 0, 0);

  for (int i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
    xmin = min(xmin, lprofile[i]);
  }

  for (int i = 0; i < proot->height; i++) {
    print_next = 0; // Global variable.
    print_level(proot, -xmin, i);
    printf("\n");
  }

  if (proot->height >= MAX_HEIGHT)  {
    printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }

  free_ascii_tree(proot); // Destroy tree, deallocate memory.
}



//////////////////// Various functions ////////////////////

int min (int x, int y)
{
  return ((x) < (y)) ? (x) : (y);
}

int max (int x, int y)
{
  return ((x) > (y)) ? (x) : (y);
}
