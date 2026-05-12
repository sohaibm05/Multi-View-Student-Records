#include "StudentRecords.hpp"
#include <cctype>

bool StudentRecords::startsWith(const std::string& text, const std::string& prefix) 
{
    if (prefix.size() > text.size())
    { 
        return false;
    }

    for (size_t i = 0; i < prefix.size(); i++) 
    {
        char a = static_cast<char>(std::tolower(static_cast<unsigned char>(text[i])));
        char b = static_cast<char>(std::tolower(static_cast<unsigned char>(prefix[i])));
        if (a != b)
        { 
            return false;
        }
    }
    return true;
}

bool StudentRecords::addStudent(int id, const std::string& name, double cgpa, int batchYear) {
    if (byId.contains(id))
    { 
        return false;
    }

    Student temp{id, name, cgpa, batchYear};
    if (!byId.insert(id, temp)) return false;

    Student* stored = byId.find(id);
    bool okName = byName.insert(StudentNameKey{name, id}, stored);
    bool okCgpa = byCgpa.insert(StudentCgpaKey{cgpa, id}, stored);
    bool okBatch = byBatch.insert(StudentBatchKey{batchYear, id}, stored);

    if (!okName || !okCgpa || !okBatch) {
        if (okName) byName.erase(StudentNameKey{name, id});
        if (okCgpa) byCgpa.erase(StudentCgpaKey{cgpa, id});
        if (okBatch) byBatch.erase(StudentBatchKey{batchYear, id});
        byId.erase(id);
        return false;
    }
    return true;
}

bool StudentRecords::removeStudent(int id) {
    Student* s = byId.find(id);
    if (s == nullptr) return false;
    Student copy = *s;
    byName.erase(StudentNameKey{copy.name, copy.id});
    byCgpa.erase(StudentCgpaKey{copy.cgpa, copy.id});
    byBatch.erase(StudentBatchKey{copy.batchYear, copy.id});
    byId.erase(id);
    return true;
}

bool StudentRecords::updateCgpa(int id, double newCgpa) {
    Student* s = byId.find(id);
    if (s == nullptr) return false;
    byCgpa.erase(StudentCgpaKey{s->cgpa, s->id});
    s->cgpa = newCgpa;
    byCgpa.insert(StudentCgpaKey{s->cgpa, s->id}, s);
    return true;
}

Student* StudentRecords::findById(int id) { return byId.find(id); }
const Student* StudentRecords::findById(int id) const { return byId.find(id); }

std::vector<Student> StudentRecords::alphabeticalList() const {
    std::vector<Student> ans;
    byName.inorder([&](auto node) { ans.push_back(*(node->value)); });
    return ans;
}

std::vector<Student> StudentRecords::prefixSearch(const std::string& prefix) const {
    std::vector<Student> ans;
    byName.inorder([&](auto node) {
        if (startsWith(node->value->name, prefix)) ans.push_back(*(node->value));
    });
    return ans;
}

std::vector<Student> StudentRecords::studentsInBatch(int batchYear) const {
    std::vector<Student> ans;
    StudentBatchKey lo{batchYear, std::numeric_limits<int>::min()};
    StudentBatchKey hi{batchYear, std::numeric_limits<int>::max()};
    auto nil = byBatch.getNil();
    auto node = byBatch.lowerBoundNode(lo);
    while (node != nil && !(hi < node->key)) {
        ans.push_back(*(node->value));
        node = byBatch.upperBoundNode(node->key);
    }
    return ans;
}

std::vector<Student> StudentRecords::studentsInCgpaRange(double low, double high) const {
    std::vector<Student> ans;
    StudentCgpaKey lo{low, std::numeric_limits<int>::min()};
    StudentCgpaKey hi{high, std::numeric_limits<int>::max()};
    auto nil = byCgpa.getNil();
    auto node = byCgpa.lowerBoundNode(lo);
    while (node != nil && !(hi < node->key)) {
        ans.push_back(*(node->value));
        node = byCgpa.upperBoundNode(node->key);
    }
    return ans;
}

Student* StudentRecords::kthSmallestCgpa(int k) {
    if (k <= 0 || k > byCgpa.size()) return nullptr;
    auto node = byCgpa.getRoot();
    auto nil = byCgpa.getNil();
    while (node != nil) {
        int leftSize = node->left->subtreeSize;
        if (k == leftSize + 1) return node->value;
        if (k <= leftSize) node = node->left;
        else {
            k = k - leftSize - 1;
            node = node->right;
        }
    }
    return nullptr;
}

int StudentRecords::rankByCgpa(double cgpa) const {
    int rank = 1;
    auto node = byCgpa.getRoot();
    auto nil = byCgpa.getNil();
    StudentCgpaKey key{cgpa, std::numeric_limits<int>::min()};
    while (node != nil) {
        if (!(node->key < key)) node = node->left;
        else {
            rank += node->left->subtreeSize + 1;
            node = node->right;
        }
    }
    return rank;
}

int StudentRecords::countLessOrEqual(double cgpa) const {
    int count = 0;
    auto node = byCgpa.getRoot();
    auto nil = byCgpa.getNil();
    StudentCgpaKey key{cgpa, std::numeric_limits<int>::max()};

    while (node != nil) {
        if (!(node->key < key) && !(key < node->key)) {
            count += node->left->subtreeSize + 1;
            node = node->right;
        } else if (key < node->key) {
            node = node->left;
        } else {
            count += node->left->subtreeSize + 1;
            node = node->right;
        }
    }
    return count;
}

int StudentRecords::countCgpaInRange(double low, double high) const {
    if (low > high) return 0;
    return countLessOrEqual(high) - countLessOrEqual(low - 0.0000001);
}

int StudentRecords::size() const { return byId.size(); }

void StudentRecords::printAll() const {
    auto list = alphabeticalList();
    for (const Student& s : list) {
        std::cout << s.id << " | " << s.name << " | CGPA: " << s.cgpa
                  << " | Batch: " << s.batchYear << "\n";
    }
}
