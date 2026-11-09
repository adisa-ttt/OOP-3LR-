package source

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestQueueBasicOps_A(t *testing.T) {
	q := NewQueue()
	assert.True(t, q.Empty())

	q.Push("first")
	q.Push("second")
	assert.False(t, q.Empty())
	assert.Equal(t, []string{"first", "second"}, q.Get())
}

func TestQueuePop_A(t *testing.T) {
	q := NewQueue()
	q.Push("a")
	q.Push("b")

	val, err := q.Pop()
	assert.NoError(t, err)
	assert.Equal(t, "a", val)

	val, err = q.Pop()
	assert.NoError(t, err)
	assert.Equal(t, "b", val)

	assert.True(t, q.Empty())
}

func TestQueuePopEmpty_A(t *testing.T) {
	q := NewQueue()
	_, err := q.Pop()
	assert.Error(t, err)
}

func TestQueueClear_A(t *testing.T) {
	q := NewQueue()
	q.Push("x")
	q.Clear()
	assert.True(t, q.Empty())
}