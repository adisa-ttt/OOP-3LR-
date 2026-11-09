package hash

func Fmix(h uint32) uint32 {
	h ^= h >> 16
	h *= 0x85ebca6b
	h ^= h >> 13
	h *= 0xc2b2ae35
	h ^= h >> 16
	return h
}

func Murmur3_32(key uint32, seed uint32) uint32 {
	const c1 uint32 = 0xcc9e2d51
	const c2 uint32 = 0x1b873593

	k := key
	h := seed
	k *= c1

	k = (k << 15) | (k >> 17)
	h ^= k
	h = (h << 13) | (h >> 19)
	h = h*5 + 0xe6546b64

	h ^= 4
	h = Fmix(h)

	return h
}
