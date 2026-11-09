package source

import "testing"

func TestAVLInsertAndFind_T(t *testing.T) {
	tree := NewAVLTree()
	if !tree.Empty() {
		t.Fatal("New AVL tree should be empty")
	}

	tree.Insert(5)
	tree.Insert(3)
	tree.Insert(8)

	if tree.Empty() {
		t.Error("Tree should not be empty after inserts")
	}
	if !tree.Find(3) || !tree.Find(5) || !tree.Find(8) {
		t.Error("Should find existing keys")
	}
	if tree.Find(4) {
		t.Error("Should not find missing key 4")
	}
}

func TestAVLInorderTraversal_T(t *testing.T) {
	tree := NewAVLTree()
	for _, v := range []int{5, 3, 8, 1, 4} {
		tree.Insert(v)
	}

	expected := []int{1, 3, 4, 5, 8}
	result := tree.Inorder()

	if len(result) != len(expected) {
		t.Fatalf("Length mismatch: got %d, want %d", len(result), len(expected))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("Index %d: got %d, want %d", i, result[i], v)
		}
	}
}

func TestAVLRemove_T(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(1)
	tree.Insert(2)
	tree.Insert(3)

	if !tree.Remove(2) {
		t.Error("Removing existing key should return true")
	}
	if tree.Find(2) {
		t.Error("Key 2 should be gone")
	}

	expected := []int{1, 3}
	result := tree.Inorder()
	if len(result) != len(expected) || result[0] != 1 || result[1] != 3 {
		t.Errorf("Inorder after remove failed. Got: %v", result)
	}

	if tree.Remove(99) {
		t.Error("Removing non-existent key should return false")
	}
}

func TestAVLTraversals_T(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(2)
	tree.Insert(1)
	tree.Insert(3)

	inorder := tree.Inorder()
	if len(inorder) != 3 || inorder[0] != 1 || inorder[1] != 2 || inorder[2] != 3 {
		t.Errorf("Inorder failed: got %v", inorder)
	}

	preorder := tree.Preorder()
	if len(preorder) != 3 || preorder[0] != 2 || preorder[1] != 1 || preorder[2] != 3 {
		t.Errorf("Preorder failed: got %v", preorder)
	}

	postorder := tree.Postorder()
	if len(postorder) != 3 || postorder[0] != 1 || postorder[1] != 3 || postorder[2] != 2 {
		t.Errorf("Postorder failed: got %v", postorder)
	}

	bfs := tree.BFS()
	if len(bfs) != 3 || bfs[0] != 2 || bfs[1] != 1 || bfs[2] != 3 {
		t.Errorf("BFS failed: got %v", bfs)
	}
}

func TestAVLClearNonEmpty_T(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(1)
	tree.Insert(2)
	tree.Clear()
	if !tree.Empty() {
		t.Error("Tree should be empty after clear")
	}
}