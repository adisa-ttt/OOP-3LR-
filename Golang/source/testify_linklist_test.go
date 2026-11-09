package source

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestLinkListBasicOps_A(t *testing.T) {
	ll := NewLinkList()
	assert.True(t, ll.Empty())

	ll.PushFront("b")
	ll.PushFront("a")
	ll.PushBack("c")

	expected := []string{"a", "b", "c"}
	assert.Equal(t, expected, ll.Get())
}

func TestLinkListPop_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("first")
	ll.PushBack("second")

	val, err := ll.PopFront()
	assert.NoError(t, err)
	assert.Equal(t, "first", val)

	val, err = ll.PopBack()
	assert.NoError(t, err)
	assert.Equal(t, "second", val)

	assert.True(t, ll.Empty())
}

func TestLinkListFindAndInsert_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("c")

	assert.Equal(t, 0, ll.Find("a"))
	assert.Equal(t, -1, ll.Find("z"))

	assert.True(t, ll.InsertAfterValue("a", "b"))
	expected := []string{"a", "b", "c"}
	assert.Equal(t, expected, ll.Get())
}

func TestLinkListRemoveValue_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")

	assert.True(t, ll.RemoveValue("b"))
	expected := []string{"a", "c"}
	assert.Equal(t, expected, ll.Get())

	assert.False(t, ll.RemoveValue("z"))
}

func TestLinkList_InsertEdges_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("b")
	
	assert.True(t, ll.InsertBeforeValue("b", "a"))
	assert.Equal(t, []string{"a", "b"}, ll.Get())
	
	assert.True(t, ll.InsertAfterValue("b", "c"))
	assert.Equal(t, []string{"a", "b", "c"}, ll.Get())
	
	assert.False(t, ll.InsertBeforeValue("zzz", "x"))
	assert.False(t, ll.InsertAfterValue("zzz", "x"))
}

func TestLinkList_RemoveBeforeAfter_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")
	ll.PushBack("d")
	
	assert.True(t, ll.RemoveBeforeValue("c"))
	assert.Equal(t, []string{"a", "c", "d"}, ll.Get())
	
	assert.True(t, ll.RemoveAfterValue("a")) 
	assert.Equal(t, []string{"a", "d"}, ll.Get())
	
	assert.False(t, ll.RemoveBeforeValue("a"))
	assert.False(t, ll.RemoveAfterValue("d"))
	assert.False(t, ll.RemoveBeforeValue("zzz"))
}

func TestLinkList_GetHelpers_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")
	
	assert.Equal(t, []string{"c"}, ll.GetAfterValue("b"))
	assert.Empty(t, ll.GetAfterValue("c"))
	assert.Empty(t, ll.GetAfterValue("zzz"))
	
	assert.Equal(t, []string{"a"}, ll.GetBeforeValue("b"))
	assert.Empty(t, ll.GetBeforeValue("a"))
	assert.Nil(t, ll.GetBeforeValue("zzz"))
	
	val, ok := ll.GetValue("b")
	assert.True(t, ok)
	assert.Equal(t, "b", val)
	
	_, ok = ll.GetValue("zzz")
	assert.False(t, ok)
}

func TestLinkList_PopBackSingle_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("solo")

	val, err := ll.PopBack()
	assert.NoError(t, err)
	assert.Equal(t, "solo", val)
	assert.True(t, ll.Empty())
}

func TestLinkList_GetAtErrors_A(t *testing.T) {
	ll := NewLinkList()
	
	_, err := ll.GetAt(-1)
	assert.Error(t, err)
	
	_, err = ll.GetAt(0)
	assert.Error(t, err)

	ll.PushBack("x")
	_, err = ll.GetAt(5)
	assert.Error(t, err)
}

func TestLinkList_InsertBeforeHead_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("target")
	
	assert.True(t, ll.InsertBeforeValue("target", "newHead"))
	assert.Equal(t, []string{"newHead", "target"}, ll.Get())
}

func TestLinkList_RemoveEdges_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("first")
	ll.PushBack("middle")
	ll.PushBack("last")

	assert.True(t, ll.RemoveValue("first"))
	assert.True(t, ll.RemoveValue("last"))
	assert.Equal(t, []string{"middle"}, ll.Get())
}

func TestLinkList_RemoveBeforeAfterEdges_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")
	ll.PushBack("d")

	assert.True(t, ll.RemoveBeforeValue("b"))
	assert.True(t, ll.RemoveAfterValue("c"))
	assert.Equal(t, []string{"b", "c"}, ll.Get())
}

func TestLinkList_GetHelpersEdges_A(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("x")
	ll.PushBack("y")

	assert.Empty(t, ll.GetAfterValue("y"))
	assert.Empty(t, ll.GetBeforeValue("x"))
	assert.Empty(t, ll.GetAfterValue("missing"))
	assert.Nil(t, ll.GetBeforeValue("missing"))
}