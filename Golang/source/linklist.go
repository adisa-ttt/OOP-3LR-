package source

import (
	"fmt"
	"os"
	"errors"
	"encoding/binary"
	"bytes"
)

type FNode struct {
	data string
	next *FNode
}

type LinkList struct {
	head *FNode
	tail *FNode
}

func NewLinkList() *LinkList {
	return &LinkList{}
}

func (ll *LinkList) Empty() bool {
	return ll.head == nil
}

func (ll *LinkList) Clear() {
	ll.head = nil
	ll.tail = nil
}

func (ll *LinkList) PushFront(str string) {
	newNode := &FNode{data: str, next: ll.head}
	if ll.head == nil {
		ll.tail = newNode
	}
	ll.head = newNode
}

func (ll *LinkList) PushBack(str string) {
	newNode := &FNode{data: str, next: nil}
	if ll.head == nil {
		ll.head = newNode
		ll.tail = newNode
	} else {
		ll.tail.next = newNode
		ll.tail = newNode
	}
}

func (ll *LinkList) PopFront() (string, error) {
	if ll.Empty() {
		return "", errors.New("list is empty!")
	}
	popped := ll.head
	data := popped.data
	ll.head = ll.head.next
	if ll.head == nil {
		ll.tail = nil
	}
	return data, nil
}

func (ll *LinkList) PopBack() (string, error) {
	if ll.Empty() {
		return "", errors.New("list is empty!")
	}
	if ll.head == ll.tail {
		data := ll.head.data
		ll.head = nil
		ll.tail = nil
		return data, nil
	}
	prevTail := ll.head
	for prevTail.next != ll.tail {
		prevTail = prevTail.next
	}
	data := ll.tail.data
	ll.tail = prevTail
	ll.tail.next = nil
	return data, nil
}

func (ll *LinkList) GetAt(index int) (string, error) {
	if index < 0 || ll.Empty() {
		return "", errors.New("index can not be smaller than 0!")
	}
	curr := ll.head
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

func (ll *LinkList) InsertBeforeValue(value, str string) bool {
	if ll.Empty() {
		return false
	}
	if ll.head.data == value {
		ll.PushFront(str)
		return true
	}
	prev := ll.head
	curr := ll.head.next
	for curr != nil && curr.data != value {
		prev = curr
		curr = curr.next
	}
	if curr == nil {
		return false
	}
	prev.next = &FNode{data: str, next: curr}
	return true
}

func (ll *LinkList) InsertAfterValue(value, str string) bool {
	if ll.Empty() {
		return false
	}
	curr := ll.head
	for curr != nil && curr.data != value {
		curr = curr.next
	}
	if curr == nil {
		return false
	}
	newNode := &FNode{data: str, next: curr.next}
	curr.next = newNode
	if newNode.next == nil {
		ll.tail = newNode
	}
	return true
}

func (ll *LinkList) RemoveValue(value string) bool {
	if ll.Empty() {
		return false
	}
	if ll.head.data == value {
		_, _ = ll.PopFront()
		return true
	}
	prev := ll.head
	curr := ll.head.next
	for curr != nil {
		if curr.data == value {
			if curr == ll.tail {
				_, _ = ll.PopBack()
				return true
			}
			prev.next = curr.next
			return true
		}
		prev = curr
		curr = curr.next
	}
	return false
}

func (ll *LinkList) RemoveBeforeValue(value string) bool {
	if ll.Empty() || ll.head.data == value {
		return false
	}
	var prevPrev *FNode
	prev := ll.head
	curr := ll.head.next
	for curr != nil && curr.data != value {
		prevPrev = prev
		prev = curr
		curr = curr.next
	}
	if curr == nil {
		return false
	}
	if prevPrev == nil {
		ll.head = prev.next
		return true
	}
	prevPrev.next = prev.next
	return true
}

func (ll *LinkList) RemoveAfterValue(value string) bool {
	if ll.Empty() {
		return false
	}
	curr := ll.head
	for curr != nil && curr.data != value {
		curr = curr.next
	}
	if curr == nil || curr == ll.tail {
		return false
	}
	toDelete := curr.next
	if toDelete == ll.tail {
		ll.tail = curr
		ll.tail.next = nil
		return true
	}
	curr.next = toDelete.next
	return true
}

func (ll *LinkList) Find(value string) int {
	n := 0
	for c := ll.head; c != nil; c = c.next {
		if c.data == value {
			return n
		}
		n++
	}
	return -1
}

func (ll *LinkList) Get() []string {
	var result []string
	for curr := ll.head; curr != nil; curr = curr.next {
		result = append(result, curr.data)
	}
	return result
}

func (ll *LinkList) GetAfterValue(value string) []string {
	var result []string
	if ll.Empty() {
		return result
	}
	curr := ll.head
	for curr != nil && curr.data != value {
		curr = curr.next
	}
	if curr == nil {
		return result
	}
	for c := curr.next; c != nil; c = c.next {
		result = append(result, c.data)
	}
	return result
}

func (ll *LinkList) GetBeforeValue(value string) []string {
	var result []string
	if ll.Empty() {
		return result
	}
	curr := ll.head
	for curr != nil && curr.data != value {
		result = append(result, curr.data)
		curr = curr.next
	}
	if curr == nil {
		return nil
	}
	return result
}

func (ll *LinkList) GetValue(value string) (string, bool) {
	curr := ll.head
	for curr != nil && curr.data != value {
		curr = curr.next
	}
	if curr == nil {
		return "", false
	}
	return curr.data, true
}

func (ll *LinkList) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := ll.Get()
	fmt.Fprintln(file, len(elems))
	for _, e := range elems {
		fmt.Fprintln(file, e)
	}
	return nil
}

func (ll *LinkList) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	ll.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var s string
		fmt.Fscan(file, &s)
		ll.PushBack(s)
	}
	return nil
}

func (ll *LinkList) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := ll.Get()
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

func (ll *LinkList) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	ll.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var strLen int32
		binary.Read(file, binary.LittleEndian, &strLen)
		str := make([]byte, strLen)
		binary.Read(file, binary.LittleEndian, str)
		ll.PushBack(string(str))
	}
	return nil
}