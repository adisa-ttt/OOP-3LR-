package source

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestStackBasicOps_A(t *testing.T) {
	s := NewStack()
	assert.True(t, s.Empty())

	s.Push("first")
	s.Push("second")
	assert.False(t, s.Empty())
	assert.Equal(t, []string{"second", "first"}, s.Get())
}

func TestQueueGet_A(t *testing.T) {
	q := NewQueue()
	q.Push("a")
	q.Push("b")
	assert.Equal(t, []string{"a", "b"}, q.Get())
}

func TestStackPop_A(t *testing.T) {
	s := NewStack()
	s.Push("a")
	s.Push("b")

	val, err := s.Pop()
	assert.NoError(t, err)
	assert.Equal(t, "b", val)

	val, err = s.Pop()
	assert.NoError(t, err)
	assert.Equal(t, "a", val)

	assert.True(t, s.Empty())
}

func TestStackPopEmpty_A(t *testing.T) {
	s := NewStack()
	_, err := s.Pop()
	assert.Error(t, err)
}

func TestStackClear_A(t *testing.T) {
	s := NewStack()
	s.Push("x")
	s.Clear()
	assert.True(t, s.Empty())
}

/*

cd ~/PROGA/3LR/Golang
go test ./source/... -coverprofile=coverage.out
go tool cover -html=coverage.out

cd ~/PROGA/3LR/Golang/source
go test . -bench=. -benchmem -benchtime=2s

*/