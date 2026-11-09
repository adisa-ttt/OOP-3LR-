package source

import "testing"

func TestQueueBasicOps_T(t *testing.T) {
	q := NewQueue()
	if !q.Empty() {
		t.Fatal("New queue should be empty")
	}

	q.Push("first")
	q.Push("second")

	if q.Empty() {
		t.Error("Queue should not be empty after pushes")
	}

	expected := []string{"first", "second"}
	result := q.Get()

	if len(result) != len(expected) {
		t.Fatalf("Length mismatch: got %d, want %d", len(result), len(expected))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("Index %d: got '%s', want '%s'", i, result[i], v)
		}
	}
}

func TestQueuePop_T(t *testing.T) {
	q := NewQueue()
	q.Push("a")
	q.Push("b")

	val, err := q.Pop()
	if err != nil || val != "a" {
		t.Errorf("Pop failed: got '%s', want 'a'", val)
	}

	val, err = q.Pop()
	if err != nil || val != "b" {
		t.Errorf("Pop failed: got '%s', want 'b'", val)
	}

	if !q.Empty() {
		t.Error("Queue should be empty after popping all elements")
	}
}

func TestQueuePopEmpty_T(t *testing.T) {
	q := NewQueue()
	_, err := q.Pop()
	if err == nil {
		t.Error("Pop from empty queue should return error")
	}
}

func TestQueueClear_T(t *testing.T) {
	q := NewQueue()
	q.Push("x")
	q.Clear()
	if !q.Empty() {
		t.Error("Queue should be empty after clear")
	}
}