#include "main.h"
#include "ChainHash.h"
#include "CuckooHash.h"
#include "DoubleLinkedList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

void clearRegistry(std::map<std::string, Structure>& registry);
bool loadFromFile(const string& filename, map<string, Structure>& registry);
bool saveToFile(const string& filename, map<string, Structure>& registry);

vector<string> parseQuery(const string& query) {
    vector<string> tokens;
    istringstream iss(query);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

structType typeFromCode(char code) {
    switch (code) {
        case 'M': return structType::ARRAY;
        case 'F': return structType::LIST;
        case 'L': return structType::DOUBLE_LIST;
        case 'Q': return structType::QUEUE;
        case 'S': return structType::STACK;
        case 'T': return structType::TREE;
        case 'H': return structType::CHAINHASH;
        case 'U': return structType::CUCKOOHASH;
        default:  return structType::LIST;
    }
}

bool createStruct(const string& name, char typeCode, map<string, Structure>& registry) {
    structType type = typeFromCode(typeCode);
    if (type == structType::STACK) {
        registry[name] = {structType::STACK, new Stack()};
    } else if (type == structType::QUEUE) {
        registry[name] = {structType::QUEUE, new Queue()};
    } else if (type == structType::LIST) {
        registry[name] = {structType::LIST, new LinkList()};
    } else if (type == structType::DOUBLE_LIST) {
        registry[name] = {structType::DOUBLE_LIST, new doubleLinkList};
    } else if (type == structType::ARRAY) {
        registry[name] = {structType::ARRAY, new Array()};
    } else if (type == structType::TREE) {
        registry[name] = {structType::TREE, new avlTree()};
    } else if (type == structType::CHAINHASH) {
        registry[name] = {structType::CHAINHASH, new ChainTable(128)};
    } else if (type == structType::CUCKOOHASH) {
        registry[name] = {structType::CUCKOOHASH, new CuckooTable(128)};
    } else {
        return false;
    }
    return true;
}

// Нормализация команды: первая буква заглавная, остальные строчные
string normalizeCmd(const string& cmd) {
    if (cmd.empty()) return cmd;
    string result = "";
    result += toupper(cmd[0]);
    for (size_t i = 1; i < cmd.length(); i++) {
        result += tolower(cmd[i]);
    }
    return result;
}

bool executeCmd(const string& query, map<string, Structure>& registry) {
    vector<string> tokens = parseQuery(query);
    if (tokens.empty()) return false;
    
    string cmd = tokens[0];
    // Нормализуем регистр команды
    string normCmd = normalizeCmd(cmd);
    cmd = normCmd;
    tokens[0] = cmd;
    
    if (tokens.size() < 2) {
        cout << "error: missing structure name.\n";
        return false;
    }
    string name = tokens[1];
    
    bool exists = (registry.find(name) != registry.end());
    
    if (!exists) {
        if (cmd == "Fpush" || cmd == "Lpush" || cmd == "Qpush" ||
            cmd == "Spush" || cmd == "Mpush" || cmd == "Tinsert" ||
            cmd == "Hcpush" || cmd == "Cupush") {
            char typeCode = toupper(cmd[0]);
            if (cmd == "Hcpush") typeCode = 'H';
            if (cmd == "Cupush") typeCode = 'U';
            if (!createStruct(name, typeCode, registry)) {
                cout << "error: unknown type code " << typeCode << ".\n";
                return false;
            }
            exists = true;
        } else {
            cout << "error: structure " << name << " not found.\n";
            return false;
        }
    }
    
    Structure& str = registry[name];
    
    if (cmd == "Mpush") {
        Array* arr = static_cast<Array*>(str.data);
        if (tokens.size() == 3) {
            arr->push_back(tokens[2]);
        } else if (tokens.size() == 4) {
            int idx = stoi(tokens[2]);
            arr->insert(idx, tokens[3]);
        } else {
            cout << "error: invalid MPUSH syntax.\n";
            return false;
        }
        return true;
    }
    if (cmd == "Mget") {
        Array* arr = static_cast<Array*>(str.data);
        if (tokens.size() == 2) {
            vector<string> vals = arr->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
        } else if (tokens.size() == 3) {
            int idx = stoi(tokens[2]);
            string val;
            try {
                val = arr->getAt(idx);
                cout << val << "\n";
            } catch (...) {
                cout << "error: index out of range.\n";
            }
        }
        return true;
    }
    if (cmd == "Mdel") {
        if (tokens.size() == 3) {
            Array* arr = static_cast<Array*>(str.data);
            int idx = stoi(tokens[2]);
            if (arr->removeAt(idx)) cout << "success.\n";
            else cout << "error: index out of range.\n";
        }
        return true;
    }
    if (cmd == "Mset") {
        if (tokens.size() == 4) {
            Array* arr = static_cast<Array*>(str.data);
            int idx = stoi(tokens[2]);
            if (arr->set(idx, tokens[3])) cout << "success.\n";
            else cout << "error: index out of range.\n";
        }
        return true;
    }
    if (cmd == "Msize") {
        Array* arr = static_cast<Array*>(str.data);
        cout << arr->getSize() << "\n";
        return true;
    }
    
    if (cmd == "Spush") {
        if (tokens.size() < 3) {
            cout << "error: SPUSH requires value.\n";
            return false;
        }
        Stack* stk = static_cast<Stack*>(str.data);
        stk->push(tokens[2]);
        return true;
    }
    if (cmd == "Spop") {
        Stack* stk = static_cast<Stack*>(str.data);
        string out;
        try {
            out = stk->pop();
            cout << out << "\n";
        } catch (...) {
            cout << "error: stack is empty.\n";
        }
        return true;
    }
    if (cmd == "Sget") {
        Stack* stk = static_cast<Stack*>(str.data);
        vector<string> vals = stk->get();
        for (const string& v : vals) cout << v << " ";
        cout << "\n";
        return true;
    }
    
    if (cmd == "Qpush") {
        if (tokens.size() < 3) {
            cout << "error: QPUSH requires value.\n";
            return false;
        }
        Queue* que = static_cast<Queue*>(str.data);
        que->push(tokens[2]);
        return true;
    }
    if (cmd == "Qpop") {
        Queue* que = static_cast<Queue*>(str.data);
        string out;
        try {
            out = que->pop();
            cout << out << "\n";
        } catch (...) {
            cout << "error: queue is empty.\n";
        }
        return true;
    }
    if (cmd == "Qget") {
        Queue* que = static_cast<Queue*>(str.data);
        vector<string> vals = que->get();
        for (const string& v : vals) cout << v << " ";
        cout << "\n";
        return true;
    }
    
    if (cmd == "Fpush") {
        if (tokens.size() < 4) {
            cout << "error: FPUSH requires mode and value.\n";
            return false;
        }
        LinkList* ll = static_cast<LinkList*>(str.data);
        if (tokens.size() == 4) {
            if (tokens[2] == "front") ll->push_front(tokens[3]);
            else if (tokens[2] == "back") ll->push_back(tokens[3]);
            else { cout << "error: invalid mode\n"; return false; }
        } else if (tokens.size() == 5) {
            if (tokens[2] == "before") ll->insertBeforeValue(tokens[3], tokens[4]);
            else if (tokens[2] == "after") ll->insertAfterValue(tokens[3], tokens[4]);
            else { cout << "error: invalid mode\n"; return false; }
        }
        return true;
    }
    if (cmd == "Fpop") {
        if (tokens.size() < 3) {
            cout << "error: Fpop requires mode.\n";
            return false;
        }
        LinkList* ll = static_cast<LinkList*>(str.data);
        string tmp;
        try {
            if (tokens[2] == "front") {
                tmp = ll->pop_front();
                cout << tmp << "\n";
            } else if (tokens[2] == "back") {
                tmp = ll->pop_back();
                cout << tmp << "\n";
            } else { cout << "error: invalid mode\n"; return false; }
        } catch (...) {
            cout << "error: list is empty.\n";
        }
        return true;
    }
    if (cmd == "Fdel") {
        if (tokens.size() < 3) {
            cout << "error: FDEL requires value.\n";
            return false;
        }
        LinkList* ll = static_cast<LinkList*>(str.data);
        if (tokens.size() == 3) {
            bool ok = ll->removeValue(tokens[2]);
            cout << (ok ? "success.\n" : "ERROR: value not found.\n");
        } else if (tokens.size() == 4) {
            bool ok = false;
            if (tokens[2] == "before") ok = ll->removeBeforeValue(tokens[3]);
            else if (tokens[2] == "after") ok = ll->removeAfterValue(tokens[3]);
            else { cout << "error: invalid mode\n"; return false; }
            cout << (ok ? "success.\n" : "ERROR: operation failed.\n");
        }
        return true;
    }
    if (cmd == "Fget") {
        LinkList* ll = static_cast<LinkList*>(str.data);
        if (tokens.size() == 2) {
            vector<string> vals = ll->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
        } else if (tokens.size() == 4) {
            vector<string> vals;
            if (tokens[2] == "before") vals = ll->getBeforeValue(tokens[3]);
            else if (tokens[2] == "after") vals = ll->getAfterValue(tokens[3]);
            else { cout << "error: invalid mode\n"; return false; }
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
        }
        return true;
    }
    
    if (cmd == "Lpush") {
        if (tokens.size() < 4) {
            cout << "error: LPUSH requires mode and value.\n";
            return false;
        }
        doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
        if (tokens.size() == 4) {
            if (tokens[2] == "front") dll->push_front(tokens[3]);
            else if (tokens[2] == "back") dll->push_back(tokens[3]);
            else { cout << "error: invalid mode\n"; return false; }
        } else if (tokens.size() == 5) {
            if (tokens[2] == "before") dll->insertBeforeValue(tokens[3], tokens[4]);
            else if (tokens[2] == "after") dll->insertAfterValue(tokens[3], tokens[4]);
            else { cout << "error: invalid mode\n"; return false; }
        }
        return true;
    }
    if (cmd == "Lpop") {
        if (tokens.size() < 3) {
            cout << "error: Lpop requires mode.\n";
            return false;
        }
        doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
        string tmp;
        try {
            if (tokens[2] == "front") {
                tmp = dll->pop_front();
                cout << tmp << "\n";
            } else if (tokens[2] == "back") {
                tmp = dll->pop_back();
                cout << tmp << "\n";
            } else { cout << "error: invalid mode\n"; return false; }
        } catch (...) {
            cout << "error: list is empty.\n";
        }
        return true;
    }
    if (cmd == "Ldel") {
        if (tokens.size() < 3) {
            cout << "error: LDEL requires value.\n";
            return false;
        }
        doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
        if (tokens.size() == 3) {
            bool ok = dll->removeValue(tokens[2]);
            cout << (ok ? "success.\n" : "ERROR: value not found.\n");
        } else if (tokens.size() == 4) {
            bool ok = false;
            if (tokens[2] == "before") ok = dll->removeBeforeValue(tokens[3]);
            else if (tokens[2] == "after") ok = dll->removeAfterValue(tokens[3]);
            else { cout << "error: invalid mode\n"; return false; }
            cout << (ok ? "success.\n" : "ERROR: operation failed.\n");
        }
        return true;
    }
    if (cmd == "Lget") {
        doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
        if (tokens.size() == 2) {
            vector<string> vals = dll->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
        } else if (tokens.size() == 4) {
            vector<string> vals;
            if (tokens[2] == "before") vals = dll->getBeforeValue(tokens[3]);
            else if (tokens[2] == "after") vals = dll->getAfterValue(tokens[3]);
            else if (tokens[2] == "reverse") vals = dll->getReverse();
            else { cout << "error: invalid mode\n"; return false; }
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
        }
        return true;
    }
    
    if (cmd == "Tinsert") {
        if (tokens.size() < 3) {
            cout << "error: Tinsert requires value.\n";
            return false;
        }
        avlTree* tree = static_cast<avlTree*>(str.data);
        int key = stoi(tokens[2]);
        tree->insert(key);
        return true;
    }
    if (cmd == "Tdel") {
        if (tokens.size() < 3) {
            cout << "error: TDEL requires value.\n";
            return false;
        }
        avlTree* tree = static_cast<avlTree*>(str.data);
        int key = stoi(tokens[2]);
        if (tree->remove(key)) cout << "success.\n";
        else cout << "error: key not found.\n";
        return true;
    }
    if (cmd == "Tget") {
        avlTree* tree = static_cast<avlTree*>(str.data);
        vector<int> vals = tree->inorder();
        for (int v : vals) cout << v << " ";
        cout << "\n";
        return true;
    }
    if (cmd == "Ismember") {
        if (tokens.size() < 3) {
            cout << "error: ISMEMBER requires value.\n";
            return false;
        }
        avlTree* tree = static_cast<avlTree*>(str.data);
        int key = stoi(tokens[2]);
        bool found = tree->find(key);
        cout << (found ? "TRUE" : "FALSE") << "\n";
        return true;
    }
    
    if (cmd == "Hcpush") {
        if (tokens.size() < 4) {
            cout << "error: HCPUSH requires key and value.\n";
            return false;
        }
        ChainTable* ct = static_cast<ChainTable*>(str.data);
        int key = stoi(tokens[2]);
        if (ct->ChainInsert(key, tokens[3])) cout << "success.\n";
        else cout << "key already exists, value updated.\n";
        return true;
    }
    if (cmd == "Hcget") {
        if (tokens.size() < 3) {
            cout << "error: HCget requires key.\n";
            return false;
        }
        ChainTable* ct = static_cast<ChainTable*>(str.data);
        int key = stoi(tokens[2]);
        string val;
        bool ok = ct->getValue(key, val);
        if (ok) cout << val << "\n";
        else cout << "error: key not found.\n";
        return true;
    }
    if (cmd == "Hcdel") {
        if (tokens.size() < 3) {
            cout << "error: HCDEL requires key.\n";
            return false;
        }
        ChainTable* ct = static_cast<ChainTable*>(str.data);
        int key = stoi(tokens[2]);
        if (ct->chainRemove(key)) cout << "success.\n";
        else cout << "error: key not found.\n";
        return true;
    }
    if (cmd == "Hcfind") {
        if (tokens.size() < 3) {
            cout << "error: HCFIND requires key.\n";
            return false;
        }
        ChainTable* ct = static_cast<ChainTable*>(str.data);
        int key = stoi(tokens[2]);
        bool found = ct->chainFind(key);
        cout << (found ? "TRUE" : "FALSE") << "\n";
        return true;
    }
    
    if (cmd == "Cupush") {
        if (tokens.size() < 4) {
            cout << "error: CUPUSH requires key and value.\n";
            return false;
        }
        CuckooTable* ct = static_cast<CuckooTable*>(str.data);
        int key = stoi(tokens[2]);
        if (ct->CuckooInsert(key, tokens[3])) cout << "success.\n";
        else cout << "key already exists, value updated.\n";
        return true;
    }
    if (cmd == "Cuget") {
        if (tokens.size() < 3) {
            cout << "error: CUget requires key.\n";
            return false;
        }
        CuckooTable* ct = static_cast<CuckooTable*>(str.data);
        int key = stoi(tokens[2]);
        string val;
        bool ok = ct->getValue(key, val);
        if (ok) cout << val << "\n";
        else cout << "error: key not found.\n";
        return true;
    }
    if (cmd == "Cudel") {
        if (tokens.size() < 3) {
            cout << "error: CUDEL requires key.\n";
            return false;
        }
        CuckooTable* ct = static_cast<CuckooTable*>(str.data);
        int key = stoi(tokens[2]);
        if (ct->CuckooRemove(key)) cout << "success.\n";
        else cout << "error: key not found.\n";
        return true;
    }
    if (cmd == "Cufind") {
        if (tokens.size() < 3) {
            cout << "error: CUFIND requires key.\n";
            return false;
        }
        CuckooTable* ct = static_cast<CuckooTable*>(str.data);
        int key = stoi(tokens[2]);
        bool found = ct->CuckooFind(key);
        cout << (found ? "TRUE" : "FALSE") << "\n";
        return true;
    }
    
    if (cmd == "Print") {
        if (str.type == structType::TREE) {
            avlTree* tree = static_cast<avlTree*>(str.data);
            vector<int> vals = tree->inorder();
            for (int v : vals) cout << v << " ";
            cout << "\n";
            return true;
        }
        if (str.type == structType::ARRAY) {
            Array* arr = static_cast<Array*>(str.data);
            vector<string> vals = arr->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
            return true;
        }
        if (str.type == structType::LIST) {
            LinkList* ll = static_cast<LinkList*>(str.data);
            vector<string> vals = ll->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
            return true;
        }
        if (str.type == structType::DOUBLE_LIST) {
            doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
            vector<string> vals = dll->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
            return true;
        }
        if (str.type == structType::QUEUE) {
            Queue* que = static_cast<Queue*>(str.data);
            vector<string> vals = que->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
            return true;
        }
        if (str.type == structType::STACK) {
            Stack* stk = static_cast<Stack*>(str.data);
            vector<string> vals = stk->get();
            for (const string& v : vals) cout << v << " ";
            cout << "\n";
            return true;
        }
        cout << "error: cannot print this structure type.\n";
        return true;
    }
    
    if (cmd == "Save") {
        if (tokens.size() < 3) {
            cout << "error: SAVE requires filename.\n";
            return false;
        }
        return saveToFile(tokens[2], registry);
    }
    if (cmd == "Load") {
        if (tokens.size() < 3) {
            cout << "error: LOAD requires filename.\n";
            return false;
        }
        return loadFromFile(tokens[2], registry);
    }

    cout << "error: unknown command " << cmd << "\n";
    return false;
}

bool loadFromFile(const string& filename, map<string, Structure>& registry) {
    ifstream fs;
    fs.open(filename);
    if (!fs.is_open()) {
        cout << "error: cannot open file " << filename << "\n";
        return false;
    }
    
    string line;
    while (getline(fs, line)) {
        if (line.empty()) continue;
        
        // Проверяем, не является ли это строкой с хеш-таблицей
        if (line.find("CHAINHASH ") == 0 || line.find("CUCKOOHASH ") == 0) {
            istringstream iss(line);
            string type, name, dataFile;
            iss >> type >> name >> dataFile;
            
            if (type == "CHAINHASH") {
                registry[name] = {structType::CHAINHASH, new ChainTable(128)};
                ChainTable* ct = static_cast<ChainTable*>(registry[name].data);
                ct->loadFromFile(dataFile);  // Загружаем из отдельного файла
            } else if (type == "CUCKOOHASH") {
                registry[name] = {structType::CUCKOOHASH, new CuckooTable(128)};
                CuckooTable* ct = static_cast<CuckooTable*>(registry[name].data);
                ct->loadFromFile(dataFile);  // Загружаем из отдельного файла
            }
        } else {
            // Обычная команда
            executeCmd(line, registry);
        }
    }
    fs.close();
    return true;
}

bool saveToFile(const string& filename, map<string, Structure>& registry) {
    ofstream out;
    out.open(filename);
    if (!out.is_open()) {
        cout << "error: cannot open file " << filename << "\n";
        return false;
    }
    for (auto& pair : registry) {
        string name = pair.first;
        Structure& str = pair.second;
        
        if (str.type == structType::LIST) {
            out << "LIST " << name << "\n";
            LinkList* ll = static_cast<LinkList*>(str.data);
            vector<string> vals = ll->get();
            for (const string& v : vals)
                out << "  FPUSH " << name << " back " << v << "\n";
        }
        else if (str.type == structType::DOUBLE_LIST) {
            out << "DOUBLE_LIST " << name << "\n";
            doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
            vector<string> vals = dll->get();
            for (const string& v : vals)
                out << "  LPUSH " << name << " back " << v << "\n";
        }
        else if (str.type == structType::QUEUE) {
            out << "QUEUE " << name << "\n";
            Queue* que = static_cast<Queue*>(str.data);
            vector<string> vals = que->get();
            for (const string& v : vals)
                out << "  QPUSH " << name << " " << v << "\n";
        }
        else if (str.type == structType::STACK) {
            out << "STACK " << name << "\n";
            Stack* stk = static_cast<Stack*>(str.data);
            vector<string> vals = stk->get();
            for (int i = (int)vals.size() - 1; i >= 0; i--)
                out << "  SPUSH " << name << " " << vals[i] << "\n";
        } 
        else if (str.type == structType::ARRAY) {
            out << "ARRAY " << name << "\n";
            Array* arr = static_cast<Array*>(str.data);
            vector<string> vals = arr->get();
            for (const string& v : vals)
                out << "  MPUSH " << name << " " << v << "\n";
        } 
        else if (str.type == structType::TREE) {
            out << "TREE " << name << "\n";
            avlTree* tree = static_cast<avlTree*>(str.data);
            vector<int> vals = tree->inorder();
            for (int v : vals)
                out << "  Tinsert " << name << " " << v << "\n";
        } 
        else if (str.type == structType::CHAINHASH) {
            // Используем встроенный метод ChainTable
            out << "CHAINHASH " << name << " " << name << ".chain\n";
            ChainTable* ct = static_cast<ChainTable*>(str.data);
            ct->saveToFile(name + ".chain");  // Сохраняем в отдельный файл
        } 
        else if (str.type == structType::CUCKOOHASH) {
            // Используем встроенный метод CuckooTable
            out << "CUCKOOHASH " << name << " " << name << ".cuckoo\n";
            CuckooTable* ct = static_cast<CuckooTable*>(str.data);
            ct->saveToFile(name + ".cuckoo");  // Сохраняем в отдельный файл
        }
    }
    out.close();
    return true;
}

bool isModified(const string& query) {
    string upperQuery = query;
    for (char& c : upperQuery) c = toupper(c);
    return upperQuery.find("PUSH") != string::npos
        || upperQuery.find("DEL") != string::npos
        || upperQuery.find("POP") != string::npos
        || upperQuery.find("INSERT") != string::npos
        || upperQuery.find("SET") != string::npos;
}

void clearRegistry(map<string, Structure>& registry) {
    for (auto& pair : registry) {
        Structure& str = pair.second;
        if (str.type == structType::LIST) {
            LinkList* ll = static_cast<LinkList*>(str.data);
            delete ll;
        } else if (str.type == structType::DOUBLE_LIST) {
            doubleLinkList* dll = static_cast<doubleLinkList*>(str.data);
            delete dll;
        } else if (str.type == structType::QUEUE) {
            Queue* que = static_cast<Queue*>(str.data);
            delete que;
        } else if (str.type == structType::STACK) {
            Stack* stk = static_cast<Stack*>(str.data);
            delete stk;
        } else if (str.type == structType::TREE) {
            avlTree* tree = static_cast<avlTree*>(str.data);
            delete tree;
        } else if (str.type == structType::ARRAY) {
            Array* arr = static_cast<Array*>(str.data);
            delete arr;
        } else if (str.type == structType::CHAINHASH) {
            ChainTable* ct = static_cast<ChainTable*>(str.data);
            delete ct;
        } else if (str.type == structType::CUCKOOHASH) {
            CuckooTable* ct = static_cast<CuckooTable*>(str.data);
            delete ct;
        }
    }
    registry.clear();
}

int main(int argc, char* argv[]) {
    map<string, Structure> registry;
    string filename = "file.data";
    string query = "";
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        } else {
            cout << "error: unknown argument " << arg << "\n";
            return 1;
        }
    }
    ifstream ifs(filename);
    if (ifs) {
        string current_line;
        while (getline(ifs, current_line)) {
            if (current_line.empty()) continue;
            executeCmd(current_line, registry);
        }
        ifs.close();
    }
    if (!query.empty()) {
        bool ok = executeCmd(query, registry);
        if (ok && isModified(query)) {
            ofstream ofs(filename);
            for (auto& pair : registry) {
                string n = pair.first;
                Structure& s = pair.second;
                if (s.type == structType::LIST) {
                    for (const string& v : static_cast<LinkList*>(s.data)->get())
                        ofs << "FPUSH " << n << " back " << v << "\n";
                } else if (s.type == structType::DOUBLE_LIST) {
                    for (const string& v : static_cast<doubleLinkList*>(s.data)->get())
                        ofs << "LPUSH " << n << " back " << v << "\n";
                } else if (s.type == structType::QUEUE) {
                    for (const string& v : static_cast<Queue*>(s.data)->get())
                        ofs << "QPUSH " << n << " " << v << "\n";
                } else if (s.type == structType::STACK) {
                    vector<string> vals = static_cast<Stack*>(s.data)->get();
                    for (int i = (int)vals.size() - 1; i >= 0; i--)
                        ofs << "SPUSH " << n << " " << vals[i] << "\n";
                } else if (s.type == structType::ARRAY) {
                    for (const string& v : static_cast<Array*>(s.data)->get())
                        ofs << "MPUSH " << n << " " << v << "\n";
                } else if (s.type == structType::TREE) {
                    for (int v : static_cast<avlTree*>(s.data)->preorder())
                        ofs << "Tinsert " << n << " " << v << "\n";
                } else if (s.type == structType::CHAINHASH) {
                    for (auto& kv : static_cast<ChainTable*>(s.data)->items())
                        ofs << "HCPUSH " << n << " " << kv.first << " " << kv.second << "\n";
                } else if (s.type == structType::CUCKOOHASH) {
                    for (auto& kv : static_cast<CuckooTable*>(s.data)->items())
                        ofs << "CUPUSH " << n << " " << kv.first << " " << kv.second << "\n";
}
            }
        }
    } else {
        string line;
        cout << "> ";
        while (getline(cin, line)) {
            if (line.empty()) { cout << "> "; continue; }
            bool ok = executeCmd(line, registry);
            if (ok && isModified(line)) {
                ofstream ofs(filename);
                for (auto& pair : registry) {
                    string n = pair.first;
                    Structure& s = pair.second;
                    if (s.type == structType::LIST) {
                        for (const string& v : static_cast<LinkList*>(s.data)->get())
                            ofs << "FPUSH " << n << " back " << v << "\n";
                    } else if (s.type == structType::DOUBLE_LIST) {
                        for (const string& v : static_cast<doubleLinkList*>(s.data)->get())
                            ofs << "LPUSH " << n << " back " << v << "\n";
                    } else if (s.type == structType::QUEUE) {
                        for (const string& v : static_cast<Queue*>(s.data)->get())
                            ofs << "QPUSH " << n << " " << v << "\n";
                    } else if (s.type == structType::STACK) {
                        vector<string> vals = static_cast<Stack*>(s.data)->get();
                        for (int i = (int)vals.size() - 1; i >= 0; i--)
                            ofs << "SPUSH " << n << " " << vals[i] << "\n";
                    } else if (s.type == structType::ARRAY) {
                        for (const string& v : static_cast<Array*>(s.data)->get())
                            ofs << "MPUSH " << n << " " << v << "\n";
                    } else if (s.type == structType::TREE) {
                        for (int v : static_cast<avlTree*>(s.data)->preorder())
                            ofs << "Tinsert " << n << " " << v << "\n";
                    } else if (s.type == structType::CHAINHASH) {
                        ofs << "# ChainHash " << n << " - serialization not fully implemented\n";
                    } else if (s.type == structType::CUCKOOHASH) {
                        ofs << "# CuckooHash " << n << " - serialization not fully implemented\n";
                    }
                }
            }
            cout << "> ";
        }
    }
    clearRegistry(registry);
    return 0;
}

/*
mkdir -p build
cd build
cmake ..
make -j$(nproc)

#!/bin/bash

rm -f file.data

echo "          array"
./structorium --file file.data --query 'MPUSH myarr apple'
./structorium --file file.data --query 'MPUSH myarr banana'
./structorium --file file.data --query 'MPUSH myarr 0 cherry'
./structorium --file file.data --query 'MGET myarr'
./structorium --file file.data --query 'MSIZE myarr'
./structorium --file file.data --query 'MSET myarr 0 date'
./structorium --file file.data --query 'MDEL myarr 1'
./structorium --file file.data --query 'MGET myarr'

echo "          stack"
./structorium --file file.data --query 'SPUSH mystack 100'
./structorium --file file.data --query 'SPUSH mystack 200'
./structorium --file file.data --query 'SPUSH mystack 300'
./structorium --file file.data --query 'SGET mystack'
./structorium --file file.data --query 'SPOP mystack'
./structorium --file file.data --query 'SGET mystack'

echo "          queue"
./structorium --file file.data --query 'QPUSH myqueue first'
./structorium --file file.data --query 'QPUSH myqueue second'
./structorium --file file.data --query 'QPUSH myqueue third'
./structorium --file file.data --query 'QGET myqueue'
./structorium --file file.data --query 'QPOP myqueue'
./structorium --file file.data --query 'QGET myqueue'

echo "          list"
./structorium --file file.data --query 'FPUSH mylist back A'
./structorium --file file.data --query 'FPUSH mylist back B'
./structorium --file file.data --query 'FPUSH mylist front C'
./structorium --file file.data --query 'FGET mylist'
./structorium --file file.data --query 'FPUSH mylist before B X'
./structorium --file file.data --query 'FPUSH mylist after A Y'
./structorium --file file.data --query 'FGET mylist'
./structorium --file file.data --query 'FDEL mylist X'
./structorium --file file.data --query 'FGET mylist'

echo "          double_list"
./structorium --file file.data --query 'LPUSH mydll back 1'
./structorium --file file.data --query 'LPUSH mydll back 2'
./structorium --file file.data --query 'LPUSH mydll front 0'
./structorium --file file.data --query 'LGET mydll'
./structorium --file file.data --query 'LPUSH mydll before 2 99'
./structorium --file file.data --query 'LGET mydll'
./structorium --file file.data --query 'LPOP mydll back'
./structorium --file file.data --query 'LGET mydll'

echo "          tree"
./structorium --file file.data --query 'TINSERT mytree 10'
./structorium --file file.data --query 'TINSERT mytree 5'
./structorium --file file.data --query 'TINSERT mytree 20'
./structorium --file file.data --query 'TINSERT mytree 3'
./structorium --file file.data --query 'TINSERT mytree 7'
./structorium --file file.data --query 'TGET mytree'
./structorium --file file.data --query 'ISMEMBER mytree 7'
./structorium --file file.data --query 'ISMEMBER mytree 99'
./structorium --file file.data --query 'TDEL mytree 10'
./structorium --file file.data --query 'TGET mytree'

./structorium --file file.data --query 'HCPUSH chain 10 apple'
./structorium --file file.data --query 'HCPUSH chain 20 banana'
./structorium --file file.data --query 'HCPUSH chain 10 cherry'    # Обновление существующего ключа
./structorium --file file.data --query 'HCget chain 10'            # Ожидается: cherry
./structorium --file file.data --query 'HCget chain 20'            # Ожидается: banana
./structorium --file file.data --query 'HCFIND chain 20'           # Ожидается: TRUE
./structorium --file file.data --query 'HCFIND chain 99'           # Ожидается: FALSE
./structorium --file file.data --query 'HCDEL chain 10'            # Ожидается: success.
./structorium --file file.data --query 'HCget chain 10'            # Ожидается: error: key not found.


./structorium --file file.data --query 'CUPUSH cuckoo 100 red'
./structorium --file file.data --query 'CUPUSH cuckoo 200 green'
./structorium --file file.data --query 'CUPUSH cuckoo 100 blue'    # Обновление существующего ключа
./structorium --file file.data --query 'CUget cuckoo 100'          # Ожидается: blue
./structorium --file file.data --query 'CUget cuckoo 200'          # Ожидается: green
./structorium --file file.data --query 'CUFIND cuckoo 200'         # Ожидается: TRUE
./structorium --file file.data --query 'CUFIND cuckoo 999'         # Ожидается: FALSE
./structorium --file file.data --query 'CUDEL cuckoo 100'          # Ожидается: success.
./structorium --file file.data --query 'CUget cuckoo 100'          # Ожидается: error: key not found.


echo "file"
cat file.data


*/