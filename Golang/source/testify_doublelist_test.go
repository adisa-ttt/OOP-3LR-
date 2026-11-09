package source

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestDoubleListBasicOps_A(t *testing.T) {
	dll := NewDoubleLinkList()
	assert.True(t, dll.Empty())

	dll.PushFront("b")
	dll.PushFront("a")
	dll.PushBack("c")

	expected := []string{"a", "b", "c"}
	assert.Equal(t, expected, dll.Get())
	
	reverseExpected := []string{"c", "b", "a"}
	assert.Equal(t, reverseExpected, dll.GetReverse())
}

func TestDoubleListPop_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("first")
	dll.PushBack("second")

	val, err := dll.PopFront()
	assert.NoError(t, err)
	assert.Equal(t, "first", val)

	val, err = dll.PopBack()
	assert.NoError(t, err)
	assert.Equal(t, "second", val)

	assert.True(t, dll.Empty())
}

func TestDoubleListFindAndInsert_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("c")

	assert.Equal(t, 0, dll.Find("a"))
	assert.Equal(t, -1, dll.Find("z"))

	assert.True(t, dll.InsertAfterValue("a", "b"))
	expected := []string{"a", "b", "c"}
	assert.Equal(t, expected, dll.Get())
}

func TestDoubleListRemoveValue_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")

	assert.True(t, dll.RemoveValue("b"))
	expected := []string{"a", "c"}
	assert.Equal(t, expected, dll.Get())

	assert.False(t, dll.RemoveValue("z"))
}

func TestDoubleList_GetReverse_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")
	
	assert.Equal(t, []string{"c", "b", "a"}, dll.GetReverse())
}

func TestDoubleList_InsertEdges_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("b")
	
	assert.True(t, dll.InsertBeforeValue("b", "a"))
	assert.Equal(t, []string{"a", "b"}, dll.Get())
	
	assert.True(t, dll.InsertAfterValue("b", "c"))
	assert.Equal(t, []string{"a", "b", "c"}, dll.Get())
	
	assert.False(t, dll.InsertBeforeValue("zzz", "x"))
	assert.False(t, dll.InsertAfterValue("zzz", "x"))
}

func TestDoubleList_RemoveBeforeAfter_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")
	dll.PushBack("d")
	
	// Удаление перед значением
	assert.True(t, dll.RemoveBeforeValue("c")) 
	assert.Equal(t, []string{"a", "c", "d"}, dll.Get())
	
	// Удаление после значения
	assert.True(t, dll.RemoveAfterValue("a")) 
	assert.Equal(t, []string{"a", "d"}, dll.Get())
	
	// Крайние случаи
	assert.False(t, dll.RemoveBeforeValue("a")) 
	assert.False(t, dll.RemoveAfterValue("d"))  
	assert.False(t, dll.RemoveBeforeValue("zzz"))
}

func TestDoubleList_GetHelpers_A(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")
	
	assert.Equal(t, []string{"c"}, dll.GetAfterValue("b"))
	assert.Empty(t, dll.GetAfterValue("c"))      
	assert.Empty(t, dll.GetAfterValue("zzz"))   
	
	assert.Equal(t, []string{"a"}, dll.GetBeforeValue("b"))
	assert.Empty(t, dll.GetBeforeValue("a"))      
	assert.Nil(t, dll.GetBeforeValue("zzz"))     
	
	val, ok := dll.GetValue("b")
	assert.True(t, ok)
	assert.Equal(t, "b", val)
	
	_, ok = dll.GetValue("zzz")
	assert.False(t, ok)
}