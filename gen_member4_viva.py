from fpdf import FPDF
from datetime import datetime

class VivaGuide(FPDF):
    def header(self):
        self.set_font('Helvetica', 'B', 12)
        self.set_fill_color(25, 50, 100)
        self.set_text_color(255, 255, 255)
        self.cell(0, 10, 'Member 4 - Course Registration & Integration | Viva Preparation',
                 fill=True, align='C', new_x='LMARGIN', new_y='NEXT')
        self.set_text_color(0, 0, 0)
        self.ln(2)

    def footer(self):
        self.set_y(-12)
        self.set_font('Helvetica', 'I', 8)
        self.cell(0, 8, f'Page {self.page_no()} | CS201: Data Structures II | sm09751',
                 align='C')

    def section(self, title):
        self.ln(3)
        self.set_font('Helvetica', 'B', 11)
        self.set_fill_color(200, 220, 250)
        self.cell(0, 8, title, fill=True, new_x='LMARGIN', new_y='NEXT')
        self.ln(1)

    def subsection(self, title):
        self.ln(2)
        self.set_font('Helvetica', 'B', 10)
        self.set_text_color(30, 60, 120)
        self.cell(0, 7, '> ' + title, new_x='LMARGIN', new_y='NEXT')
        self.set_text_color(0, 0, 0)

    def q_and_a(self, q, a):
        self.set_font('Helvetica', 'B', 9.5)
        self.set_text_color(180, 0, 0)
        self.multi_cell(0, 5, f'Q: {q}', new_x='LMARGIN', new_y='NEXT')
        self.set_font('Helvetica', '', 9)
        self.set_text_color(30, 30, 30)
        self.set_x(self.l_margin + 5)
        self.multi_cell(0, 5, f'A: {a}', new_x='LMARGIN', new_y='NEXT')
        self.ln(1.5)

    def code_box(self, code_text):
        self.set_font('Courier', '', 8)
        self.set_fill_color(240, 240, 240)
        self.set_x(self.l_margin + 3)
        self.multi_cell(0, 3.5, code_text, border=1, fill=True, new_x='LMARGIN', new_y='NEXT')
        self.ln(1)

pdf = VivaGuide()
pdf.set_margins(15, 15, 15)
pdf.set_auto_page_break(auto=True, margin=15)
pdf.add_page()

# Title & Overview
pdf.set_font('Helvetica', 'I', 10)
pdf.set_text_color(80, 80, 80)
pdf.cell(0, 5, 'UniCore: University Management Suite using Red-Black Trees', align='C',
        new_x='LMARGIN', new_y='NEXT')
pdf.cell(0, 5, f'Generated: {datetime.now().strftime("%B %d, %Y")}', align='C',
        new_x='LMARGIN', new_y='NEXT')
pdf.set_text_color(0, 0, 0)
pdf.ln(2)

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('PROPOSAL REQUIREMENTS FOR MEMBER 4')

pdf.set_font('Helvetica', '', 9.5)
req_text = '''Member 4 is responsible for the Course Registration & Integration subsystem:
- Build an RBTree keyed by composite priority (CGPA desc, batchYear asc, timestamp asc)
- Implement waitlist management with automatic sorting by student priority
- Provide dynamic re-prioritization when student CGPA changes
- Implement cross-subsystem queries combining Student Records with the waitlist
- Create an interactive CLI menu to demo all four subsystems
- Run benchmarks comparing RBTree operations against naive vector implementations'''
pdf.multi_cell(0, 4, req_text, new_x='LMARGIN', new_y='NEXT')

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('CORE DATA STRUCTURES')

pdf.subsection('PriorityKey Struct')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'struct PriorityKey {\n'
    '    double cgpa;              // Higher CGPA = higher priority\n'
    '    int batchYear;            // Earlier batch year = higher priority (seniors first)\n'
    '    long long timestamp;      // Earlier signup = higher priority (FCFS within same CGPA/batch)\n'
    '};\n\n'
    'The comparator: a < b if (a.cgpa > b.cgpa) OR (same CGPA AND a.batchYear < b.batchYear)\n'
    'OR (same both AND a.timestamp < b.timestamp).\n'
    'This ensures RBTree.front() (leftmost) is always the highest-priority student.',
    border=0)

pdf.subsection('WaitlistEntry Struct')
pdf.multi_cell(0, 4,
    'struct WaitlistEntry { std::string studentId; };\n\n'
    'Contains only the student ID. The entry is stored as the VALUE in the RBTree<PriorityKey, WaitlistEntry>.',
    border=0)

pdf.subsection('CourseWaitlist Class')
pdf.multi_cell(0, 4,
    'class CourseWaitlist : public RBTree<PriorityKey, WaitlistEntry>\n'
    '- Inherits all RBTree functionality (insert, remove, find, traversal)\n'
    '- Adds promoteFront() to pop the highest-priority (leftmost) node\n'
    '- Tree automatically maintains sorted order via the PriorityKey comparator',
    border=0)

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('IMPLEMENTED FUNCTIONS')

pdf.subsection('1. addToWaitlist(id, cgpa, batch, time)')
pdf.code_box(
    'void CourseRegistration::addToWaitlist(\n'
    '    const std::string& id, double cgpa, int batch, long long time) {\n'
    '    PriorityKey key{cgpa, batch, time};\n'
    '    WaitlistEntry entry{id};\n'
    '    waitlist.insert(key, entry);\n'
    '}')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'Purpose: Add a student to the waitlist with their priority fields. The RBTree automatically places them in the correct sorted position based on the PriorityKey comparator.\n'
    'Time Complexity: O(log n) - standard RBTree insert with rebalancing\n'
    'Side Effect: If a duplicate key exists, the entry is updated in-place (see RBTree line 131-133).',
    border=0)

pdf.subsection('2. promoteFront()')
pdf.code_box(
    'std::string CourseWaitlist::promoteFront() {\n'
    '    Node* node = front();  // RBTree::front() = leftmost node\n'
    '    if (node == nullptr || node == NIL) return \"\";\n'
    '    std::string id = node->value.studentId;\n'
    '    remove(node->key);\n'
    '    return id;\n'
    '}')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'Purpose: When a course seat opens, remove and return the highest-priority student from the front of the waitlist.\n'
    'Time Complexity: O(log n) - front() walks left (O(log n)) + remove() rebalances (O(log n))\n'
    'Note: Returns empty string if waitlist is empty.',
    border=0)

pdf.subsection('3. reprioritize(id, oldCgpa, oldBatch, oldTime, newCgpa, newBatch, newTime)')
pdf.code_box(
    'void CourseRegistration::reprioritize(\n'
    '    const std::string& id,\n'
    '    double oldCgpa, int oldBatch, long long oldTime,\n'
    '    double newCgpa, int newBatch, long long newTime) {\n'
    '    PriorityKey oldKey{oldCgpa, oldBatch, oldTime};\n'
    '    waitlist.remove(oldKey);\n'
    '    PriorityKey newKey{newCgpa, newBatch, newTime};\n'
    '    waitlist.insert(newKey, WaitlistEntry{id});\n'
    '}')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'Purpose: Update a student\'s priority when their CGPA changes (e.g., after grade update). Must remove OLD entry and reinsert with NEW key.\n'
    'Why remove+reinsert? Cannot change a node\'s key in-place because the BST structure depends on key ordering. Modifying in-place would silently corrupt tree invariants.\n'
    'Time Complexity: O(log n) for remove + O(log n) for insert = O(log n) total\n'
    'Note: All 6 parameters are required to reconstruct both the old and new keys.',
    border=0)

pdf.subsection('4. topKFromBatch(batchYear, k)')
pdf.code_box(
    'std::vector<std::string> CourseRegistration::topKFromBatch(\n'
    '    int batchYear, size_t k) const {\n'
    '    std::vector<std::string> result;\n'
    '    waitlist.inorder([&](const PriorityKey& key,\n'
    '                          const WaitlistEntry& entry) {\n'
    '        if (result.size() < k && key.batchYear == batchYear)\n'
    '            result.push_back(entry.studentId);\n'
    '    });\n'
    '    return result;\n'
    '}')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'Purpose: Cross-subsystem query - return up to k highest-priority students on the waitlist from a specific batch year.\n'
    'How it works: inorder() traversal visits nodes from smallest to largest key. Since the comparator makes highest CGPA the smallest key, inorder yields highest priority first.\n'
    'Time Complexity: O(n) worst case if k is large or no batch matches. A better implementation would use a secondary index keyed by (batchYear, PriorityKey) to achieve O(log n + k).\n'
    'Result: Students returned in priority order (highest CGPA first within the batch).',
    border=0)

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('KEY ALGORITHMIC CONCEPTS')

pdf.subsection('Why Higher CGPA is "Less Than" in the Comparator')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'The RBTree stores keys in sorted order (BST property). The leftmost node (minimum) is accessed via front().\n'
    'Goal: Make the highest-priority student (best CGPA) the leftmost node.\n'
    'Solution: Define the comparator so that higher CGPA values are considered "smaller" than lower CGPA values.\n'
    'Concretely: operator< returns true when (a.cgpa > b.cgpa), so 4.0 < 3.9 in the tree\'s ordering.\n'
    'Result: 4.0 CGPA student is the minimum (leftmost, front) and gets promoted first.',
    border=0)

pdf.subsection('Why Remove+Reinsert for reprioritize()')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'Problem: If you change a node\'s key in-place (e.g., set node->key.cgpa = newCgpa), the node is now in the wrong position.\n'
    'Why? The tree structure was built assuming the node\'s key was the OLD value. The node\'s subtree placement depends on its key.\n'
    'Consequence: Subsequent searches, insertions, or deletions traverse the wrong path. Binary search property is violated.\n'
    'Solution: Remove the entry (restructure tree, rebalance), then reinsert with the new key (tree restructures for new position).\n'
    'This is the ONLY correct way to change a key in a BST.',
    border=0)

pdf.subsection('Why inorder() Yields Highest Priority First')
pdf.set_font('Helvetica', '', 9)
pdf.multi_cell(0, 4,
    'Definition: inorder() traversal visits nodes from smallest to largest key (standard BST in-order).\n'
    'In our tree: "smallest" key = highest CGPA (because comparator defines a.cgpa > b.cgpa as a < b).\n'
    'Therefore: inorder() visits the best student first, then second-best, etc.\n'
    'Application: topKFromBatch() iterates inorder and collects matching students until reaching k, yielding them in priority order.',
    border=0)

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('VIVA QUESTIONS & ANSWERS')

pdf.q_and_a(
    'Why does the composite key need three fields (CGPA, batch, timestamp)?',
    'To break ties in a well-defined way. If two students have the same CGPA, batch year selection (seniors first) is fair. If they also have the same batch, signup order (FCFS) is fair. All three fields together guarantee a unique key for every student.'
)

pdf.q_and_a(
    'What happens if two students have identical CGPA, batch year, AND timestamp?',
    'This should never happen in practice (timestamps are unique). But if it does, the RBTree.insert() overwrites the entry for that key (see RBTree line 131-133). Only one student can occupy that exact PriorityKey.'
)

pdf.q_and_a(
    'Walk through a promoteFront() call. What are the steps and time complexities?',
    'Step 1: front() walks left pointers from root until reaching the leftmost node (O(log n)). Step 2: Extract the studentId from the leftmost node (O(1)). Step 3: remove() deletes the node and rebalances the tree (O(log n) rotations + fixup). Total: O(log n). The promoted student is no longer in the waitlist.'
)

pdf.q_and_a(
    'Why can\'t you use a std::set<PriorityKey> instead of rolling your own RBTree?',
    'The project explicitly requires building an RBTree from scratch to understand the data structure deeply. Also, std::set doesn\'t let you add augmentations like subtreeSize. The learning goal is understanding RBT implementation, not just using it.'
)

pdf.q_and_a(
    'Explain the topKFromBatch() function and its time complexity.',
    'It calls inorder() with a lambda that collects up to k students matching the target batchYear. Since inorder visits every node, the worst case is O(n). A better approach (O(log n + k)) would require a secondary index keyed by (batchYear, priority). The current implementation is acceptable for a proof-of-concept cross-subsystem query.'
)

pdf.q_and_a(
    'What is a "cross-subsystem query" and how does topKFromBatch() demonstrate it?',
    'A cross-subsystem query combines functionality from multiple subsystems. topKFromBatch() "walks" the Course Registration waitlist (our subsystem) while filtering by batch year (a concept from Student Records). It shows how two independent data structures can be queried together.'
)

pdf.q_and_a(
    'What would happen if you forgot to call remove() inside promoteFront()?',
    'The student would still be marked as promoted (their ID is returned), but they\'d remain in the waitlist. Calling promoteFront() again would return the same student. This violates the semantic of "promotion" (removal from waitlist when a seat opens).'
)

pdf.q_and_a(
    'Design reprioritize() to handle the case where the OLD key is not found in the tree.',
    'You could check if remove() returns false (no node found), then skip the insert and log an error. The current implementation assumes the old key exists (as provided by the application layer). In a production system, you\'d add error handling.'
)

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('EDGE CASES & TRICKY SCENARIOS')

pdf.set_font('Helvetica', '', 9)
edge_cases = '''
[1] Empty Waitlist: promoteFront() returns empty string. topKFromBatch() returns empty vector. Both handled gracefully.

[2] Duplicate Key: If a student with (CGPA, batch, timestamp) is added twice, second insert() overwrites. One entry per key only.

[3] Entire Waitlist from One Batch: topKFromBatch(2022, 100) where all students are from batch 2022. Works correctly; returns all in priority order.

[4] No Students from Requested Batch: topKFromBatch(1990, 5) when no 1990 students exist. Returns empty vector.

[5] Large k: topKFromBatch(2022, 1000) when only 10 students from batch 2022 exist. Returns 10 entries (doesn't pad to k).

[6] CGPA Increases: Student reprioritized from 3.5 to 3.8. Must know old key (3.5, batch, time) to remove, then reinsert with new key (3.8, batch, time). Still O(log n).

[7] Timestamp Collision: Two students signup at exact same microsecond. Use incrementing counter instead of system time. Project uses explicit parameter, so application responsibility.

[8] Tree Rebalancing: After promoteFront() removes highest-priority student, tree rebalances. Next front() may take different path but still O(log n).
'''
pdf.multi_cell(0, 3.5, edge_cases, new_x='LMARGIN', new_y='NEXT')

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('BENCHMARKS: WHAT TO EXPECT')

pdf.set_font('Helvetica', '', 9)
bench_text = '''
Course Registration Benchmark (promoteFront × 5000):
• RBTree: O(n log n) = ~5000 * log(5000) ≈ 60,000 operations
• Sorted Vector with erase(begin()): O(n²) = 5000 * 5000 / 2 ≈ 12.5 million operations
• Expected Speedup: 200x+ (vector much slower due to element shifts)
• Real Wall-Clock: RBTree ~1ms, Vector ~100ms on modern CPU

Event Scheduler, Library Manager, Student Records Benchmarks (Q=1000 queries):
• RBTree: O(Q log n) = ~1000 * 12 ≈ 12,000 operations
• Naive Scan: O(Q * n) = ~1000 * 5000 = 5,000,000 operations
• Expected Speedup: 400x+ due to pruning via augmentations

Why Course Registration is the Most Dramatic:
The speedup compounds over all N removals. N removals at O(log n) vs O(n) each = O(n log n) vs O(n²) total.
With N=5000, this is a quadratic difference, not just a constant factor.
'''
pdf.multi_cell(0, 3.8, bench_text, new_x='LMARGIN', new_y='NEXT')

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('TIME COMPLEXITY CHEAT SHEET')

pdf.set_font('Courier', 'B', 9)
complexities = [
    ('addToWaitlist(id, cgpa, batch, time)', 'O(log n)'),
    ('promoteFront()', 'O(log n)'),
    ('reprioritize(...)', 'O(log n)'),
    ('topKFromBatch(batch, k)', 'O(n) worst case'),
    ('waitlist.front()', 'O(log n)'),
    ('waitlist.remove(key)', 'O(log n)'),
    ('waitlist.insert(key, entry)', 'O(log n)'),
]
for op, tc in complexities:
    pdf.set_text_color(60, 60, 60)
    pdf.cell(120, 5, op, border=1)
    pdf.set_text_color(0, 100, 0)
    pdf.cell(0, 5, tc, border=1, new_x='LMARGIN', new_y='NEXT')

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('WHAT NOT TO SAY IN THE VIVA')

pdf.set_font('Helvetica', '', 9.5)
bad_answers = [
    ('[WRONG] "I can change the key in-place by updating node->key.cgpa."',
     '[CORRECT] This breaks the BST property silently. Incorrect answer.'),
    ('[WRONG] "topKFromBatch is O(log n) because I use inorder()."',
     '[CORRECT] inorder() visits all n nodes in worst case, so its O(n). Not O(log n).'),
    ('[WRONG] "promoteFront is O(1) because it just removes the front node."',
     '[CORRECT] Removal includes rebalancing/rotations, which takes O(log n). Not O(1).'),
    ('[WRONG] "I could use a std::vector sorted by priority; insertions would be O(1)."',
     '[CORRECT] Vector insertions are O(n) (element shifts). RBTree is O(log n) and better.'),
    ('[WRONG] "The timestamp field is optional."',
     '[CORRECT] Without it, two students with same CGPA and batch would have duplicate keys. Required for uniqueness.'),
]
for bad, correct in bad_answers:
    pdf.set_text_color(200, 0, 0)
    pdf.multi_cell(0, 4, bad, new_x='LMARGIN', new_y='NEXT')
    pdf.set_text_color(0, 120, 0)
    pdf.set_x(pdf.l_margin + 10)
    pdf.multi_cell(0, 4, correct, new_x='LMARGIN', new_y='NEXT')
    pdf.set_text_color(0, 0, 0)
    pdf.ln(1)

# ──────────────────────────────────────────────────────────────────────────────
pdf.section('FINAL TIPS FOR THE VIVA')

pdf.set_font('Helvetica', '', 9.5)
tips = '''
1. MEMORIZE THE COMPARATOR
   The PriorityKey comparator is THE KEY to understanding why front() = highest priority.
   Know it by heart: cgpa (descending), then batchYear (ascending), then timestamp (ascending).

2. UNDERSTAND REMOVE+REINSERT
   This is the most common viva question for reprioritize(). Be ready to explain why you can't change a key in-place.

3. KNOW YOUR TIME COMPLEXITIES
   Every operation should have a clear O() complexity with justification. Don't guess.

4. TEST YOUR CODE
   Make sure addToWaitlist, promoteFront, reprioritize, and topKFromBatch all work correctly with the RBTree.
   Test edge cases: empty waitlist, duplicate batch queries, large k values.

5. EXPLAIN CROSS-SUBSYSTEM QUERIES
   topKFromBatch is your Module 4 contribution. Be ready to explain how it "combines" the waitlist with batch filtering.

6. BENCHMARK INSIGHTS
   Understand why Course Registration shows the biggest speedup. Be able to explain O(n log n) vs O(n²).

7. KNOW THE SHARED RBTree BASE
   You're not building the RBTree alone-all 4 members contributed. Know the rotation hook, fixup cases, and augmentation mechanism.

8. MENTION THE PROPOSAL
   Reference the proposal when answering. Example: "According to the proposal, Member 4 must implement promoteFront()
   to pop the highest-priority student in O(log n)..." Shows you read and understood it.
'''
pdf.multi_cell(0, 3.5, tips, new_x='LMARGIN', new_y='NEXT')

pdf.output(r'c:\Users\SOHAIB MUHAMMAD\OneDrive\Documents\GitHub\Multi-View-Student-Records\Member4_Viva_Prep.pdf')
print('PDF generated successfully: Member4_Viva_Prep.pdf')
