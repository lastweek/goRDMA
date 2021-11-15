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
	C.goRDMA_get_device_list()

	ib_dev := C.goRDMA_get_ibv_device(C.CString("mlx5_0"))
	fmt.Println(ib_dev)
	C.ibv_open_device(ib_dev)
}
