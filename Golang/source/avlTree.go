package source

import (
	"fmt"
	"encoding/binary"
	"bytes"
	"os"
)

type AVLNode struct {
	key    int
	left   *AVLNode
	right  *AVLNode
	height byte
}

type AVLTree struct {
	root *AVLNode
}

func NewAVLTree() *AVLTree {
	return &AVLTree{}
}

func getHeight(node *AVLNode) byte {
	if node == nil {
		return 0
	}
	return node.height
}

func getBalanceFactor(node *AVLNode) int {
	if node == nil {
		return 0
	}
	return int(getHeight(node.left)) - int(getHeight(node.right))
}

func fixHeight(node *AVLNode) {
	hL := getHeight(node.left)
	hR := getHeight(node.right)
	if hL > hR {
		node.height = hL + 1
	} else {
		node.height = hR + 1
	}
}

func rotateRight(node *AVLNode) *AVLNode {
	temp := node.left
	node.left = temp.right
	temp.right = node
	fixHeight(node)
	fixHeight(temp)
	return temp
}

func rotateLeft(node *AVLNode) *AVLNode {
	temp := node.right
	node.right = temp.left
	temp.left = node
	fixHeight(node)
	fixHeight(temp)
	return temp
}

func insertNode(node *AVLNode, key int) *AVLNode {
	if node == nil {
		return &AVLNode{key: key, height: 1}
	}
	if key < node.key {
		node.left = insertNode(node.left, key)
	} else if key > node.key {
		node.right = insertNode(node.right, key)
	} else {
		return node
	}

	fixHeight(node)
	bf := getBalanceFactor(node)

	if bf == 2 {
		if getBalanceFactor(node.left) >= 0 {
			return rotateRight(node)
		} else {
			node.left = rotateLeft(node.left)
			return rotateRight(node)
		}
	}
	if bf == -2 {
		if getBalanceFactor(node.right) <= 0 {
			return rotateLeft(node)
		} else {
			node.right = rotateRight(node.right)
			return rotateLeft(node)
		}
	}
	return node
}

func findMinNode(node *AVLNode) *AVLNode {
	if node == nil {
		return nil
	}
	if node.left == nil {
		return node
	}
	return findMinNode(node.left)
}

func removeNode(node *AVLNode, key int) *AVLNode {
	if node == nil {
		return nil
	}
	if key < node.key {
		node.left = removeNode(node.left, key)
	} else if key > node.key {
		node.right = removeNode(node.right, key)
	} else {
		if node.left == nil {
			return node.right
		} else if node.right == nil {
			return node.left
		}
		minNode := findMinNode(node.right)
		node.key = minNode.key
		node.right = removeNode(node.right, minNode.key)
	}

	fixHeight(node)
	bf := getBalanceFactor(node)

	if bf == 2 {
		if getBalanceFactor(node.left) >= 0 {
			return rotateRight(node)
		} else {
			node.left = rotateLeft(node.left)
			return rotateRight(node)
		}
	}
	if bf == -2 {
		if getBalanceFactor(node.right) <= 0 {
			return rotateLeft(node)
		} else {
			node.right = rotateRight(node.right)
			return rotateLeft(node)
		}
	}
	return node
}

func findNode(node *AVLNode, key int) bool {
	if node == nil {
		return false
	}
	if key == node.key {
		return true
	}
	if key < node.key {
		return findNode(node.left, key)
	}
	return findNode(node.right, key)
}

func (t *AVLTree) Empty() bool {
	return t.root == nil
}

func (t *AVLTree) Clear() {
	t.root = nil // Сборщик мусора (GC) сделает всё остальное автоматичеки
}

func (t *AVLTree) Insert(key int) {
	t.root = insertNode(t.root, key)
}

func (t *AVLTree) Remove(key int) bool {
	if !findNode(t.root, key) {
		return false
	}
	t.root = removeNode(t.root, key)
	return true
}

func (t *AVLTree) Find(key int) bool {
	return findNode(t.root, key)
}

func inorderWalk(node *AVLNode, res *[]int) {
	if node == nil {
		return
	}
	inorderWalk(node.left, res)
	*res = append(*res, node.key)
	inorderWalk(node.right, res)
}

func (t *AVLTree) Inorder() []int {
	var result []int
	inorderWalk(t.root, &result)
	return result
}

func preorderWalk(node *AVLNode, res *[]int) {
	if node == nil {
		return
	}
	*res = append(*res, node.key)
	preorderWalk(node.left, res)
	preorderWalk(node.right, res)
}

func (t *AVLTree) Preorder() []int {
	var result []int
	preorderWalk(t.root, &result)
	return result
}

func postorderWalk(node *AVLNode, res *[]int) {
	if node == nil {
		return
	}
	postorderWalk(node.left, res)
	postorderWalk(node.right, res)
	*res = append(*res, node.key)
}

func (t *AVLTree) Postorder() []int {
	var result []int
	postorderWalk(t.root, &result)
	return result
}

func (t *AVLTree) BFS() []int {
	if t.root == nil {
		return nil
	}
	var result []int
	queue := []*AVLNode{t.root}

	for len(queue) > 0 {
		curr := queue[0]
		queue = queue[1:]
		result = append(result, curr.key)
		if curr.left != nil {
			queue = append(queue, curr.left)
		}
		if curr.right != nil {
			queue = append(queue, curr.right)
		}
	}
	return result
}

func (t *AVLTree) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := t.Preorder()
	fmt.Fprintln(file, len(elems))
	for _, v := range elems {
		fmt.Fprintln(file, v)
	}
	return nil
}

func (t *AVLTree) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	t.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var v int
		fmt.Fscan(file, &v)
		t.Insert(v)
	}
	return nil
}

func (t *AVLTree) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := t.Preorder()
	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, int32(len(elems)))
	for _, v := range elems {
		binary.Write(&buf, binary.LittleEndian, int32(v))
	}
	_, err = file.Write(buf.Bytes())
	return err
}

func (t *AVLTree) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	t.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var v int32
		binary.Read(file, binary.LittleEndian, &v)
		t.Insert(int(v))
	}
	return nil
}