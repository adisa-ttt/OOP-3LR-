package source

import (
	"testing"
	"os"
)
func TestStackBasicOps_T(t *testing.T) {
	s := NewStack()
	if !s.Empty() {
		t.Fatal("New stack should be empty")
	}

	s.Push("first")
	s.Push("second")

	if s.Empty() {
		t.Error("Stack should not be empty after pushes")
	}

	expected := []string{"second", "first"}
	result := s.Get()

	if len(result) != len(expected) {
		t.Fatalf("Length mismatch: got %d, want %d", len(result), len(expected))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("Index %d: got '%s', want '%s'", i, result[i], v)
		}
	}
}

func TestStackPop_T(t *testing.T) {
	s := NewStack()
	s.Push("a")
	s.Push("b")

	val, err := s.Pop()
	if err != nil || val != "b" {
		t.Errorf("Pop failed: got '%s', want 'b'", val)
	}

	val, err = s.Pop()
	if err != nil || val != "a" {
		t.Errorf("Pop failed: got '%s', want 'a'", val)
	}

	if !s.Empty() {
		t.Error("Stack should be empty after popping all elements")
	}
}

func TestStackPopEmpty_T(t *testing.T) {
	s := NewStack()
	_, err := s.Pop()
	if err == nil {
		t.Error("Pop from empty stack should return error")
	}
}



func TestStackClear_T(t *testing.T) {
	s := NewStack()
	s.Push("x")
	s.Clear()
	if !s.Empty() {
		t.Error("Stack should be empty after clear")
	}
}


func TestStackSaveLoadTextEmpty_T(t *testing.T) {
	s := NewStack()
	err := s.SaveToText("/tmp/test_stack_empty.txt")
	if err != nil {
		t.Fatalf("SaveToText failed: %v", err)
	}
	s2 := NewStack()
	err = s2.LoadFromText("/tmp/test_stack_empty.txt")
	if err != nil {
		t.Fatalf("LoadFromText failed: %v", err)
	}
	if !s2.Empty() {
		t.Error("Loaded stack should be empty")
	}
	os.Remove("/tmp/test_stack_empty.txt")
}

func TestStackLoadTextNonexistent_T(t *testing.T) {
	s := NewStack()
	err := s.LoadFromText("/tmp/nonexistent_stack.txt")
	if err == nil {
		t.Error("LoadFromText should return error for nonexistent file")
	}
}
