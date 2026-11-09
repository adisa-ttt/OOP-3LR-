package source

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestArrayEmpty_A(t *testing.T) {
	a := NewArray()
	assert.True(t, a.Empty())
	assert.Equal(t, 0, a.GetSize())
}

func TestArrayPushBackAndGet_A(t *testing.T) {
	a := NewArray()
	a.PushBack("hello")
	a.PushBack("world")

	assert.False(t, a.Empty())
	assert.Equal(t, 2, a.GetSize())
	
	val, err := a.GetAt(0)
	assert.NoError(t, err)
	assert.Equal(t, "hello", val)

	val, err = a.GetAt(1)
	assert.NoError(t, err)
	assert.Equal(t, "world", val)
}

func TestArrayGetAtOutOfRange_A(t *testing.T) {
	a := NewArray()
	a.PushBack("test")

	_, err := a.GetAt(-1)
	assert.Error(t, err)

	_, err = a.GetAt(1)
	assert.Error(t, err)
}

func TestArrayInsertAndRemove_A(t *testing.T) {
	a := NewArray()
	a.PushBack("b")
	a.PushBack("d")

	assert.True(t, a.Insert(0, "a"))
	assert.True(t, a.Insert(2, "c"))
	
	expected := []string{"a", "b", "c", "d"}
	assert.Equal(t, expected, a.Get())

	assert.True(t, a.RemoveAt(1)) 
	expected = []string{"a", "c", "d"}
	assert.Equal(t, expected, a.Get())
}

func TestArrayResize_A(t *testing.T) {
	a := NewArray()
	for i := 0; i < 4; i++ {
		a.PushBack(fmt.Sprintf("old%d", i))
	}
	
	assert.True(t, a.Insert(2, "new"), "Insert should trigger resize and succeed")
	assert.Equal(t, 5, a.GetSize())
}

func TestArrayClear_A(t *testing.T) {
	a := NewArray()
	a.PushBack("x")
	a.Clear()
	
	assert.True(t, a.Empty())
	assert.Equal(t, 0, a.GetSize())
}

func TestArray_ResizeOnInsert_A(t *testing.T) {
	a := NewArray()
	for i := 0; i < 4; i++ {
		a.PushBack(fmt.Sprintf("old%d", i))
	}
	
	assert.True(t, a.Insert(2, "new"))
	assert.Equal(t, 5, a.GetSize())
	
	data := a.Get()
	assert.Equal(t, []string{"old0", "old1", "new", "old2", "old3"}, data)
}