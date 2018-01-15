package main

import "fmt"

type Node struct {
	le   *Node
	data interface{}
	ri   *Node
}

func NewNode(left, right *Node) *Node {
	return &Node{left, nil, right}
}

func (n *Node) SetData(data interface{}) {
	n.data = data
}

func (n *Node) String() string {
	return fmt.Sprintf("L = '%v' ", n.le.data) + fmt.Sprintf("C = '%v' ", n.data) + fmt.Sprintf("R = '%v'", n.ri.data)
}

func main() {
	root := NewNode(nil, nil)
	root.SetData("root node")
	// make child (left) nodes:
	a := NewNode(nil, nil)
	a.SetData("left node")
	b := NewNode(nil, nil)
	//b.SetData("right node")
	b.SetData(true)
	root.le = a
	root.ri = b
	fmt.Printf("%v\n", root)
}
