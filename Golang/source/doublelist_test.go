package source

import (
	"reflect"
	"testing")

func TestDoubleListBasicOps_T(t *testing.T) {
	dll := NewDoubleLinkList()
	if !dll.Empty() {
		t.Fatal("New list should be empty")
	}

	dll.PushFront("b")
	dll.PushFront("a")
	dll.PushBack("c")

	expected := []string{"a", "b", "c"}
	result := dll.Get()
	if len(result) != len(expected) {
		t.Fatalf("Get failed: got %v, want %v", result, expected)
	}

	reverseExpected := []string{"c", "b", "a"}
	reverseResult := dll.GetReverse()
	if len(reverseResult) != len(reverseExpected) {
		t.Fatalf("GetReverse failed: got %v, want %v", reverseResult, reverseExpected)
	}
}

func TestDoubleList_ClearAndGetAt_T(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")

	val, err := dll.GetAt(1)
	if err != nil || val != "b" {
		t.Errorf("GetAt failed: got '%s', want 'b'", val)
	}

	dll.Clear()
	if !dll.Empty() {
		t.Error("List should be empty after clear")
	}
}

func TestDoubleListPop_T(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("first")
	dll.PushBack("second")

	val, err := dll.PopFront()
	if err != nil || val != "first" {
		t.Errorf("PopFront failed: got '%s', want 'first'", val)
	}

	val, err = dll.PopBack()
	if err != nil || val != "second" {
		t.Errorf("PopBack failed: got '%s', want 'second'", val)
	}

	if !dll.Empty() {
		t.Error("List should be empty after popping all elements")
	}
}

func TestDoubleListPopSingle_T(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("solo")

	val, err := dll.PopBack()
	if err != nil || val != "solo" {
		t.Errorf("PopBack single failed: got '%s', want 'solo'", val)
	}
	if !dll.Empty() {
		t.Error("List should be empty after popping last element")
	}
}

func TestDoubleListInsertEdges_T(t *testing.T) {
	dll := NewDoubleLinkList()

	if dll.InsertBeforeValue("a", "z"){
		t.Error("error")
	}

	dll.PushBack("b")
	dll.PushBack("a")

	if !dll.InsertBeforeValue("a", "z") {
		t.Error("InsertBeforeValue head should succeed")
	}

	if !dll.InsertAfterValue("b", "c") {
		t.Error("InsertAfterValue tail should succeed")
	}

	if !dll.InsertBeforeValue("z", "NEW") {
		t.Error("InsertBeforeValue head should succeed")
	}

	expected := []string{"NEW", "z", "a", "b", "c"}
	result := dll.Get()
	if len(result) != len(expected) {
		t.Fatalf("Insert edges failed: got %v, want %v", result, expected)
	}
}


func TestInsertBefoeTail(t *testing.T) {
	dll := &DoubleLinkList{}

	dll.PushBack("a");
	dll.PushBack("b");
	dll.PushBack("c");
	if dll.InsertBeforeValue("z", "a") {
		t.Error("befor tail")
	}

}


func TestInsertBeforeValue_EmptyList(t *testing.T) {
	dll := &DoubleLinkList{}

	if dll.InsertBeforeValue("x", "new") {
		t.Error("expected false for empty list")
	}
	if dll.head != nil || dll.tail != nil {
		t.Error("empty list must stay empty")
	}
}

func collectForward(dll *DoubleLinkList) []string {
	var res []string
	for n := dll.head; n != nil; n = n.next {
		res = append(res, n.data)
	}
	return res
}

func TestInsertBeforeValue_BeforeTail(t *testing.T) {
	dll := &DoubleLinkList{}
	dll.PushBack("a")
	dll.PushBack("b")

	// вставка перед последним элементом -> a, new, b
	if !dll.InsertBeforeValue("b", "new") {
		t.Fatal("expected true")
	}
	if got := collectForward(dll); !reflect.DeepEqual(got, []string{"a", "new", "b"}) {
		t.Errorf("got %v", got)
	}
	if dll.tail.data != "b" {
		t.Error("tail must remain b")
	}
}


func TestDoubleListRemoveValue_T(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")

	if !dll.RemoveValue("a") {
		t.Error("RemoveValue head should succeed")
	}
	if !dll.RemoveValue("c") {
		t.Error("RemoveValue tail should succeed")
	}
	if !dll.RemoveValue("b") {
		t.Error("RemoveValue middle should succeed")
	}
	if !dll.Empty() {
		t.Error("List should be empty after removing all")
	}
}

func TestDoubleListRemoveBeforeAfter_T(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")
	dll.PushBack("d")

	if !dll.RemoveBeforeValue("c") {
		t.Error("RemoveBeforeValue should succeed")
	}
	if !dll.RemoveAfterValue("a") {
		t.Error("RemoveAfterValue should succeed")
	}

	expected := []string{"a", "d"}
	result := dll.Get()
	if len(result) != len(expected) {
		t.Errorf("Remove before/after failed: got %v, want %v", result, expected)
	}
}

func TestDoubleListGetHelpers_T(t *testing.T) {
	dll := NewDoubleLinkList()
	dll.PushBack("a")
	dll.PushBack("b")
	dll.PushBack("c")

	after := dll.GetAfterValue("b")
	if len(after) != 1 || after[0] != "c" {
		t.Errorf("GetAfterValue failed: got %v", after)
	}

	before := dll.GetBeforeValue("b")
	if len(before) != 1 || before[0] != "a" {
		t.Errorf("GetBeforeValue failed: got %v", before)
	}

	val, ok := dll.GetValue("b")
	if !ok || val != "b" {
		t.Errorf("GetValue failed: got '%s' (ok=%v)", val, ok)
	}
}
