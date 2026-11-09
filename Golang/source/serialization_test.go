package source

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)



func TestArray_TextEmpty(t *testing.T) {
	a := NewArray()
	err := a.SaveToText("/tmp/test_array_empty.txt")
	assert.NoError(t, err)

	b := NewArray()
	err = b.LoadFromText("/tmp/test_array_empty.txt")
	assert.NoError(t, err)
	assert.True(t, b.Empty())
	os.Remove("/tmp/test_array_empty.txt")
}

func TestArray_LoadNonexistent(t *testing.T) {
	a := NewArray()
	err := a.LoadFromText("/tmp/nonexistent_array.txt")
	assert.Error(t, err)
}

func TestArray_TextRoundtrip(t *testing.T) {
	a := NewArray()
	a.PushBack("hello")
	a.PushBack("world")
	a.PushBack("test")

	err := a.SaveToText("/tmp/test_array.txt")
	assert.NoError(t, err)

	b := NewArray()
	err = b.LoadFromText("/tmp/test_array.txt")
	assert.NoError(t, err)

	assert.Equal(t, a.GetSize(), b.GetSize())
	assert.Equal(t, a.Get(), b.Get())
	os.Remove("/tmp/test_array.txt")
}




func TestStack_TextEmpty(t *testing.T) {
	s := NewStack()
	err := s.SaveToText("/tmp/test_stack_empty.txt")
	assert.NoError(t, err)

	t2 := NewStack()
	err = t2.LoadFromText("/tmp/test_stack_empty.txt")
	assert.NoError(t, err)
	assert.True(t, t2.Empty())
	os.Remove("/tmp/test_stack_empty.txt")
}


func TestQueue_TextEmpty(t *testing.T) {
	q := NewQueue()
	err := q.SaveToText("/tmp/test_queue_empty.txt")
	assert.NoError(t, err)

	r := NewQueue()
	err = r.LoadFromText("/tmp/test_queue_empty.txt")
	assert.NoError(t, err)
	assert.True(t, r.Empty())
	os.Remove("/tmp/test_queue_empty.txt")
}

func TestQueue_TextRoundtrip(t *testing.T) {
	q := NewQueue()
	q.Push("first")
	q.Push("second")
	q.Push("third")

	err := q.SaveToText("/tmp/test_queue.txt")
	assert.NoError(t, err)

	r := NewQueue()
	err = r.LoadFromText("/tmp/test_queue.txt")
	assert.NoError(t, err)

	assert.Equal(t, q.Get(), r.Get())
	os.Remove("/tmp/test_queue.txt")
}



func TestLinkList_TextEmpty(t *testing.T) {
	ll := NewLinkList()
	err := ll.SaveToText("/tmp/test_list_empty.txt")
	assert.NoError(t, err)

	m := NewLinkList()
	err = m.LoadFromText("/tmp/test_list_empty.txt")
	assert.NoError(t, err)
	assert.True(t, m.Empty())
	os.Remove("/tmp/test_list_empty.txt")
}

func TestLinkList_TextRoundtrip(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")

	err := ll.SaveToText("/tmp/test_list.txt")
	assert.NoError(t, err)

	m := NewLinkList()
	err = m.LoadFromText("/tmp/test_list.txt")
	assert.NoError(t, err)

	assert.Equal(t, ll.Get(), m.Get())
	os.Remove("/tmp/test_list.txt")
}



func TestDoubleList_TextEmpty(t *testing.T) {
	dll := NewDoubleLinkList()
	err := dll.SaveToText("/tmp/test_dll_empty.txt")
	assert.NoError(t, err)

	m := NewDoubleLinkList()
	err = m.LoadFromText("/tmp/test_dll_empty.txt")
	assert.NoError(t, err)
	assert.True(t, m.Empty())
	os.Remove("/tmp/test_dll_empty.txt")
}

func TestDoubleList_TextRoundtrip(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("1")
	dll.PushFront("0")
	dll.PushBack("2")

	err := dll.SaveToText("/tmp/test_dll.txt")
	assert.NoError(t, err)

	m := NewDoubleLinkList()
	err = m.LoadFromText("/tmp/test_dll.txt")
	assert.NoError(t, err)

	assert.Equal(t, dll.Get(), m.Get())
	assert.Equal(t, dll.GetReverse(), m.GetReverse())
	os.Remove("/tmp/test_dll.txt")
}



func TestAVLTree_TextEmpty(t *testing.T) {
	tree := NewAVLTree()
	err := tree.SaveToText("/tmp/test_avl_empty.txt")
	assert.NoError(t, err)

	u := NewAVLTree()
	err = u.LoadFromText("/tmp/test_avl_empty.txt")
	assert.NoError(t, err)
	assert.True(t, u.Empty())
	os.Remove("/tmp/test_avl_empty.txt")
}

func TestAVLTree_TextRoundtrip(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(5)
	tree.Insert(3)
	tree.Insert(8)
	tree.Insert(1)
	tree.Insert(4)

	err := tree.SaveToText("/tmp/test_avl.txt")
	assert.NoError(t, err)

	u := NewAVLTree()
	err = u.LoadFromText("/tmp/test_avl.txt")
	assert.NoError(t, err)

	assert.Equal(t, tree.Inorder(), u.Inorder())
	assert.True(t, u.Find(3))
	assert.False(t, u.Find(99))
	os.Remove("/tmp/test_avl.txt")
}



func TestChainTable_TextEmpty(t *testing.T) {
	ct := NewChainTable(128)
	err := ct.SaveToText("/tmp/test_chain_empty.txt")
	assert.NoError(t, err)

	u := NewChainTable(128)
	err = u.LoadFromText("/tmp/test_chain_empty.txt")
	assert.NoError(t, err)
	assert.True(t, u.Empty())
	os.Remove("/tmp/test_chain_empty.txt")
}





func TestCuckooTable_TextEmpty(t *testing.T) {
	ct := NewCuckooTable(128)
	err := ct.SaveToText("/tmp/test_cuckoo_empty.txt")
	assert.NoError(t, err)

	u := NewCuckooTable(128)
	err = u.LoadFromText("/tmp/test_cuckoo_empty.txt")
	assert.NoError(t, err)
	assert.True(t, u.Empty())
	os.Remove("/tmp/test_cuckoo_empty.txt")
}




func TestArray_BinaryEmpty(t *testing.T) {
	a := NewArray()
	err := a.SaveToBinary("/tmp/test_array_empty.bin")
	assert.NoError(t, err)
	b := NewArray()
	err = b.LoadFromBinary("/tmp/test_array_empty.bin")
	assert.NoError(t, err)
	assert.True(t, b.Empty())
	os.Remove("/tmp/test_array_empty.bin")
}

func TestArray_LoadBinaryNonexistent(t *testing.T) {
	a := NewArray()
	err := a.LoadFromBinary("/tmp/nonexistent_array.bin")
	assert.Error(t, err)
}

func TestArray_BinaryRoundtrip(t *testing.T) {
	a := NewArray()
	a.PushBack("hello")
	a.PushBack("world")
	a.PushBack("test")
	err := a.SaveToBinary("/tmp/test_array.bin")
	assert.NoError(t, err)
	b := NewArray()
	err = b.LoadFromBinary("/tmp/test_array.bin")
	assert.NoError(t, err)
	assert.Equal(t, a.GetSize(), b.GetSize())
	assert.Equal(t, a.Get(), b.Get())
	os.Remove("/tmp/test_array.bin")
}

func TestStack_BinaryEmpty(t *testing.T) {
	s := NewStack()
	err := s.SaveToBinary("/tmp/test_stack_empty.bin")
	assert.NoError(t, err)
	t2 := NewStack()
	err = t2.LoadFromBinary("/tmp/test_stack_empty.bin")
	assert.NoError(t, err)
	assert.True(t, t2.Empty())
	os.Remove("/tmp/test_stack_empty.bin")
}

func TestStack_LoadBinaryNonexistent(t *testing.T) {
	s := NewStack()
	err := s.LoadFromBinary("/tmp/nonexistent_stack.bin")
	assert.Error(t, err)
}

func TestStack_BinaryRoundtrip(t *testing.T) {
	s := NewStack()
	s.Push("a");
	s.Push("b");
	s.Push("c");
	err := s.SaveToBinary("/tmp/test_stack.bin")
	if err != nil {
		t.Fatalf("SaveToBinary failed: %v", err)
	}
	s2 := NewStack()
	err = s2.LoadFromBinary("/tmp/test_stack.bin")
	if err != nil {
		t.Fatalf("LoadFromBinary failed: %v", err)
	}
	sElems := s.Get()
	s2Elems := s2.Get()
	if len(sElems) != len(s2Elems) {
		t.Fatalf("Size mismatch: got %d, want %d", len(s2Elems), len(sElems))
	}
	os.Remove("/tmp/test_stack.bin")
}


func TestQueue_BinaryEmpty(t *testing.T) {
	q := NewQueue()
	err := q.SaveToBinary("/tmp/test_queue_empty.bin")
	assert.NoError(t, err)
	r := NewQueue()
	err = r.LoadFromBinary("/tmp/test_queue_empty.bin")
	assert.NoError(t, err)
	assert.True(t, r.Empty())
	os.Remove("/tmp/test_queue_empty.bin")
}

func TestQueue_LoadBinaryNonexistent(t *testing.T) {
	q := NewQueue()
	err := q.LoadFromBinary("/tmp/nonexistent_queue.bin")
	assert.Error(t, err)
}

func TestQueue_BinaryRoundtrip(t *testing.T) {
	q := NewQueue()
	q.Push("first")
	q.Push("second")
	q.Push("third")
	err := q.SaveToBinary("/tmp/test_queue.bin")
	assert.NoError(t, err)
	r := NewQueue()
	err = r.LoadFromBinary("/tmp/test_queue.bin")
	assert.NoError(t, err)
	assert.Equal(t, q.Get(), r.Get())
	os.Remove("/tmp/test_queue.bin")
}

func TestLinkList_BinaryEmpty(t *testing.T) {
	ll := NewLinkList()
	err := ll.SaveToBinary("/tmp/test_list_empty.bin")
	assert.NoError(t, err)
	m := NewLinkList()
	err = m.LoadFromBinary("/tmp/test_list_empty.bin")
	assert.NoError(t, err)
	assert.True(t, m.Empty())
	os.Remove("/tmp/test_list_empty.bin")
}

func TestLinkList_LoadBinaryNonexistent(t *testing.T) {
	ll := NewLinkList()
	err := ll.LoadFromBinary("/tmp/nonexistent_list.bin")
	assert.Error(t, err)
}

func TestLinkList_BinaryRoundtrip(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")
	err := ll.SaveToBinary("/tmp/test_list.bin")
	assert.NoError(t, err)
	m := NewLinkList()
	err = m.LoadFromBinary("/tmp/test_list.bin")
	assert.NoError(t, err)
	assert.Equal(t, ll.Get(), m.Get())
	os.Remove("/tmp/test_list.bin")
}

func TestDoubleList_BinaryEmpty(t *testing.T) {
	dll := NewDoubleLinkList()
	err := dll.SaveToBinary("/tmp/test_dll_empty.bin")
	assert.NoError(t, err)
	m := NewDoubleLinkList()
	err = m.LoadFromBinary("/tmp/test_dll_empty.bin")
	assert.NoError(t, err)
	assert.True(t, m.Empty())
	os.Remove("/tmp/test_dll_empty.bin")
}

func TestDoubleList_LoadBinaryNonexistent(t *testing.T) {
	dll := NewDoubleLinkList()
	err := dll.LoadFromBinary("/tmp/nonexistent_dll.bin")
	assert.Error(t, err)
}

func TestDoubleList_BinaryRoundtrip(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("1")
	dll.PushFront("0")
	dll.PushBack("2")
	err := dll.SaveToBinary("/tmp/test_dll.bin")
	assert.NoError(t, err)
	m := NewDoubleLinkList()
	err = m.LoadFromBinary("/tmp/test_dll.bin")
	assert.NoError(t, err)
	assert.Equal(t, dll.Get(), m.Get())
	assert.Equal(t, dll.GetReverse(), m.GetReverse())
	os.Remove("/tmp/test_dll.bin")
}

func TestAVLTree_BinaryEmpty(t *testing.T) {
	tree := NewAVLTree()
	err := tree.SaveToBinary("/tmp/test_avl_empty.bin")
	assert.NoError(t, err)
	u := NewAVLTree()
	err = u.LoadFromBinary("/tmp/test_avl_empty.bin")
	assert.NoError(t, err)
	assert.True(t, u.Empty())
	os.Remove("/tmp/test_avl_empty.bin")
}

func TestAVLTree_LoadBinaryNonexistent(t *testing.T) {
	tree := NewAVLTree()
	err := tree.LoadFromBinary("/tmp/nonexistent_avl.bin")
	assert.Error(t, err)
}

func TestAVLTree_BinaryRoundtrip(t *testing.T) {
	tree := NewAVLTree()
	tree.Insert(5)
	tree.Insert(3)
	tree.Insert(8)
	tree.Insert(1)
	tree.Insert(4)
	err := tree.SaveToBinary("/tmp/test_avl.bin")
	assert.NoError(t, err)
	u := NewAVLTree()
	err = u.LoadFromBinary("/tmp/test_avl.bin")
	assert.NoError(t, err)
	assert.Equal(t, tree.Inorder(), u.Inorder())
	assert.True(t, u.Find(3))
	assert.False(t, u.Find(99))
	os.Remove("/tmp/test_avl.bin")
}

func TestChainTable_BinaryEmpty(t *testing.T) {
	ct := NewChainTable(128)
	err := ct.SaveToBinary("/tmp/test_chain_empty.bin")
	assert.NoError(t, err)
	u := NewChainTable(128)
	err = u.LoadFromBinary("/tmp/test_chain_empty.bin")
	assert.NoError(t, err)
	assert.True(t, u.Empty())
	os.Remove("/tmp/test_chain_empty.bin")
}

func TestChainTable_LoadBinaryNonexistent(t *testing.T) {
	ct := NewChainTable(128)
	err := ct.LoadFromBinary("/tmp/nonexistent_chain.bin")
	assert.Error(t, err)
}

func TestChainTable_BinaryRoundtrip(t *testing.T) {
	ct := NewChainTable(128)
	ct.ChainInsert(1, "one")
	ct.ChainInsert(2, "two")
	ct.ChainInsert(3, "three")
	err := ct.SaveToBinary("/tmp/test_chain.bin")
	assert.NoError(t, err)
	u := NewChainTable(128)
	err = u.LoadFromBinary("/tmp/test_chain.bin")
	assert.NoError(t, err)
	assert.True(t, u.ChainFind(1))
	assert.True(t, u.ChainFind(2))
	assert.True(t, u.ChainFind(3))
	assert.False(t, u.ChainFind(4))
	val, ok := u.GetValue(1)
	assert.True(t, ok)
	assert.Equal(t, "one", val)
	os.Remove("/tmp/test_chain.bin")
}

func TestCuckooTable_BinaryEmpty(t *testing.T) {
	ct := NewCuckooTable(128)
	err := ct.SaveToBinary("/tmp/test_cuckoo_empty.bin")
	assert.NoError(t, err)
	u := NewCuckooTable(128)
	err = u.LoadFromBinary("/tmp/test_cuckoo_empty.bin")
	assert.NoError(t, err)
	assert.True(t, u.Empty())
	os.Remove("/tmp/test_cuckoo_empty.bin")
}

func TestCuckooTable_LoadBinaryNonexistent(t *testing.T) {
	ct := NewCuckooTable(128)
	err := ct.LoadFromBinary("/tmp/nonexistent_cuckoo.bin")
	assert.Error(t, err)
}

func TestCuckooTable_BinaryRoundtrip_T(t *testing.T) {
	ct := NewCuckooTable(128)
	ct.CuckooInsert(10, "ten")
	ct.CuckooInsert(20, "twenty")
	ct.CuckooInsert(30, "thirty")
	err := ct.SaveToBinary("/tmp/test_cuckoo.bin")
	if err != nil {
		t.Fatalf("SaveToBinary failed: %v", err)
	}
	
	u := NewCuckooTable(128)
	err = u.LoadFromBinary("/tmp/test_cuckoo.bin")
	if err != nil {
		t.Fatalf("LoadFromBinary failed: %v", err)
	}
	if !u.CuckooFind(10) {
		t.Error("Key 10 should be found after loading")
	}
	if !u.CuckooFind(20) {
		t.Error("Key 20 should be found after loading")
	}
	if !u.CuckooFind(30) {
		t.Error("Key 30 should be found after loading")
	}
	os.Remove("/tmp/test_cuckoo.bin")
}