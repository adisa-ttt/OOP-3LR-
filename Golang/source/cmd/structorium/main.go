package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"

	"db/source"
)

type structureType int

const (
	arrayType structureType = iota
	listType
	doubleListType
	queueType
	stackType
	treeType
	chainHashType
	cuckooHashType
)

type Structure struct {
	typ  structureType
	data interface{}
}

func newStructure(t structureType) *Structure {
	var data interface{}
	switch t {
	case arrayType:
		data = source.NewArray()
	case listType:
		data = source.NewLinkList()
	case doubleListType:
		data = source.NewDoubleLinkList()
	case queueType:
		data = source.NewQueue()
	case stackType:
		data = source.NewStack()
	case treeType:
		data = source.NewAVLTree()
	case chainHashType:
		data = source.NewChainTable(128)
	case cuckooHashType:
		data = source.NewCuckooTable(128)
	}
	return &Structure{typ: t, data: data}
}

func normalizeCmd(cmd string) string {
	if cmd == "" {
		return cmd
	}
	return strings.ToUpper(cmd[:1]) + strings.ToLower(cmd[1:])
}

func isModified(query string) bool {
	q := strings.ToUpper(query)
	return strings.Contains(q, "PUSH") || strings.Contains(q, "DEL") ||
		strings.Contains(q, "POP") || strings.Contains(q, "INSERT") ||
		strings.Contains(q, "SET")
}

func executeCmd(query string, registry map[string]*Structure) bool {
	tokens := strings.Fields(query)
	if len(tokens) == 0 {
		return false
	}
	cmd := normalizeCmd(tokens[0])
	tokens[0] = cmd

	if cmd == "Save" {
		if len(tokens) < 2 {
			fmt.Println("error: SAVE requires filename.")
			return false
		}
		if err := saveRegistryToFile(tokens[1], registry); err != nil {
			fmt.Printf("error: %v\n", err)
			return false
		}
		return true
	}
	if cmd == "Load" {
		if len(tokens) < 2 {
			fmt.Println("error: LOAD requires filename.")
			return false
		}
		if err := loadRegistryFromFile(tokens[1], registry); err != nil {
			fmt.Printf("error: %v\n", err)
			return false
		}
		return true
	}

	if len(tokens) < 2 {
		fmt.Println("error: missing structure name.")
		return false
	}
	name := tokens[1]

	str, exists := registry[name]
	if !exists {
		var t structureType
		var canCreate bool
		switch cmd {
		case "Fpush":
			t, canCreate = listType, true
		case "Lpush":
			t, canCreate = doubleListType, true
		case "Qpush":
			t, canCreate = queueType, true
		case "Spush":
			t, canCreate = stackType, true
		case "Mpush":
			t, canCreate = arrayType, true
		case "Tinsert":
			t, canCreate = treeType, true
		case "Hcpush":
			t, canCreate = chainHashType, true
		case "Cupush":
			t, canCreate = cuckooHashType, true
		}
		if !canCreate {
			fmt.Printf("error: structure %s not found.\n", name)
			return false
		}
		str = newStructure(t)
		registry[name] = str
	}

	// ---------- ARRAY ----------
	switch cmd {
	case "Mpush":
		arr := str.data.(*source.Array)
		switch len(tokens) {
		case 3:
			arr.PushBack(tokens[2])
		case 4:
			idx, err := strconv.Atoi(tokens[2])
			if err != nil {
				fmt.Println("error: invalid index.")
				return false
			}
			arr.Insert(idx, tokens[3])
		default:
			fmt.Println("error: invalid MPUSH syntax.")
			return false
		}
		return true

	case "Mget":
		arr := str.data.(*source.Array)
		if len(tokens) == 2 {
			for _, v := range arr.Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
		} else if len(tokens) == 3 {
			idx, err := strconv.Atoi(tokens[2])
			if err != nil {
				fmt.Println("error: invalid index.")
				return false
			}
			val, err := arr.GetAt(idx)
			if err != nil {
				fmt.Println("error: index out of range.")
			} else {
				fmt.Println(val)
			}
		}
		return true

	case "Mdel":
		if len(tokens) == 3 {
			arr := str.data.(*source.Array)
			idx, err := strconv.Atoi(tokens[2])
			if err != nil {
				fmt.Println("error: invalid index.")
				return false
			}
			if arr.RemoveAt(idx) {
				fmt.Println("success.")
			} else {
				fmt.Println("error: index out of range.")
			}
		}
		return true

	case "Mset":
		if len(tokens) == 4 {
			arr := str.data.(*source.Array)
			idx, err := strconv.Atoi(tokens[2])
			if err != nil {
				fmt.Println("error: invalid index.")
				return false
			}
			if arr.Set(idx, tokens[3]) {
				fmt.Println("success.")
			} else {
				fmt.Println("error: index out of range.")
			}
		}
		return true

	case "Msize":
		arr := str.data.(*source.Array)
		fmt.Println(arr.GetSize())
		return true
	}

	// ---------- STACK ----------
	switch cmd {
	case "Spush":
		if len(tokens) < 3 {
			fmt.Println("error: SPUSH requires value.")
			return false
		}
		stk := str.data.(*source.Stack)
		stk.Push(tokens[2])
		return true

	case "Spop":
		stk := str.data.(*source.Stack)
		out, err := stk.Pop()
		if err != nil {
			fmt.Println("error: stack is empty.")
		} else {
			fmt.Println(out)
		}
		return true

	case "Sget":
		stk := str.data.(*source.Stack)
		for _, v := range stk.Get() {
			fmt.Print(v, " ")
		}
		fmt.Println()
		return true
	}

	// ---------- QUEUE ----------
	switch cmd {
	case "Qpush":
		if len(tokens) < 3 {
			fmt.Println("error: QPUSH requires value.")
			return false
		}
		q := str.data.(*source.Queue)
		q.Push(tokens[2])
		return true

	case "Qpop":
		q := str.data.(*source.Queue)
		out, err := q.Pop()
		if err != nil {
			fmt.Println("error: queue is empty.")
		} else {
			fmt.Println(out)
		}
		return true

	case "Qget":
		q := str.data.(*source.Queue)
		for _, v := range q.Get() {
			fmt.Print(v, " ")
		}
		fmt.Println()
		return true
	}

	// ---------- SINGLE LINK LIST ----------
	switch cmd {
	case "Fpush":
		if len(tokens) < 4 {
			fmt.Println("error: FPUSH requires mode and value.")
			return false
		}
		ll := str.data.(*source.LinkList)
		if len(tokens) == 4 {
			switch tokens[2] {
			case "front":
				ll.PushFront(tokens[3])
			case "back":
				ll.PushBack(tokens[3])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
		} else if len(tokens) == 5 {
			switch tokens[2] {
			case "before":
				ll.InsertBeforeValue(tokens[3], tokens[4])
			case "after":
				ll.InsertAfterValue(tokens[3], tokens[4])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
		}
		return true

	case "Fpop":
		if len(tokens) < 3 {
			fmt.Println("error: Fpop requires mode.")
			return false
		}
		ll := str.data.(*source.LinkList)
		switch tokens[2] {
		case "front":
			tmp, err := ll.PopFront()
			if err != nil {
				fmt.Println("error: list is empty.")
			} else {
				fmt.Println(tmp)
			}
		case "back":
			tmp, err := ll.PopBack()
			if err != nil {
				fmt.Println("error: list is empty.")
			} else {
				fmt.Println(tmp)
			}
		default:
			fmt.Println("error: invalid mode")
			return false
		}
		return true

	case "Fdel":
		if len(tokens) < 3 {
			fmt.Println("error: FDEL requires value.")
			return false
		}
		ll := str.data.(*source.LinkList)
		if len(tokens) == 3 {
			ok := ll.RemoveValue(tokens[2])
			if ok {
				fmt.Println("success.")
			} else {
				fmt.Println("ERROR: value not found.")
			}
		} else if len(tokens) == 4 {
			var ok bool
			switch tokens[2] {
			case "before":
				ok = ll.RemoveBeforeValue(tokens[3])
			case "after":
				ok = ll.RemoveAfterValue(tokens[3])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
			if ok {
				fmt.Println("success.")
			} else {
				fmt.Println("ERROR: operation failed.")
			}
		}
		return true

	case "Fget":
		ll := str.data.(*source.LinkList)
		if len(tokens) == 2 {
			for _, v := range ll.Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
		} else if len(tokens) == 4 {
			var vals []string
			switch tokens[2] {
			case "before":
				vals = ll.GetBeforeValue(tokens[3])
			case "after":
				vals = ll.GetAfterValue(tokens[3])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
			for _, v := range vals {
				fmt.Print(v, " ")
			}
			fmt.Println()
		}
		return true
	}

	// ---------- DOUBLE LINK LIST ----------
	switch cmd {
	case "Lpush":
		if len(tokens) < 4 {
			fmt.Println("error: LPUSH requires mode and value.")
			return false
		}
		dll := str.data.(*source.DoubleLinkList)
		if len(tokens) == 4 {
			switch tokens[2] {
			case "front":
				dll.PushFront(tokens[3])
			case "back":
				dll.PushBack(tokens[3])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
		} else if len(tokens) == 5 {
			switch tokens[2] {
			case "before":
				dll.InsertBeforeValue(tokens[3], tokens[4])
			case "after":
				dll.InsertAfterValue(tokens[3], tokens[4])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
		}
		return true

	case "Lpop":
		if len(tokens) < 3 {
			fmt.Println("error: Lpop requires mode.")
			return false
		}
		dll := str.data.(*source.DoubleLinkList)
		switch tokens[2] {
		case "front":
			tmp, err := dll.PopFront()
			if err != nil {
				fmt.Println("error: list is empty.")
			} else {
				fmt.Println(tmp)
			}
		case "back":
			tmp, err := dll.PopBack()
			if err != nil {
				fmt.Println("error: list is empty.")
			} else {
				fmt.Println(tmp)
			}
		default:
			fmt.Println("error: invalid mode")
			return false
		}
		return true

	case "Ldel":
		if len(tokens) < 3 {
			fmt.Println("error: LDEL requires value.")
			return false
		}
		dll := str.data.(*source.DoubleLinkList)
		if len(tokens) == 3 {
			ok := dll.RemoveValue(tokens[2])
			if ok {
				fmt.Println("success.")
			} else {
				fmt.Println("ERROR: value not found.")
			}
		} else if len(tokens) == 4 {
			var ok bool
			switch tokens[2] {
			case "before":
				ok = dll.RemoveBeforeValue(tokens[3])
			case "after":
				ok = dll.RemoveAfterValue(tokens[3])
			default:
				fmt.Println("error: invalid mode")
				return false
			}
			if ok {
				fmt.Println("success.")
			} else {
				fmt.Println("ERROR: operation failed.")
			}
		}
		return true

	case "Lget":
		dll := str.data.(*source.DoubleLinkList)
		if len(tokens) == 2 {
			for _, v := range dll.Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
		} else if len(tokens) == 4 {
			var vals []string
			switch tokens[2] {
			case "before":
				vals = dll.GetBeforeValue(tokens[3])
			case "after":
				vals = dll.GetAfterValue(tokens[3])
			case "reverse":
				vals = dll.GetReverse()
			default:
				fmt.Println("error: invalid mode")
				return false
			}
			for _, v := range vals {
				fmt.Print(v, " ")
			}
			fmt.Println()
		}
		return true
	}

	// ---------- AVL TREE ----------
	switch cmd {
	case "Tinsert":
		if len(tokens) < 3 {
			fmt.Println("error: Tinsert requires value.")
			return false
		}
		tree := str.data.(*source.AVLTree)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid value.")
			return false
		}
		tree.Insert(key)
		return true

	case "Tdel":
		if len(tokens) < 3 {
			fmt.Println("error: TDEL requires value.")
			return false
		}
		tree := str.data.(*source.AVLTree)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid value.")
			return false
		}
		if tree.Remove(key) {
			fmt.Println("success.")
		} else {
			fmt.Println("error: key not found.")
		}
		return true

	case "Tget":
		tree := str.data.(*source.AVLTree)
		for _, v := range tree.Inorder() {
			fmt.Print(v, " ")
		}
		fmt.Println()
		return true

	case "Ismember":
		if len(tokens) < 3 {
			fmt.Println("error: ISMEMBER requires value.")
			return false
		}
		tree := str.data.(*source.AVLTree)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid value.")
			return false
		}
		if tree.Find(key) {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
		return true
	}

	// ---------- CHAIN HASH ----------
	switch cmd {
	case "Hcpush":
		if len(tokens) < 4 {
			fmt.Println("error: HCPUSH requires key and value.")
			return false
		}
		ct := str.data.(*source.ChainTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		if ct.ChainInsert(key, tokens[3]) {
			fmt.Println("success.")
		} else {
			fmt.Println("key already exists, value updated.")
		}
		return true

	case "Hcget":
		if len(tokens) < 3 {
			fmt.Println("error: HCget requires key.")
			return false
		}
		ct := str.data.(*source.ChainTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		val, ok := ct.GetValue(key)
		if ok {
			fmt.Println(val)
		} else {
			fmt.Println("error: key not found.")
		}
		return true

	case "Hcdel":
		if len(tokens) < 3 {
			fmt.Println("error: HCDEL requires key.")
			return false
		}
		ct := str.data.(*source.ChainTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		if ct.ChainRemove(key) {
			fmt.Println("success.")
		} else {
			fmt.Println("error: key not found.")
		}
		return true

	case "Hcfind":
		if len(tokens) < 3 {
			fmt.Println("error: HCFIND requires key.")
			return false
		}
		ct := str.data.(*source.ChainTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		if ct.ChainFind(key) {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
		return true
	}

	// ---------- CUCKOO HASH ----------
	switch cmd {
	case "Cupush":
		if len(tokens) < 4 {
			fmt.Println("error: CUPUSH requires key and value.")
			return false
		}
		ct := str.data.(*source.CuckooTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		if ct.CuckooInsert(key, tokens[3]) {
			fmt.Println("success.")
		} else {
			fmt.Println("key already exists, value updated.")
		}
		return true

	case "Cuget":
		if len(tokens) < 3 {
			fmt.Println("error: CUget requires key.")
			return false
		}
		ct := str.data.(*source.CuckooTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		val, ok := ct.GetValue(key)
		if ok {
			fmt.Println(val)
		} else {
			fmt.Println("error: key not found.")
		}
		return true

	case "Cudel":
		if len(tokens) < 3 {
			fmt.Println("error: CUDEL requires key.")
			return false
		}
		ct := str.data.(*source.CuckooTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		if ct.CuckooRemove(key) {
			fmt.Println("success.")
		} else {
			fmt.Println("error: key not found.")
		}
		return true

	case "Cufind":
		if len(tokens) < 3 {
			fmt.Println("error: CUFIND requires key.")
			return false
		}
		ct := str.data.(*source.CuckooTable)
		key, err := strconv.Atoi(tokens[2])
		if err != nil {
			fmt.Println("error: invalid key.")
			return false
		}
		if ct.CuckooFind(key) {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
		return true
	}

	// ---------- PRINT (универсальная) ----------
	if cmd == "Print" {
		switch str.typ {
		case treeType:
			for _, v := range str.data.(*source.AVLTree).Inorder() {
				fmt.Print(v, " ")
			}
			fmt.Println()
			return true
		case arrayType:
			for _, v := range str.data.(*source.Array).Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
			return true
		case listType:
			for _, v := range str.data.(*source.LinkList).Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
			return true
		case doubleListType:
			for _, v := range str.data.(*source.DoubleLinkList).Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
			return true
		case queueType:
			for _, v := range str.data.(*source.Queue).Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
			return true
		case stackType:
			for _, v := range str.data.(*source.Stack).Get() {
				fmt.Print(v, " ")
			}
			fmt.Println()
			return true
		}
		fmt.Println("error: cannot print this structure type.")
		return true
	}

	fmt.Printf("error: unknown command %s\n", cmd)
	return false
}

// ---------- Сериализация ----------

func saveRegistryToFile(filename string, registry map[string]*Structure) error {
	f, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	w := bufio.NewWriter(f)

	for name, s := range registry {
		switch s.typ {
		case listType:
			for _, v := range s.data.(*source.LinkList).Get() {
				fmt.Fprintf(w, "FPUSH %s back %s\n", name, v)
			}
		case doubleListType:
			for _, v := range s.data.(*source.DoubleLinkList).Get() {
				fmt.Fprintf(w, "LPUSH %s back %s\n", name, v)
			}
		case queueType:
			for _, v := range s.data.(*source.Queue).Get() {
				fmt.Fprintf(w, "QPUSH %s %s\n", name, v)
			}
		case stackType:
			vals := s.data.(*source.Stack).Get()
			for i := len(vals) - 1; i >= 0; i-- {
				fmt.Fprintf(w, "SPUSH %s %s\n", name, vals[i])
			}
		case arrayType:
			for _, v := range s.data.(*source.Array).Get() {
				fmt.Fprintf(w, "MPUSH %s %s\n", name, v)
			}
		case treeType:
			for _, v := range s.data.(*source.AVLTree).Preorder() {
				fmt.Fprintf(w, "TINSERT %s %d\n", name, v)
			}
		case chainHashType:
			hashFile := name + ".chain"
			fmt.Fprintf(w, "CHAINHASH %s %s\n", name, hashFile)
			if err := s.data.(*source.ChainTable).SaveToText(hashFile); err != nil {
				return err
			}
		case cuckooHashType:
			hashFile := name + ".cuckoo"
			fmt.Fprintf(w, "CUCKOOHASH %s %s\n", name, hashFile)
			if err := s.data.(*source.CuckooTable).SaveToText(hashFile); err != nil {
				return err
			}
		}
	}
	return w.Flush()
}

func loadChainHashFromFile(ct *source.ChainTable, filename string) error {
	// ChainTable.LoadFromText корректно работает: Clear() сохраняет size
	return ct.LoadFromText(filename)
}

func loadCuckooHashFromFile(ct *source.CuckooTable, filename string) error {
	// ВНИМАНИЕ: CuckooTable.LoadFromText вызывает Clear(),
	// который обнуляет size → будет panic (деление на 0).
	// Поэтому читаем вручную без Clear().
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	var n int
	if _, err := fmt.Fscan(f, &n); err != nil {
		return err
	}
	for i := 0; i < n; i++ {
		var key int
		var value string
		if _, err := fmt.Fscan(f, &key, &value); err != nil {
			return err
		}
		ct.CuckooInsert(key, value)
	}
	return nil
}

func loadRegistryFromFile(filename string, registry map[string]*Structure) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		parts := strings.Fields(line)
		if len(parts) >= 3 && (parts[0] == "CHAINHASH" || parts[0] == "CUCKOOHASH") {
			typ := parts[0]
			name := parts[1]
			dataFile := parts[2]
			if typ == "CHAINHASH" {
				ct := source.NewChainTable(128)
				_ = loadChainHashFromFile(ct, dataFile)
				registry[name] = &Structure{typ: chainHashType, data: ct}
			} else {
				ct := source.NewCuckooTable(128)
				_ = loadCuckooHashFromFile(ct, dataFile)
				registry[name] = &Structure{typ: cuckooHashType, data: ct}
			}
		} else {
			executeCmd(line, registry)
		}
	}
	return scanner.Err()
}

// ---------- main ----------

func main() {
	registry := make(map[string]*Structure)
	filename := "file.data"
	query := ""

	args := os.Args[1:]
	for i := 0; i < len(args); i++ {
		switch args[i] {
		case "--file":
			if i+1 < len(args) {
				filename = args[i+1]
				i++
			}
		case "--query":
			if i+1 < len(args) {
				query = args[i+1]
				i++
			}
		default:
			fmt.Printf("error: unknown argument %s\n", args[i])
			os.Exit(1)
		}
	}

	if _, err := os.Stat(filename); err == nil {
		loadRegistryFromFile(filename, registry)
	}

	if query != "" {
		ok := executeCmd(query, registry)
		if ok && isModified(query) {
			_ = saveRegistryToFile(filename, registry)
		}
		return
	}

	// Интерактивный режим
	reader := bufio.NewReader(os.Stdin)
	fmt.Print("> ")
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			break
		}
		line = strings.TrimRight(line, "\r\n")
		if line == "" {
			fmt.Print("> ")
			continue
		}
		ok := executeCmd(line, registry)
		if ok && isModified(line) {
			_ = saveRegistryToFile(filename, registry)
		}
		fmt.Print("> ")
	}
}