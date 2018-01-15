package main

import (
	"fmt"
	"myDir/pkgDir"
)

func main() {
	fmt.Print("main package\n")
	pkgDir.TestPrint()
}
