# src/runtime/slice.go

```go
func makeslice(et *_type, len, cap int) slice {
    // ...
    p := mallocgc(et.size*uintptr(cap), et, true)
	return slice{p, len, cap}
}

func growslice(et *_type, old slice, cap int) slice {
    // ...
	newcap := old.cap
	doublecap := newcap + newcap
	if cap > doublecap {
		newcap = cap
	} else {
		if old.len < 1024 {
			newcap = doublecap
		} else {
			for newcap < cap {
				newcap += newcap / 4
			}
		}
	}
    // ...
}

func slicecopy(to, fm slice, width uintptr) int {
    // ...
	size := uintptr(n) * width
	if size == 1 { // common case worth about 2x to do here
		// TODO: is this still worth it with new memmove impl?
		*(*byte)(to.array) = *(*byte)(fm.array) // known to be a byte pointer
	} else {
		memmove(to.array, fm.array, size)
	}
    // ...
}

func slicestringcopy(to []byte, fm string) int {
    // ...
	memmove(unsafe.Pointer(&to[0]), stringStructOf(&fm).str, uintptr(n))
    // ...    
}
```
