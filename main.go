/*
 * Copyright (c) 2021 Yizhou Shan syzwhat@gmail.com
 */

package main

/*
#cgo LDFLAGS: -libverbs -lrt -pthread 
#include "main.c"
 */
import "C"

import (
	"fmt"
)

func main() {
	fmt.Println("Hello from Golang!")
	C.test_print()
	C.goRDMA_detect_devices()
}
