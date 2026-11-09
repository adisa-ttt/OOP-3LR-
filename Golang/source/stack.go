package source

import (
	"fmt"
	"os"
	"encoding/binary"
	"bytes"
	"errors"
) 

type SNode struct {
	data string
	next *SNode
}

type Stack struct {
	head *SNode
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Empty() bool {
	return s.head == nil
}

func (s *Stack) Push(str string) {
	s.head = &SNode{data: str, next: s.head}
}

func (s *Stack) Pop() (string, error) {
	if s.Empty() {
		return "", errors.New("stack is empty!")
	}
	oldHead := s.head
	data := oldHead.data
	s.head = oldHead.next
	return data, nil
}

func (s *Stack) Get() []string {
	var result []string
	curr := s.head
	for curr != nil {
		result = append(result, curr.data)
		curr = curr.next
	}
	return result
}

func (s *Stack) Clear() {
	s.head = nil
}

func (s *Stack) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := s.Get()
	fmt.Fprintln(file, len(elems))
	for i := len(elems) - 1; i >= 0; i-- {
		fmt.Fprintln(file, elems[i])
	}
	return nil
}

func (s *Stack) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	s.Clear()
	var n int
	fmt.Fscan(file, &n)
	var elems []string
	for i := 0; i < n; i++ {
		var str string
		fmt.Fscan(file, &str)
		elems = append(elems, str)
	}
	for i := len(elems) - 1; i >= 0; i-- {
		s.Push(elems[i])
	}
	return nil
}

func (s *Stack) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	elems := s.Get()
	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, int32(len(elems)))
	for i := len(elems) - 1; i >= 0; i-- {
		str := []byte(elems[i])
		binary.Write(&buf, binary.LittleEndian, int32(len(str)))
		buf.Write(str)
	}
	_, err = file.Write(buf.Bytes())
	return err
}

func (s *Stack) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	s.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	var elems []string
	for i := 0; i < int(n); i++ {
		var strLen int32
		binary.Read(file, binary.LittleEndian, &strLen)
		str := make([]byte, strLen)
		binary.Read(file, binary.LittleEndian, str)
		elems = append(elems, string(str))
	}
	for i := len(elems) - 1; i >= 0; i-- {
		s.Push(elems[i])
	}
	return nil
}