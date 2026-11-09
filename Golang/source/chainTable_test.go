package source

import "testing"

func TestChainTableBasicOps_T(t *testing.T) {
	ct := NewChainTable()
	if !ct.Empty() {
		t.Fatal("New chain table should be empty")
	}

	if !ct.ChainInsert(1, "one") {
		t.Error("First insert should return true")
	}
	if ct.ChainInsert(1, "two") {
		t.Error("Duplicate insert should return false")
	}

	if ct.Empty() {
		t.Error("Table should not be empty")
	}
	if !ct.ChainFind(1) {
		t.Error("Should find key 1")
	}
	if ct.ChainFind(3) {
		t.Error("Should not find key 3")
	}
}

func TestChainTableGetValue_T(t *testing.T) {
	ct := NewChainTable()
	ct.ChainInsert(10, "ten")

	val, ok := ct.GetValue(10)
	if !ok || val != "ten" {
		t.Errorf("GetValue failed: got '%s' (ok=%v)", val, ok)
	}

	_, ok = ct.GetValue(99)
	if ok {
		t.Error("GetValue for missing key should return false")
	}
}

func TestChainTableGetValueEmpty_T(t *testing.T) {
	ct := NewChainTable()
	_, ok := ct.GetValue(1)
	if ok {
		t.Error("GetValue on empty table should return false")
	}
}

func TestChainTableFindEmpty_T(t *testing.T) {
	ct := NewChainTable()
	if ct.ChainFind(1) {
		t.Error("ChainFind on empty table should return false")
	}
}

func TestChainTable_Clear_T(t *testing.T) {
	ct := NewChainTable(8)
	ct.ChainInsert(1, "one")
	ct.Clear()
	if !ct.Empty() {
		t.Error("Table should be empty after clear")
	}
}

func TestChainTableRehash_T(t *testing.T) {
	ct := NewChainTable(2)

	for i := 0; i < 5; i++ {
		if !ct.ChainInsert(i, string(rune('a'+i))) {
			t.Fatalf("Insert %d failed during rehash test", i)
		}
	}

	for i := 0; i < 5; i++ {
		if !ct.ChainFind(i) {
			t.Errorf("Key %d not found after rehash", i)
		}
	}
}

func TestChainTableRemove_T(t *testing.T) {
	ct := NewChainTable(8)
	ct.ChainInsert(1, "a")
	ct.ChainInsert(9, "b")

	if !ct.ChainRemove(1) {
		t.Error("Remove existing key should return true")
	}
	if ct.ChainFind(1) {
		t.Error("Key 1 should be gone")
	}
	if !ct.ChainFind(9) {
		t.Error("Key 9 should still exist")
	}
}

func TestChainTableMultipleInsertsAndRemoves_T(t *testing.T) {
	ct := NewChainTable(4)
	for i := 0; i < 20; i++ {
		if !ct.ChainInsert(i, string(rune('a'+i%26))) {
			t.Fatalf("Insert %d failed", i)
		}
	}
	for i := 0; i < 20; i++ {
		if !ct.ChainRemove(i) {
			t.Errorf("Remove %d failed", i)
		}
	}
	if !ct.Empty() {
		t.Error("Table should be empty after removing all")
	}
}

func TestChainTableUpdateValue_T(t *testing.T) {
	ct := NewChainTable(8)
	ct.ChainInsert(1, "old")
	ct.ChainInsert(1, "new")
	val, ok := ct.GetValue(1)
	if !ok || val != "new" {
		t.Errorf("Value should be updated to 'new', got '%s'", val)
	}
}

func TestChainTable_InsertEmptyTable_T(t *testing.T) {
	ct := NewChainTable(8)
	ct.table = nil
	if ct.ChainInsert(1, "one") {
		t.Error("Insert into nil table should return false")
	}
}

func TestChainTable_FindEmptyTable_T(t *testing.T) {
	ct := NewChainTable(8)
	ct.table = nil
	if ct.ChainFind(1) {
		t.Error("Find on nil table should return false")
	}
}

func TestChainTable_RemoveEmptyTable_T(t *testing.T) {
	ct := NewChainTable(8)
	ct.table = nil
	if ct.ChainRemove(1) {
		t.Error("Remove from nil table should return false")
	}
}