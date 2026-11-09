package source

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestAVLInsertAndFind(t *testing.T) {
	tree := NewAVLTree()
	assert.True(t, tree.Empty(), "New AVL tree should be empty")

	tree.Insert(5)
	tree.Insert(3)
	tree.Insert(8)

	assert.False(t, tree.Empty())
	assert.True(t, tree.Find(3), "Should find existing key 3")
	assert.True(t, tree.Find(5))
	assert.False(t, tree.Find(4), "Should not find missing key 4")
}

func TestAVLInorderTraversal(t *testing.T) {
	tree := NewAVLTree()
	for _, v := range []int{5, 3, 8, 1, 4} {
		tree.Insert(v)
	}

	expected := []int{1, 3, 4, 5, 8}
	assert.Equal(t, expected, tree.Inorder(), "Inorder traversal should be sorted")
}

func TestAVLRemove(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(1)
	tree.Insert(2)
	tree.Insert(3)

	assert.True(t, tree.Remove(2), "Removing existing key should return true")
	assert.False(t, tree.Find(2), "Key 2 should be gone")
	
	expected := []int{1, 3}
	assert.Equal(t, expected, tree.Inorder())

	assert.False(t, tree.Remove(99), "Removing non-existent key should return false")
}


func TestAVL_RemoveRootAndRebalance(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(3)
	tree.Insert(2)
	tree.Insert(1)
	tree.Insert(4)
	
	assert.True(t, tree.Remove(1))
	assert.Equal(t, []int{2, 3, 4}, tree.Inorder())
	
	assert.True(t, tree.Remove(3)) 
	assert.Equal(t, []int{2, 4}, tree.Inorder())
}

func TestAVL_RemoveWithTwoChildren(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(10)
	tree.Insert(5)
	tree.Insert(15)
	tree.Insert(3)
	tree.Insert(7)
	
	assert.True(t, tree.Remove(10))
	assert.False(t, tree.Find(10))
	assert.True(t, tree.Find(15))
	assert.Equal(t, []int{3, 5, 7, 15}, tree.Inorder())
}

func TestAVL_ClearNonEmpty(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(1)
	tree.Insert(2)
	tree.Clear()
	assert.True(t, tree.Empty())
	assert.Nil(t, tree.Inorder())
}

func TestAVL_RemoveWithRebalance_A(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(3)
	tree.Insert(2)
	tree.Insert(1)
	tree.Insert(4)
	
	assert.True(t, tree.Remove(1))
	assert.Equal(t, []int{2, 3, 4}, tree.Inorder())
	
	assert.True(t, tree.Remove(3))
	assert.Equal(t, []int{2, 4}, tree.Inorder())
}

func TestAVL_Traversals_A(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(2)
	tree.Insert(1)
	tree.Insert(3)
	
	assert.Equal(t, []int{1, 2, 3}, tree.Inorder())
	assert.Equal(t, []int{2, 1, 3}, tree.Preorder())
	assert.Equal(t, []int{1, 3, 2}, tree.Postorder())
	assert.Equal(t, []int{2, 1, 3}, tree.BFS())
}

func TestAVL_ClearNonEmpty_A(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(1)
	tree.Insert(2)
	tree.Clear()
	assert.True(t, tree.Empty())
	assert.Nil(t, tree.Inorder())
}