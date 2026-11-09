package source

import "testing"

func TestLinkListBasicOps_T(t *testing.T) {
	ll := NewLinkList()
	if !ll.Empty() {
		t.Fatal("New list should be empty")
	}

	ll.PushFront("b")
	ll.PushFront("a")
	ll.PushBack("c")

	expected := []string{"a", "b", "c"}
	result := ll.Get()

	if len(result) != len(expected) {
		t.Fatalf("Length mismatch: got %d, want %d", len(result), len(expected))
	}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("Index %d: got '%s', want '%s'", i, result[i], v)
		}
	}
}

func TestLinkListPop_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("first")
	ll.PushBack("second")

	val, err := ll.PopFront()
	if err != nil || val != "first" {
		t.Errorf("PopFront failed: got '%s', want 'first'", val)
	}

	val, err = ll.PopBack()
	if err != nil || val != "second" {
		t.Errorf("PopBack failed: got '%s', want 'second'", val)
	}

	if !ll.Empty() {
		t.Error("List should be empty after popping all elements")
	}
}

func TestLinkListPopSingle_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("only")

	val, err := ll.PopBack()
	if err != nil || val != "only" {
		t.Errorf("PopBack single element failed: got '%s', want 'only'", val)
	}
	if !ll.Empty() {
		t.Error("List should be empty after popping last element")
	}
}

func TestLinkListRemoveValue_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")

	if !ll.RemoveValue("b") {
		t.Error("RemoveValue('b') should return true")
	}
	expected := []string{"a", "c"}
	result := ll.Get()
	if len(result) != len(expected) || result[0] != expected[0] || result[1] != expected[1] {
		t.Errorf("After remove got: %v, want: %v", result, expected)
	}

	if ll.RemoveValue("z") {
		t.Error("RemoveValue('z') should return false")
	}
}

func TestLinkListInsertEdges_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("b")

	if !ll.InsertBeforeValue("b", "a") {
		t.Error("InsertBeforeValue head should succeed")
	}
	if !ll.InsertAfterValue("b", "c") {
		t.Error("InsertAfterValue tail should succeed")
	}

	expected := []string{"a", "b", "c"}
	result := ll.Get()
	if len(result) != len(expected) {
		t.Fatalf("Insert edges failed: got %v, want %v", result, expected)
	}

	if ll.InsertBeforeValue("zzz", "x") {
		t.Error("InsertBeforeValue non-existent should fail")
	}
	if ll.InsertAfterValue("zzz", "x") {
		t.Error("InsertAfterValue non-existent should fail")
	}
}

func TestLinkListRemoveBeforeAfter_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")
	ll.PushBack("d")

	if !ll.RemoveBeforeValue("c") {
		t.Error("RemoveBeforeValue should succeed")
	}
	if !ll.RemoveAfterValue("a") {
		t.Error("RemoveAfterValue should succeed")
	}

	expected := []string{"a", "d"}
	result := ll.Get()
	if len(result) != len(expected) {
		t.Errorf("Remove before/after failed: got %v, want %v", result, expected)
	}

	if ll.RemoveBeforeValue("a") {
		t.Error("RemoveBeforeValue head should fail")
	}
	if ll.RemoveAfterValue("d") {
		t.Error("RemoveAfterValue tail should fail")
	}
}

func TestLinkListGetHelpers_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")

	after := ll.GetAfterValue("b")
	if len(after) != 1 || after[0] != "c" {
		t.Errorf("GetAfterValue failed: got %v, want [c]", after)
	}

	before := ll.GetBeforeValue("b")
	if len(before) != 1 || before[0] != "a" {
		t.Errorf("GetBeforeValue failed: got %v, want [a]", before)
	}

	val, ok := ll.GetValue("b")
	if !ok || val != "b" {
		t.Errorf("GetValue failed: got '%s' (ok=%v), want 'b' (ok=true)", val, ok)
	}

	_, ok = ll.GetValue("zzz")
	if ok {
		t.Error("GetValue missing key should return false")
	}
}

func TestLinkList_PopBackSingle_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("only")

	val, err := ll.PopBack()
	if err != nil || val != "only" {
		t.Errorf("PopBack single failed: got '%s', want 'only'", val)
	}
	if !ll.Empty() {
		t.Error("List should be empty after popping last element")
	}
}

func TestLinkList_GetAtErrors_T(t *testing.T) {
	ll := NewLinkList()
	
	_, err := ll.GetAt(-1)
	if err == nil {
		t.Error("GetAt(-1) on empty list should return error")
	}
	
	_, err = ll.GetAt(0)
	if err == nil {
		t.Error("GetAt(0) on empty list should return error")
	}

	ll.PushBack("a")
	_, err = ll.GetAt(1)
	if err == nil {
		t.Error("GetAt(1) out of bounds should return error")
	}
}

func TestLinkList_InsertBeforeHead_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("b")
	
	if !ll.InsertBeforeValue("b", "a") {
		t.Error("InsertBeforeValue head should succeed")
	}
	
	expected := []string{"a", "b"}
	result := ll.Get()
	if len(result) != len(expected) || result[0] != expected[0] || result[1] != expected[1] {
		t.Errorf("Insert before head failed: got %v, want %v", result, expected)
	}
}

func TestLinkList_RemoveEdges_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("head")
	ll.PushBack("mid")
	ll.PushBack("tail")

	if !ll.RemoveValue("head") {
		t.Error("RemoveValue head should succeed")
	}
	if !ll.RemoveValue("tail") {
		t.Error("RemoveValue tail should succeed")
	}
	
	result := ll.Get()
	if len(result) != 1 || result[0] != "mid" {
		t.Errorf("Remove edges failed: got %v, want [mid]", result)
	}
}

func TestLinkList_Clear_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.Clear()
	if !ll.Empty() {
		t.Error("List should be empty after clear")
	}
}

func TestLinkList_RemoveBeforeAfterEdges_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")
	ll.PushBack("c")
	ll.PushBack("d")

	if !ll.RemoveBeforeValue("b") {
		t.Error("RemoveBeforeValue('b') should remove 'a'")
	}
	if !ll.RemoveAfterValue("c") {
		t.Error("RemoveAfterValue('c') should remove 'd' and update tail")
	}
	
	expected := []string{"b", "c"}
	result := ll.Get()
	if len(result) != len(expected) || result[0] != expected[0] || result[1] != expected[1] {
		t.Errorf("Remove edges failed: got %v, want %v", result, expected)
	}
}

func TestLinkList_GetHelpersEdges_T(t *testing.T) {
	ll := NewLinkList()
	ll.PushBack("a")
	ll.PushBack("b")

	after := ll.GetAfterValue("b")
	if len(after) != 0 {
		t.Errorf("GetAfterValue tail should be empty, got %v", after)
	}
	
	before := ll.GetBeforeValue("a")
	if len(before) != 0 {
		t.Errorf("GetBeforeValue head should be empty, got %v", before)
	}

	afterMissing := ll.GetAfterValue("z")
	if len(afterMissing) != 0 {
		t.Errorf("GetAfterValue missing should be empty, got %v", afterMissing)
	}

	beforeMissing := ll.GetBeforeValue("z")
	if beforeMissing != nil {
		t.Errorf("GetBeforeValue missing should be nil, got %v", beforeMissing)
	}
}