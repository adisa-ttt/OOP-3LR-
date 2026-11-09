package source

import (
	"fmt"
	"os"
	"encoding/binary"
	"bytes"
)

type ChainNode struct {
	key   int
	value string
	prev  *ChainNode
	next  *ChainNode
}

type ChainTable struct {
	table []*ChainNode
	size  int
	count int
}

func NewChainTable(startSize ...int) *ChainTable {
	initialSize := 8
	if len(startSize) > 0 {
		initialSize = startSize[0]
	}
	return &ChainTable{
		table: make([]*ChainNode, initialSize),
		size:  initialSize,
		count: 0,
	}
}

func (ct *ChainTable) hashKey(key int) int {
	return ((key % ct.size) + ct.size) % ct.size
}

func (ct *ChainTable) rehash() {
	oldSize := ct.size
	newSize := oldSize * 2
	newTable := make([]*ChainNode, newSize)

	oldTable := ct.table
	ct.table = newTable
	ct.size = newSize

	for i := 0; i < oldSize; i++ {
		curr := oldTable[i]
		for curr != nil {
			next := curr.next
			idx := ct.hashKey(curr.key)

			curr.prev = nil
			curr.next = ct.table[idx]
			if ct.table[idx] != nil {
				ct.table[idx].prev = curr
			}
			ct.table[idx] = curr
			curr = next
		}
	}
}

func (ct *ChainTable) Empty() bool {
	return ct.count == 0
}

func (ct *ChainTable) Clear() {
	ct.table = make([]*ChainNode, ct.size)
	ct.count = 0
}

func (ct *ChainTable) ChainInsert(key int, value string) bool {
	if ct.table == nil {
		return false
	}
	idx := ct.hashKey(key)
	for curr := ct.table[idx]; curr != nil; curr = curr.next {
		if curr.key == key {
			curr.value = value
			return false
		}
	}
	newNode := &ChainNode{key: key, value: value, next: ct.table[idx]}
	if ct.table[idx] != nil {
		ct.table[idx].prev = newNode
	}
	ct.table[idx] = newNode
	ct.count++
	if ct.count > ct.size {
		ct.rehash()
	}
	return true
}

func (ct *ChainTable) ChainFind(key int) bool {
	if ct.table == nil {
		return false
	}
	idx := ct.hashKey(key)
	for curr := ct.table[idx]; curr != nil; curr = curr.next {
		if curr.key == key {
			return true
		}
	}
	return false
}

func (ct *ChainTable) ChainRemove(key int) bool {
	if ct.table == nil {
		return false
	}
	idx := ct.hashKey(key)
	for curr := ct.table[idx]; curr != nil; curr = curr.next {
		if curr.key == key {
			if curr.prev != nil {
				curr.prev.next = curr.next
			} else {
				ct.table[idx] = curr.next
			}
			if curr.next != nil {
				curr.next.prev = curr.prev
			}
			ct.count--
			return true
		}
	}
	return false
}

func (ct *ChainTable) GetValue(key int) (string, bool) {
	if ct.table == nil {
		return "", false
	}
	idx := ct.hashKey(key)
	for curr := ct.table[idx]; curr != nil; curr = curr.next {
		if curr.key == key {
			return curr.value, true
		}
	}
	return "", false
}

func (ct *ChainTable) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, ct.count)
	for i := 0; i < ct.size; i++ {
		curr := ct.table[i]
		for curr != nil {
			fmt.Fprintf(file, "%d %s\n", curr.key, curr.value)
			curr = curr.next
		}
	}
	return nil
}

func (ct *ChainTable) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	ct.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var key int
		var value string
		fmt.Fscan(file, &key, &value)
		ct.ChainInsert(key, value)
	}
	return nil
}

func (ct *ChainTable) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, int32(ct.count))
	for i := 0; i < ct.size; i++ {
		curr := ct.table[i]
		for curr != nil {
			binary.Write(&buf, binary.LittleEndian, int32(curr.key))
			val := []byte(curr.value)
			binary.Write(&buf, binary.LittleEndian, int32(len(val)))
			buf.Write(val)
			curr = curr.next
		}
	}
	_, err = file.Write(buf.Bytes())
	return err
}

func (ct *ChainTable) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	ct.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var key int32
		binary.Read(file, binary.LittleEndian, &key)
		var valLen int32
		binary.Read(file, binary.LittleEndian, &valLen)
		val := make([]byte, valLen)
		binary.Read(file, binary.LittleEndian, val)
		ct.ChainInsert(int(key), string(val))
	}
	return nil
}