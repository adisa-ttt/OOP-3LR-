package source

import (
	"fmt"
	"os"
	"errors"
	"encoding/binary"
	"bytes"
)

type LNode struct {
	data string
	next *LNode
	prev *LNode
}

type DoubleLinkList struct {
	head *LNode
	tail *LNode
}

func NewDoubleLinkList() *DoubleLinkList {
	return &DoubleLinkList{}
}

func (dll *DoubleLinkList) Empty() bool {
	return dll.head == nil
}

func (dll *DoubleLinkList) Clear() {
	dll.head = nil
	dll.tail = nil
}

func (dll *DoubleLinkList) PushFront(str string) {
	newNode := &LNode{data: str, next: dll.head, prev: nil}
	if dll.head == nil {
		dll.tail = newNode
	} else {
		dll.head.prev = newNode
	}
	dll.head = newNode
}

func (dll *DoubleLinkList) PushBack(str string) {
	newNode := &LNode{data: str, next: nil, prev: dll.tail}
	if dll.tail == nil {
		dll.head = newNode
	} else {
		dll.tail.next = newNode
	}
	dll.tail = newNode
}

func (dll *DoubleLinkList) PopFront() (string, error) {
	if dll.Empty() {
		return "", errors.New("double linked list is empty!")
	}
	popped := dll.head
	data := popped.data
	dll.head = popped.next
	if dll.head == nil {
		dll.tail = nil
	} else {
		dll.head.prev = nil
	}
	return data, nil
}

func (dll *DoubleLinkList) PopBack() (string, error) {
	if dll.Empty() {
		return "", errors.New("double linked list is empty!")
	}
	popped := dll.tail
	data := popped.data
	dll.tail = popped.prev
	if dll.tail == nil {
		dll.head = nil
	} else {
		dll.tail.next = nil
	}
	return data, nil
}

func (dll *DoubleLinkList) GetAt(index int) (string, error) {
	if index < 0 || dll.Empty() {
		return "", errors.New("index can not be smaller than 0!")
	}
	curr := dll.head
	n := 0
	for n != index && curr != nil {
		curr = curr.next
		n++
	}
	if curr == nil {
		return "", errors.New("index can not be bigger than list size!")
	}
	return curr.data, nil
}

func (dll *DoubleLinkList) InsertBeforeValue(value, str string) bool {
	if dll.Empty() {
		return false
	}
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil {
		return false
	}
	
	if valueNode == dll.head {
		dll.PushFront(str)
		return true
	}
	
	left := valueNode.prev
	newNode := &LNode{data: str, next: valueNode, prev: left}
	left.next = newNode
	valueNode.prev = newNode
	return true
}

func (dll *DoubleLinkList) InsertAfterValue(value, str string) bool {
	if dll.Empty() {
		return false
	}
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil {
		return false
	}
	if valueNode == dll.tail {
		dll.PushBack(str)
		return true
	}
	right := valueNode.next
	newNode := &LNode{data: str, next: right, prev: valueNode}
	valueNode.next = newNode
	right.prev = newNode
	return true
}

func (dll *DoubleLinkList) RemoveValue(value string) bool {
	if dll.Empty() {
		return false
	}
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil {
		return false
	}
	if valueNode == dll.head {
		_, _ = dll.PopFront()
		return true
	}
	if valueNode == dll.tail {
		_, _ = dll.PopBack()
		return true
	}
	left := valueNode.prev
	right := valueNode.next
	left.next = right
	right.prev = left
	return true
}

func (dll *DoubleLinkList) RemoveBeforeValue(value string) bool {
	if dll.Empty() || dll.head.data == value {
		return false
	}
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil {
		return false
	}
	toRemove := valueNode.prev
	if toRemove == dll.head {
		dll.head = toRemove.next
		if dll.head == nil {
			dll.tail = nil
		} else {
			dll.head.prev = nil
		}
		return true
	}
	left := toRemove.prev
	right := toRemove.next
	left.next = right
	right.prev = left
	return true
}

func (dll *DoubleLinkList) RemoveAfterValue(value string) bool {
	if dll.Empty() {
		return false
	}
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil || valueNode.next == nil {
		return false
	}
	toRemove := valueNode.next
	if toRemove == dll.tail {
		dll.tail = toRemove.prev
		if dll.tail == nil {
			dll.head = nil
		} else {
			dll.tail.next = nil
		}
		return true
	}
	left := toRemove.prev
	right := toRemove.next
	left.next = right
	right.prev = left
	return true
}

func (dll *DoubleLinkList) Find(value string) int {
	n := 0
	for c := dll.head; c != nil; c = c.next {
		if c.data == value {
			return n
		}
		n++
	}
	return -1
}

func (dll *DoubleLinkList) Get() []string {
	var result []string
	for curr := dll.head; curr != nil; curr = curr.next {
		result = append(result, curr.data)
	}
	return result
}

func (dll *DoubleLinkList) GetReverse() []string {
	var result []string
	for curr := dll.tail; curr != nil; curr = curr.prev {
		result = append(result, curr.data)
	}
	return result
}

func (dll *DoubleLinkList) GetAfterValue(value string) []string {
	var result []string
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil {
		return result
	}
	for c := valueNode.next; c != nil; c = c.next {
		result = append(result, c.data)
	}
	return result
}

func (dll *DoubleLinkList) GetBeforeValue(value string) []string {
	var result []string
	curr := dll.head
	for curr != nil && curr.data != value {
		result = append(result, curr.data)
		curr = curr.next
	}
	if curr == nil {
		return nil
	}
	return result
}

func (dll *DoubleLinkList) GetValue(value string) (string, bool) {
	valueNode := dll.head
	for valueNode != nil && valueNode.data != value {
		valueNode = valueNode.next
	}
	if valueNode == nil {
		return "", false
	}
	return valueNode.data, true
}

func (dll *DoubleLinkList) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := dll.Get()
	fmt.Fprintln(file, len(elems))
	for _, e := range elems {
		fmt.Fprintln(file, e)
	}
	return nil
}

func (dll *DoubleLinkList) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	dll.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var s string
		fmt.Fscan(file, &s)
		dll.PushBack(s)
	}
	return nil
}

func (dll *DoubleLinkList) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := dll.Get()
	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, int32(len(elems)))
	for _, e := range elems {
		str := []byte(e)
		binary.Write(&buf, binary.LittleEndian, int32(len(str)))
		buf.Write(str)
	}
	_, err = file.Write(buf.Bytes())
	return err
}

func (dll *DoubleLinkList) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	dll.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var strLen int32
		binary.Read(file, binary.LittleEndian, &strLen)
		str := make([]byte, strLen)
		binary.Read(file, binary.LittleEndian, str)
		dll.PushBack(string(str))
	}
	return nil
}