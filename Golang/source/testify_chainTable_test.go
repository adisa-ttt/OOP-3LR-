package source

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestChainTableBasicOps(t *testing.T) {
	ct := NewChainTable()
	assert.True(t, ct.Empty())

	assert.True(t, ct.ChainInsert(1, "one"))
	assert.True(t, ct.ChainInsert(-2, "-two"))
	
	assert.False(t, ct.Empty())
	assert.True(t, ct.ChainFind(1))
	assert.False(t, ct.ChainFind(3))
}

func TestChainTableGetValue(t *testing.T) {
	ct := NewChainTable()
	ct.ChainInsert(10, "ten")

	val, ok := ct.GetValue(10)
	assert.True(t, ok)
	assert.Equal(t, "ten", val)

	_, ok = ct.GetValue(99)
	assert.False(t, ok)
}

func TestChainTableUpdateExisting(t *testing.T) {
	ct := NewChainTable()
	assert.True(t, ct.ChainInsert(1, "old"))
	assert.False(t, ct.ChainInsert(1, "new"), "Updating existing key should return false")

	val, _ := ct.GetValue(1)
	assert.Equal(t, "new", val, "Value should be updated to 'new'")
}

func TestChainTable_RehashAndUpdate(t *testing.T) {
	ct := NewChainTable(2) 
	for i := 0; i < 5; i++ {
		assert.True(t, ct.ChainInsert(i, fmt.Sprintf("val%d", i)))
	}
	
	for i := 0; i < 5; i++ {
		val, ok := ct.GetValue(i)
		assert.True(t, ok)
		assert.Equal(t, fmt.Sprintf("val%d", i), val)
	}
	
	// Обновление существующего
	assert.False(t, ct.ChainInsert(0, "updated"))
	val, _ := ct.GetValue(0)
	assert.Equal(t, "updated", val)
}

func TestChainTable_RemoveFromChain(t *testing.T) {
	ct := NewChainTable(8)
	ct.ChainInsert(1, "a")
	ct.ChainInsert(9, "b") 
	
	assert.True(t, ct.ChainRemove(1))
	assert.False(t, ct.ChainFind(1))
	assert.True(t, ct.ChainFind(9))
}