package source

import (
	"testing"
	"fmt"
	"github.com/stretchr/testify/assert"
)

func TestCuckooTableBasicOps(t *testing.T) {
	ct := NewCuckooTable()
	assert.True(t, ct.Empty())

	assert.True(t, ct.CuckooInsert(1, "one"))
	assert.True(t, ct.CuckooInsert(2, "two"))
	
	assert.False(t, ct.Empty())
	assert.True(t, ct.CuckooFind(1))
	assert.False(t, ct.CuckooFind(3))
}

func TestCuckooTableManyInserts(t *testing.T) {
	ct := NewCuckooTable(64)
	n := 500
	
	for i := 0; i < n; i++ {
		ok := ct.CuckooInsert(i, string(rune('a'+i%26)))
		assert.True(t, ok, "Failed to insert key %d during bulk insert", i)
	}

	for i := 0; i < n; i++ {
		assert.True(t, ct.CuckooFind(i), "Key %d not found after bulk insert", i)
	}
}

func TestCuckooTableRemove(t *testing.T) {
	ct := NewCuckooTable()
	ct.CuckooInsert(7, "seven")
	
	assert.True(t, ct.CuckooRemove(7))
	assert.False(t, ct.CuckooFind(7))
	assert.True(t, ct.Empty())
}

func TestCuckooTable_RehashAndBulk(t *testing.T) {
	ct := NewCuckooTable(4) 
	
	for i := 0; i < 50; i++ {
		ok := ct.CuckooInsert(i, fmt.Sprintf("v%d", i))
		assert.True(t, ok, "Failed to insert %d", i)
	}
	
	for i := 0; i < 50; i++ {
		assert.True(t, ct.CuckooFind(i))
		val, ok := ct.GetValue(i)
		assert.True(t, ok)
		assert.Equal(t, fmt.Sprintf("v%d", i), val)
	}
}

func TestCuckooTable_RemoveNonExistent(t *testing.T) {
	ct := NewCuckooTable()
	assert.False(t, ct.CuckooRemove(999))
	
	ct.CuckooInsert(1, "a")
	assert.False(t, ct.CuckooRemove(2))
	assert.True(t, ct.CuckooRemove(1))
}