package source

import (
	"db/hash"
	"fmt"
	"os"
	"encoding/binary"
	"bytes"
)

type Cell struct {
	key   int
	value string
	used  bool
}

type CuckooTable struct {
	table1 []Cell
	table2 []Cell
	size   int
	count  int
	seed1  uint32
	seed2  uint32
	kicks  int
}

const MaxKicks = 30

func NewCuckooTable(startSize ...int) *CuckooTable {
	initialSize := 16
	if len(startSize) > 0 {
		initialSize = startSize[0]
	}
	return &CuckooTable{
		table1: make([]Cell, initialSize),
		table2: make([]Cell, initialSize),
		size:   initialSize,
		seed1:  0x9E3779B9,
		seed2:  0x85EBCA6B,
		kicks:  0,
	}
}

func (cc *CuckooTable) hash1(key int) int {
	return int(hash.Murmur3_32(uint32(key), cc.seed1) % uint32(cc.size))
}

func (cc *CuckooTable) hash2(key int) int {
	return int(hash.Murmur3_32(uint32(key), cc.seed2) % uint32(cc.size))
}

func (cc *CuckooTable) nextSeed(s uint32) uint32 {
	return s*1664525 + 1013904223
}

func (cc *CuckooTable) allocTables(newSize int) {
	cc.table1 = make([]Cell, newSize)
	cc.table2 = make([]Cell, newSize)
	cc.size = newSize
}

func (cc *CuckooTable) placeInto(key int, value string) bool {
	currentValue := value
	for i := 0; i < MaxKicks; i++ {
		idx1 := cc.hash1(key)
		if !cc.table1[idx1].used {
			cc.table1[idx1] = Cell{key: key, value: currentValue, used: true}
			return true
		}
		key, cc.table1[idx1].key = cc.table1[idx1].key, key
		currentValue, cc.table1[idx1].value = cc.table1[idx1].value, currentValue
		cc.kicks++

		idx2 := cc.hash2(key)
		if !cc.table2[idx2].used {
			cc.table2[idx2] = Cell{key: key, value: currentValue, used: true}
			return true
		}
		key, cc.table2[idx2].key = cc.table2[idx2].key, key
		currentValue, cc.table2[idx2].value = cc.table2[idx2].value, currentValue
		cc.kicks++
	}
	return false
}

func (cc *CuckooTable) rehash() {
	oldSize := cc.size
	newSize := oldSize * 2

	oldT1 := cc.table1
	oldT2 := cc.table2
	cc.seed1 = cc.nextSeed(cc.seed1)
	cc.seed2 = cc.nextSeed(cc.seed2)

	cc.allocTables(newSize)
	cc.count = 0

	for i := 0; i < oldSize; i++ {
		if oldT1[i].used {
			cc.placeInto(oldT1[i].key, oldT1[i].value)
			cc.count++
		}
		if oldT2[i].used {
			cc.placeInto(oldT2[i].key, oldT2[i].value)
			cc.count++
		}
	}
}

func (cc *CuckooTable) Empty() bool {
	return cc.count == 0
}

func (cc *CuckooTable) CuckooInsert(key int, value string) bool {
	if cc.CuckooFind(key) {
		idx1 := cc.hash1(key)
		if cc.table1[idx1].used && cc.table1[idx1].key == key {
			cc.table1[idx1].value = value
			return false
		}
		idx2 := cc.hash2(key)
		if cc.table2[idx2].used && cc.table2[idx2].key == key {
			cc.table2[idx2].value = value
			return false
		}
	}
	if cc.count*2 >= cc.size {
		cc.rehash()
	}
	if !cc.placeInto(key, value) {
		cc.rehash()
		if !cc.placeInto(key, value) {
			return false
		}
	}
	cc.count++
	return true
}

func (cc *CuckooTable) CuckooFind(key int) bool {
	if cc.table1 == nil {
		return false
	}
	idx1 := cc.hash1(key)
	if cc.table1[idx1].used && cc.table1[idx1].key == key {
		return true
	}
	idx2 := cc.hash2(key)
	if cc.table2[idx2].used && cc.table2[idx2].key == key {
		return true
	}
	return false
}

func (cc *CuckooTable) GetValue(key int) (string, bool) {
	if cc.table1 == nil {
		return "", false
	}
	idx1 := cc.hash1(key)
	if cc.table1[idx1].used && cc.table1[idx1].key == key {
		return cc.table1[idx1].value, true
	}
	idx2 := cc.hash2(key)
	if cc.table2[idx2].used && cc.table2[idx2].key == key {
		return cc.table2[idx2].value, true
	}
	return "", false
}

func (cc *CuckooTable) CuckooRemove(key int) bool {
	if cc.table1 == nil {
		return false
	}

	idx1 := cc.hash1(key)
	if cc.table1[idx1].used && cc.table1[idx1].key == key {
		cc.table1[idx1].used = false
		cc.table1[idx1].value = ""
		cc.count--
		return true
	}

	idx2 := cc.hash2(key)
	if cc.table2[idx2].used && cc.table2[idx2].key == key {
		cc.table2[idx2].used = false
		cc.table2[idx2].value = ""
		cc.count--
		return true
	}

	return false
}

func (cc *CuckooTable) Clear() {
	cc.table1 = nil
	cc.table2 = nil
	cc.size = 0
	cc.count = 0
}

func (cc *CuckooTable) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, cc.count)
	for i := 0; i < cc.size; i++ {
		if cc.table1[i].used {
			fmt.Fprintf(file, "%d %s\n", cc.table1[i].key, cc.table1[i].value)
		}
		if cc.table2[i].used {
			fmt.Fprintf(file, "%d %s\n", cc.table2[i].key, cc.table2[i].value)
		}
	}
	return nil
}

func (cc *CuckooTable) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	cc.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var key int
		var value string
		fmt.Fscan(file, &key, &value)
		cc.CuckooInsert(key, value)
	}
	return nil
}


func (cc *CuckooTable) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, int32(cc.count))
	for i := 0; i < cc.size; i++ {
		if cc.table1[i].used {
			binary.Write(&buf, binary.LittleEndian, int32(cc.table1[i].key))
			val := []byte(cc.table1[i].value)
			binary.Write(&buf, binary.LittleEndian, int32(len(val)))
			buf.Write(val)
		}
		if cc.table2[i].used {
			binary.Write(&buf, binary.LittleEndian, int32(cc.table2[i].key))
			val := []byte(cc.table2[i].value)
			binary.Write(&buf, binary.LittleEndian, int32(len(val)))
			buf.Write(val)
		}
	}
	_, err = file.Write(buf.Bytes())
	return err
}

func (cc *CuckooTable) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

cc.table1 = make([]Cell, 16)
    cc.table2 = make([]Cell, 16)
    cc.size = 16
    cc.count = 0
    cc.seed1 = 0x9E3779B9
    cc.seed2 = 0x85EBCA6B	
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var key int32
		binary.Read(file, binary.LittleEndian, &key)
		var valLen int32
		binary.Read(file, binary.LittleEndian, &valLen)
		val := make([]byte, valLen)
		binary.Read(file, binary.LittleEndian, val)
		cc.CuckooInsert(int(key), string(val))
	}
	return nil
}