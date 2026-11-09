package source

import (
	"fmt"
	"testing"
)

func BenchmarkArrayPushBack(b *testing.B) {
	a := NewArray()
	i := 0
	for b.Loop() {
		a.PushBack(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkArrayGetAt(b *testing.B) {
	a := NewArray()
	for i := 0; i < 1000; i++ {
		a.PushBack(fmt.Sprintf("item_%d", i))
	}
	idx := 0
	for b.Loop() {
		a.GetAt(idx % a.GetSize())
		idx++
	}
}

func BenchmarkArrayFind(b *testing.B) {
	a := NewArray()
	for i := 0; i < 1000; i++ {
		a.PushBack(fmt.Sprintf("item_%d", i))
	}
	target := "item_500"
	for b.Loop() {
		for j := 0; j < a.GetSize(); j++ {
			val, _ := a.GetAt(j)
			if val == target {
				break
			}
		}
	}
}

func BenchmarkStackPush(b *testing.B) {
	s := NewStack()
	i := 0
	for b.Loop() {
		s.Push(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkQueuePush(b *testing.B) {
	q := NewQueue()
	i := 0
	for b.Loop() {
		q.Push(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkQueuePop(b *testing.B) {
	q := NewQueue()
	for b.Loop() {
		q.Push("item")
		q.Pop()
	}
}

func BenchmarkQueueFind(b *testing.B) {
	q := NewQueue()
	for i := 0; i < 100; i++ {
		q.Push(fmt.Sprintf("item_%d", i))
	}
	target := "item_50"
	for b.Loop() {
		for _, v := range q.Get() {
			if v == target {
				break
			}
		}
	}
}

func BenchmarkLinkListPushFront(b *testing.B) {
	ll := NewLinkList()
	i := 0
	for b.Loop() {
		ll.PushFront(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkLinkListPushBack(b *testing.B) {
	ll := NewLinkList()
	i := 0
	for b.Loop() {
		ll.PushBack(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkLinkListPopFront(b *testing.B) {
	ll := NewLinkList()
	for b.Loop() {
		ll.PushBack("item")
		ll.PopFront()
	}
}

func BenchmarkLinkListFind(b *testing.B) {
	ll := NewLinkList()
	for i := 0; i < 100; i++ {
		ll.PushBack(fmt.Sprintf("item_%d", i))
	}
	target := "item_50"
	for b.Loop() {
		ll.Find(target)
	}
}

func BenchmarkLinkListRemove(b *testing.B) {
	for b.Loop() {
		ll := NewLinkList()
		for i := 0; i < 100; i++ {
			ll.PushBack(fmt.Sprintf("item_%d", i))
		}
		ll.RemoveValue("item_50")
	}
}

func BenchmarkDoubleListPushFront(b *testing.B) {
	dll := NewDoubleLinkList()
	i := 0
	for b.Loop() {
		dll.PushFront(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkDoubleListPushBack(b *testing.B) {
	dll := NewDoubleLinkList()
	i := 0
	for b.Loop() {
		dll.PushBack(fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkDoubleListPopFront(b *testing.B) {
	dll := NewDoubleLinkList()
	for b.Loop() {
		dll.PushBack("item")
		dll.PopFront()
	}
}

func BenchmarkDoubleListFind(b *testing.B) {
	dll := NewDoubleLinkList()
	for i := 0; i < 100; i++ {
		dll.PushBack(fmt.Sprintf("item_%d", i))
	}
	target := "item_50"
	for b.Loop() {
		dll.Find(target)
	}
}

func BenchmarkAVLTreeInsert(b *testing.B) {
	tree := NewAVLTree()
	i := 0
	for b.Loop() {
		tree.Insert(i)
		i++
	}
}

func BenchmarkAVLTreeFind(b *testing.B) {
	tree := NewAVLTree()
	for i := 0; i < 1000; i++ {
		tree.Insert(i)
	}
	for b.Loop() {
		tree.Find(500)
	}
}

func BenchmarkAVLTreeFindMissing(b *testing.B) {
	tree := NewAVLTree()
	for i := 0; i < 1000; i++ {
		tree.Insert(i * 2)
	}
	for b.Loop() {
		tree.Find(999)
	}
}

func BenchmarkAVLTreeRemove(b *testing.B) {
	for b.Loop() {
		tree := NewAVLTree()
		for i := 0; i < 100; i++ {
			tree.Insert(i)
		}
		tree.Remove(50)
	}
}

func BenchmarkChainTableInsert(b *testing.B) {
	ct := NewChainTable(128)
	i := 0
	for b.Loop() {
		ct.ChainInsert(i, fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkChainTableFind(b *testing.B) {
	ct := NewChainTable(128)
	for i := 0; i < 1000; i++ {
		ct.ChainInsert(i, "item")
	}
	for b.Loop() {
		ct.ChainFind(500)
	}
}

func BenchmarkChainTableFindMissing(b *testing.B) {
	ct := NewChainTable(128)
	for i := 0; i < 1000; i++ {
		ct.ChainInsert(i, "item")
	}
	for b.Loop() {
		ct.ChainFind(9999)
	}
}

func BenchmarkChainTableRemove(b *testing.B) {
	for b.Loop() {
		ct := NewChainTable(128)
		for i := 0; i < 100; i++ {
			ct.ChainInsert(i, "item")
		}
		ct.ChainRemove(50)
	}
}

func BenchmarkCuckooTableInsert(b *testing.B) {
	ct := NewCuckooTable(128)
	i := 0
	for b.Loop() {
		ct.CuckooInsert(i, fmt.Sprintf("item_%d", i))
		i++
	}
}

func BenchmarkCuckooTableFind(b *testing.B) {
	ct := NewCuckooTable(128)
	for i := 0; i < 1000; i++ {
		ct.CuckooInsert(i, "item")
	}
	for b.Loop() {
		ct.CuckooFind(500)
	}
}

func BenchmarkCuckooTableFindMissing(b *testing.B) {
	ct := NewCuckooTable(128)
	for i := 0; i < 1000; i++ {
		ct.CuckooInsert(i, "item")
	}
	for b.Loop() {
		ct.CuckooFind(9999)
	}
}

func BenchmarkCuckooTableRemove(b *testing.B) {
	for b.Loop() {
		ct := NewCuckooTable(128)
		for i := 0; i < 100; i++ {
			ct.CuckooInsert(i, "item")
		}
		ct.CuckooRemove(50)
	}
}
