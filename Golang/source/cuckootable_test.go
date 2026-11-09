package source

import (
	"testing"
)

func TestCuckooTableBasicOps_T(t *testing.T) {
	ct := NewCuckooTable()
	if !ct.Empty() {
		t.Fatal("New cuckoo table should be empty")
	}

	if !ct.CuckooInsert(1, "one") {
		t.Error("First insert should return true")
	}
	if ct.CuckooInsert(1, "two") {
		t.Error("Duplicate insert should return false")
	}

	if ct.Empty() {
		t.Error("Table should not be empty")
	}
	if !ct.CuckooFind(1) {
		t.Error("Should find key 1")
	}
	if ct.CuckooFind(3) {
		t.Error("Should not find key 3")
	}
}

func TestCuckooTableGetValue_T(t *testing.T) {
	ct := NewCuckooTable()
	ct.CuckooInsert(42, "answer")

	val, ok := ct.GetValue(42)
	if !ok || val != "answer" {
		t.Errorf("GetValue failed: got '%s' (ok=%v)", val, ok)
	}

	_, ok = ct.GetValue(0)
	if ok {
		t.Error("GetValue for missing key should return false")
	}
}

func TestCuckooTableManyInserts_T(t *testing.T) {
	ct := NewCuckooTable(4)
	n := 500

	for i := 0; i < n; i++ {
		ok := ct.CuckooInsert(i, string(rune('a'+i%26)))
		if !ok {
			t.Fatalf("Failed to insert key %d during bulk insert", i)
		}
	}

	for i := 0; i < n; i++ {
		if !ct.CuckooFind(i) {
			t.Errorf("Key %d not found after bulk insert", i)
		}
	}
}

func TestCuckooTableGetValueEmpty_T(t *testing.T) {
	ct := NewCuckooTable()
	_, ok := ct.GetValue(1)
	if ok {
		t.Error("GetValue on empty table should return false")
	}
}

func TestCuckooTableFindEmpty_T(t *testing.T) {
	ct := NewCuckooTable()
	if ct.CuckooFind(1) {
		t.Error("CuckooFind on empty table should return false")
	}
}

func TestCuckooTableRemove_T(t *testing.T) {
	ct := NewCuckooTable()
	ct.CuckooInsert(7, "seven")

	if !ct.CuckooRemove(7) {
		t.Error("Remove existing key should return true")
	}
	if ct.CuckooFind(7) {
		t.Error("Key 7 should be gone")
	}
}

func TestCuckooTable_Clear_T(t *testing.T) {
	ct := NewCuckooTable()
	ct.CuckooInsert(1, "one")
	ct.Clear()
	if !ct.Empty() {
		t.Error("Table should be empty after clear")
	}
}

func TestCuckooTableRemoveEmpty_T(t *testing.T) {
	ct := NewCuckooTable()
	if ct.CuckooRemove(1) {
		t.Error("CuckooRemove on empty table should return false")
	}
}


func TestCuckooTableUpdateValue_T(t *testing.T) {
	ct := NewCuckooTable()
	ct.CuckooInsert(1, "old")
	ct.CuckooInsert(1, "new")
	val, ok := ct.GetValue(1)
	if !ok || val != "new" {
		t.Errorf("Value should be updated to 'new', got '%s'", val)
	}
}

func TestCuckooTableMultipleRemoves_T(t *testing.T) {
	ct := NewCuckooTable(8)
	for i := 0; i < 20; i++ {
		ct.CuckooInsert(i, string(rune('a'+i%26)))
	}
	for i := 0; i < 20; i++ {
		if !ct.CuckooRemove(i) {
			t.Errorf("Remove %d failed", i)
		}
	}
	if !ct.Empty() {
		t.Error("Table should be empty after removing all")
	}
}

func TestCuckooTableMissingAndUpdate_T(t *testing.T) {
	ct := NewCuckooTable()
	if ct.CuckooRemove(1) {
		t.Error("Remove on empty table should return false")
	}

	for i := 0; i < 100; i++ {
		ct.CuckooInsert(i, "old")
	}
	for i := 1000; i < 1200; i++ {
		if ct.CuckooRemove(i) {
			t.Errorf("Remove of missing key %d should return false", i)
		}
		if ct.CuckooFind(i) {
			t.Errorf("Missing key %d should not be found", i)
		}
		if _, ok := ct.GetValue(i); ok {
			t.Errorf("GetValue of missing key %d should return false", i)
		}
	}

	// обновление существующих ключей (часть лежит во второй таблице)
	for i := 0; i < 100; i++ {
		if ct.CuckooInsert(i, "new") {
			t.Errorf("Insert of existing key %d should return false", i)
		}
		if val, ok := ct.GetValue(i); !ok || val != "new" {
			t.Errorf("Key %d: expected 'new', got '%s' (ok=%v)", i, val, ok)
		}
	}
}

func TestCuckooTableRemoveNonExistent_T(t *testing.T) {
	ct := NewCuckooTable()
	ct.CuckooInsert(1, "one")
	if ct.CuckooRemove(99) {
		t.Error("Remove non-existent key should return false")
	}
}

func TestCuckooTableRehash_T(t *testing.T) {
	ct := NewCuckooTable(2)
	for i := 0; i < 10; i++ {
		if !ct.CuckooInsert(i, string(rune('a'+i))) {
			t.Fatalf("Insert %d failed during rehash", i)
		}
	}
	for i := 0; i < 10; i++ {
		if !ct.CuckooFind(i) {
			t.Errorf("Key %d not found after rehash", i)
		}
	}
}