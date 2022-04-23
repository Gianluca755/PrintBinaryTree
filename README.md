# Print binary search tree (BST) in ascii, on the console.

It prints a BST on the console in vertical orientation.
The file contains the various functions needed to mainpulate trees.
The BST is created using a series of insert (and delete), then is passed to a conversion function that create a new tree with the additional parameters needed for printing.

## References
Archived Sources: http://web.archive.org/web/20090617110918/http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/
http://web.archive.org/web/20071224095835/http://www.openasthra.com:80/wp-content/uploads/2007/12/binary_trees1.c

Stackoverflow: https://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console

GitHub gist_1: https://gist.github.com/ximik777/e04e5a9f0548a2f41cb09530924bdd9a

GitHub gist_2: https://gist.github.com/ryangraham/0e3c9af79e4901dee1ce956a07cbccd2

GitHub repo_1: https://github.com/edmlin/PrintAsciiTree

GitHub repo_2: https://github.com/nivpgir/ascii_tree_print

## Output
```
After inserting element 10..
10

After inserting element 5..
 10
 /
5

After inserting element 15..
 10
 / \
5  15

After inserting elements 9, 13..
   10
   / \
  /   \
 /     \
5      15
 \     /
  9   13

After inserting elements 2, 6, 12, 14..
     10
     / \
    /   \
   /     \
  5      15
 / \     /
2   9   13
   /   / \
  6   /   \
     12   14


After deleting a node (14) with no child..
     10
     / \
    /   \
   /     \
  5      15
 / \     /
2   9   13
   /   /
  6   12


After deleting a node (13) with left child..
     10
     / \
    /   \
   /     \
  5      15
 / \     /
2   9   12
   /
  6


After deleting a node (5) with left and right children..
     10
     / \
    /   \
   /     \
  6      15
 / \     /
2   9   12


After deleting a node (10, root node) with left and right children..
   12
   / \
  6  15
 / \
2   9

```
