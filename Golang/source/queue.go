package source

import (
	"fmt"
	"os"
	"errors"
	"encoding/binary"
	"bytes"
)

type QNode struct {
	data string
	next *QNode
}

type Queue struct {
	head *QNode
	tail *QNode
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Empty() bool {
	return q.head == nil
}

func (q *Queue) Push(str string) {
	newNode := &QNode{data: str, next: nil}
	if q.tail == nil {
		q.head = newNode
		q.tail = newNode
	} else {
		q.tail.next = newNode
		q.tail = newNode
	}
}

func (q *Queue) Pop() (string, error) {
	if q.Empty() {
		return "", errors.New("queue is empty!")
	}
	oldHead := q.head
	data := oldHead.data
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return data, nil
}

func (q *Queue) Get() []string {
	var result []string
	curr := q.head
	for curr != nil {
		result = append(result, curr.data)
		curr = curr.next
	}
	return result
}

func (q *Queue) Clear() {
	q.head = nil
	q.tail = nil
}

func (q *Queue) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := q.Get()
	fmt.Fprintln(file, len(elems))
	for _, e := range elems {
		fmt.Fprintln(file, e)
	}
	return nil
}

func (q *Queue) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	q.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var s string
		fmt.Fscan(file, &s)
		q.Push(s)
	}
	return nil
}

func (q *Queue) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := q.Get()
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

func (q *Queue) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	q.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var strLen int32
		binary.Read(file, binary.LittleEndian, &strLen)
		str := make([]byte, strLen)
		binary.Read(file, binary.LittleEndian, str)
		q.Push(string(str))
	}
	return nil
}