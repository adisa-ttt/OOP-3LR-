package source

import "testing"

func TestArrayEmpty_T(t *testing.T) {
	a := NewArray()
	if !a.Empty() {
		t.Error("New array should be empty")
	}
	if a.GetSize() != 0 {
		t.Errorf("Expected size 0, got %d", a.GetSize())
	}
}

func TestArrayPushBack_T(t *testing.T) {
	a := NewArray()
	a.PushBack("hello")
	a.PushBack("world")

	if a.Empty() {
		t.Error("Array should not be empty after push")
	}
	if a.GetSize() != 2 {
		t.Errorf("Expected size 2, got %d", a.GetSize())
	}

	val, err := a.GetAt(0)
	if err != nil || val != "hello" {
		t.Errorf("GetAt(0) failed: got '%s', want 'hello'", val)
	}
}

func TestArrayInsertAndRemove_T(t *testing.T) {
	a := NewArray()
	a.PushBack("b")
	a.PushBack("d")

	if !a.Insert(0, "a") {
		t.Error("Insert at 0 should succeed")
	}

	data := a.Get()
	if len(data) != 3 || data[0] != "a" || data[1] != "b" {
		t.Errorf("Insert failed. Got: %v", data)
	}

	if !a.RemoveAt(1) {
		t.Error("RemoveAt(1) should succeed")
	}
	expected := []string{"a", "d"}
	result := a.Get()
	if len(result) != len(expected) || result[0] != expected[0] || result[1] != expected[1] {
		t.Errorf("After remove got: %v, want: %v", result, expected)
	}
}

func TestArrayResizeOnInsert_T(t *testing.T) {
	a := NewArray()
	for i := 0; i < StartCapacity; i++ {
		a.PushBack("old")
	}

	if !a.Insert(1, "new") {
		t.Error("Insert should trigger resize and succeed")
	}
	if a.GetSize() != StartCapacity+1 {
		t.Errorf("Expected size %d after insert+resize, got %d", StartCapacity+1, a.GetSize())
	}
}

func TestArraySet_T(t *testing.T) {
	a := NewArray()
	a.PushBack("x")

	if !a.Set(0, "y") {
		t.Error("Set valid index should return true")
	}
	val, _ := a.GetAt(0)
	if val != "y" {
		t.Errorf("Set failed: got '%s', want 'y'", val)
	}

	if a.Set(-1, "z") {
		t.Error("Set negative index should return false")
	}
	if a.Set(1, "z") {
		t.Error("Set out of bounds index should return false")
	}
}

func TestArrayClear_T(t *testing.T) {
	a := NewArray()
	a.PushBack("x")
	a.Clear()

	if !a.Empty() {
		t.Error("Array should be empty after clear")
	}
	if a.GetSize() != 0 {
		t.Errorf("Expected size 0 after clear, got %d", a.GetSize())
	}
}