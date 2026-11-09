package source

import (
	"errors"
	"fmt"
	"os"
	"encoding/binary"
	"bytes"
)
const StartCapacity = 4

type Array struct {
	data     []string
	size     int
	capacity int
}

func NewArray() *Array {
	return &Array{
		data:     make([]string, 0, StartCapacity),
		size:     0,
		capacity: StartCapacity,
	}
}

func (a *Array) resize() {
	newCap := StartCapacity
	if a.capacity > 0 {
		newCap = a.capacity * 2
	}
	newData := make([]string, a.size, newCap)
	copy(newData, a.data)
	a.data = newData
	a.capacity = newCap
}

func (a *Array) Empty() bool {
	return a.size == 0
}

func (a *Array) Clear() {
	a.data = nil
	a.size = 0
	a.capacity = 0
}

func (a *Array) PushBack(str string) {
	if a.size == a.capacity {
		a.resize()
	}
	if len(a.data) < a.size+1 {
		a.data = append(a.data, str)
	} else {
		a.data[a.size] = str
	}
	a.size++
}

func (a *Array) Insert(index int, str string) bool {
	if index < 0 || index >= a.size {
		a.PushBack(str)
		return true
	}
	if a.size == a.capacity {
		a.resize()
	}
	a.data = append(a.data, "")
	copy(a.data[index+1:], a.data[index:])
	a.data[index] = str
	a.size++
	return true
}

func (a *Array) RemoveAt(index int) bool {
	if index < 0 || index >= a.size {
		return false
	}
	a.data = append(a.data[:index], a.data[index+1:]...)
	a.size--
	return true
}

func (a *Array) GetSize() int {
	return a.size
}

func (a *Array) Set(index int, str string) bool {
	if index < 0 || index >= a.size {
		return false
	}
	a.data[index] = str
	return true
}

func (a *Array) GetAt(index int) (string, error) {
	if index < 0 || index >= a.size {
		return "", errors.New("index out of range!")
	}
	return a.data[index], nil
}

func (a *Array) Get() []string {
	result := make([]string, a.size)
	copy(result, a.data[:a.size])
	return result
}

func (a *Array) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, a.size)
	for i := 0; i < a.size; i++ {
		fmt.Fprintln(file, a.data[i])
	}
	return nil
}

func (a *Array) LoadFromText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	a.Clear()
	var n int
	fmt.Fscan(file, &n)
	for i := 0; i < n; i++ {
		var s string
		fmt.Fscan(file, &s)
		a.PushBack(s)
	}
	return nil
}

func (a *Array) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, int32(a.size))
	for i := 0; i < a.size; i++ {
		str := []byte(a.data[i])
		binary.Write(&buf, binary.LittleEndian, int32(len(str)))
		buf.Write(str)
	}
	_, err = file.Write(buf.Bytes())
	return err
}

func (a *Array) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	a.Clear()
	var n int32
	binary.Read(file, binary.LittleEndian, &n)
	for i := 0; i < int(n); i++ {
		var strLen int32
		binary.Read(file, binary.LittleEndian, &strLen)
		str := make([]byte, strLen)
		binary.Read(file, binary.LittleEndian, str)
		a.PushBack(string(str))
	}
	return nil
}