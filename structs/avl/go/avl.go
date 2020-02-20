/*
 * algrm - structs
 * AVL binary search tree (BST)
*/

package main

import (
	"fmt"
	"strings"
)

// define min of two ints because go has no min for ints
func min(x int, y int) int {
	if(x <= y) {
		return x
	} else {
		return y
	}
}

// define min of two ints because go has no max for ints
func max(x int, y int) int {
	if(x > y) {
		return x
	} else {
		return y 
	}
}

func abs(x int) int {
	if(x < 0) {
		return -x
	} else {
		return x
	}
}

// define comparison interface
type Comparable interface {
	// return true this item less than other, false otherwise
	Less(Comparable) bool
	// return true this item more than other, false otherwise
	More(Comparable) bool
	// return true this item equal with other, false otherwise
	Equal(Comparable) bool
}

// implement comparable interface so we have 
// something to store in the avl tree
type Int int 

func (x Int) Less(y Comparable) bool {
	return x < y.(Int)
}

func (x Int) More(y Comparable) bool {
	return x > y.(Int)
}

func (x Int) Equal(y Comparable) bool {
	return x == y.(Int)
}

/* AVL Binary Search Tree */
// node in the AVL BST 
type AVL struct {
	left *AVL; // left subtree
	right *AVL; // right subtree
	parent *AVL; // parent of the node
	item Comparable
	count int // no. of items stored
}

// insert the given item into the avl tree
// modifies the tree in place
// returns the avl tree with the item inserted
func (avl *AVL) insert(item Comparable) *AVL {
	if avl.item == nil { // new avl tree
		avl.item = item
	} else if item.Less(avl.item) {
		if(avl.left != nil) {
			// recursively insert item into left subtree
			avl.left = avl.left.insert(item)
		} else { 
			// insert node on as left node
			avl.left = &AVL {
				parent: avl,
				item: item,
				count:  1,
			}
		}
	} else if item.More(avl.item) {
		if avl.right != nil {
			// recursively insert item into right subtree
			avl.right = avl.right.insert(item)
		} else {
			// insert node on as right node
			avl.right = &AVL {
				parent: avl,
				item: item,
				count:  1,
			}

		}
	} else { // items equal
		avl.count += 1
	}

	return avl
}

// add item to the avl tree
// automatically rebalances the tree after insertion
func (avl *AVL) add(item Comparable) *AVL {
	avl = avl.insert(item)
	avl = avl.rebalance()
	return avl
}

// find the given node with the given item the avl tree
// returns node with the given item or nil if not found
func (avl *AVL) find(item Comparable) *AVL {
	// base case: not found 
	if avl == nil || avl.item == nil {
		return nil
	}

	// base case: found item
	if item.Equal(avl.item) {
		return avl
	}

	// recursively search subtrees for item
	if item.Less(avl.item) {
		return avl.left.find(item)
	} else { // avl item more than item
		return avl.right.find(item)
	}
}

// find the node with min item 
// return node with min item
func (avl *AVL) min() *AVL {
	// base case: empty node
	if avl == nil || avl.item == nil {
		return nil
	}
	
	// found min
	if avl.left == nil && avl.right == nil {
		return avl
	}
	
	// recusively find min
	return avl.left.min()
}

// remove the node with the given item
func (avl *AVL) remove(item Comparable) *AVL {
	// base case: not found 
	if avl == nil || avl.item == nil {
		return nil
	}

	// recursively find item to remove
	if !item.Equal(avl.item) {
		if item.Less(avl.item) {
			avl.left = avl.left.remove(item)
		} else if item.More(avl.item) {
			avl.right = avl.right.remove(item)
		}
		
		return avl
	}

	// found item to remove
	if avl.left == nil && avl.right == nil {
		// no children: just remove
		return nil
	} else if avl.left == nil  {
		// only right child: replace node with rigth child
		return avl.right 
	} else if avl.right == nil  {
		// only left child: replace node with rigth child
		return avl.left 
	} else {
		min := avl.right.min()
		avl.right = avl.right.remove(min.item)
		avl.item = min.item
		return avl
	}
}

// drop the node with the given item
// automatically rebalaces the tree
func (avl *AVL) drop(item Comparable) *AVL {
	avl = avl.remove(item)
	avl = avl.rebalance()
	return avl
}

// compute the height of the given avl tree
func (avl *AVL) height() int {
	// base case: nil
	// base case: new avl tree
	if avl == nil || avl.item == nil {
		return 0
	}

	// base case: leaf node
	if avl.right == nil && avl.left == nil {
		return 1
	}

	// compute height on left and right branches
	leftHeight, rightHeight := 0, 0
	if avl.left != nil {
		leftHeight = avl.left.height()
	}
	if avl.right != nil {
		rightHeight = avl.right.height()
	}
	
	return max(leftHeight, rightHeight) + 1
}

// return the string with a prefix to every line
func prefixLines(lines []string, prefix string) string {
	newStr := ""
	for _, line := range lines[:] {
		// skip empty lines
		if len(line)  <= 0 {
			continue
		}
		newStr += prefix + line + "\n"
	}

	return newStr
}

// render the given child string 
func (avl *AVL) toChildStr() string {
	avlStr := avl.toString()
	lines := strings.Split(avlStr, "\n")
	// put arm on direct child
	childLine := "|= "  + lines[0]
	
	// prefix other lines with spacing 
	otherLines := prefixLines(lines[1:], "   ")
	lines = append([]string{ childLine }, otherLines)
	
	return strings.Join(lines, "\n")
}

// convert bst to string representation
func (avl *AVL) toString() string {
	// base case: nil
	// base case: new avl tree
	if avl == nil || avl.item == nil {
		return ""
	}

	avlStr := fmt.Sprintf("%d\n", avl.item)

	// recursively print children
	// print "arms" if have children
	if avl.left != nil {
		avlStr += avl.left.toChildStr()
	} 
	if avl.right != nil {
		avlStr += avl.right.toChildStr()
	}
	
	return avlStr
}

/* AVL Rotations */
//   z                                y
//  /  \                            /   \ 
// T1   y     Left Rotate(z)       z      x
//     /  \   - - - - - - - ->    / \    / \
//    T2   x                     T1  T2 T3  T4
//        / \
//      T3  T4
// perform left rotation of the given node with the right subtree
// returns node that will replace given node after peforming left rotation
func (avl *AVL) rotateLeft() *AVL {
	// check arguments
	if avl == nil || avl.item == nil {
		// no rotation required
		return avl 
	} else if avl.right == nil {
		panic("rotateLeft(): Missing right subtree to rotate with")
	}

	node := avl
	// save left subtree of right subtree (T2)
	subtree := node.right.left
	// swap right subtree with node.
	// node becomes left substree of previously right substree
	avl = node.right
	avl.parent = node.parent
	avl.left = node
	node.parent = avl
	// reattach subtree as right subtree of node
	node.right = subtree
	if(subtree != nil) {
		subtree.parent = node
	}
	
	return avl
}

//         z                                      y 
//        / \                                   /   \
//       y   T4      Right Rotate (z)          x      z
//      / \          - - - - - - - - ->      /  \    /  \ 
//     x   T3                               T1  T2  T3  T4
//    / \
//  T1   T2
// perform right rotation of given node with the right subtree
// returns node that will replace node wih the right substree
func (avl *AVL) rotateRight() *AVL {
	if avl == nil || avl.item == nil {
		// no rotation required
		return  avl
	} else if avl.left == nil {
		panic("rotateRight(): no left subtree to rotate with")
	}
	
	node := avl
	// save right subtree of left subtree (T3)
	subtree := node.left.right
	// swap left substree with node
	avl = node.left
	avl.parent = node.parent
	avl.right = node
	node.parent = avl
	
	// reattach subtree as left subtree of node
	node.left = subtree
	if(subtree != nil) {
		subtree.parent = node
	}
	
	return avl
}

/* AVL Balancing */
// check the balance of the bst (ie height of left/right subtrees must be +- 1 height
// returning true if balanced, false otherwise
func (avl *AVL) isBalanced() bool{ 
	// base case: nil
	// base case: new avl tree
	if avl == nil || avl.item == nil {
		return true
	}
	
	// recursive case: check if left and right height balanced 
	// (+- 1 height rangef)
	return abs(avl.left.height() - avl.right.height()) <= 1
}

// performs the required rotations to rebalance the BST
func (avl *AVL) rebalance() *AVL {
	// base case: new avl tree
	if avl == nil || avl.item == nil {
		return avl
	}
	
	// TODO: balance the recusive balance subtrees
	avl.left = avl.left.rebalance()
	avl.right = avl.right.rebalance()

	// already balanced: nothing to do
	if avl.isBalanced() {
		return avl
	}

	// balance the current node
	if avl.left.height() > avl.right.height() &&
		avl.left.left.height() > avl.left.right.height() {
		// left/left imbalane
		avl = avl.rotateRight()
	} else if avl.left.height() > avl.right.height() &&
		avl.left.left.height() < avl.left.right.height() {
		// left/right imbalance
		avl.left = avl.left.rotateLeft()
		avl = avl.rotateRight()
	} else if avl.right.height() > avl.left.height() &&
		avl.right.right.height() > avl.right.left.height() {
		// right/right imbalance
		avl = avl.rotateLeft()
	} else if avl.right.height() > avl.left.height() &&
		avl.right.right.height() < avl.right.left.height() {
		// right/left imbalance
		avl.right = avl.right.rotateRight() // rotate right with right subtree
		avl = avl.rotateLeft()
	} 

	return avl 
}


func main() {
	var tree AVL
	var x Int = 2
	avl := &tree
	avl = avl.add(x)
	
	x = 1; avl = avl.add(x)
	x = -1; avl = avl.add(x)
	x = -2; avl = avl.add(x)
	x = -3; avl = avl.add(x)
	x = -4; avl = avl.add(x)
	x = 3; avl = avl.add(x)
	x = 4; avl = avl.add(x)
	x = 5; avl = avl.add(x)
	x = 7; avl = avl.add(x)
	x = 6; avl = avl.add(x)
	fmt.Printf("height: %d\n", avl.height())
	fmt.Println(avl.toString())
	
	fmt.Println(avl.toString())
	x = 5
	found := avl.find(x)
	fmt.Println(found.item)

	found = avl.min()
	fmt.Println(found.item)

	x = -1 ;avl.drop(x)
	fmt.Println(avl.toString())
}

