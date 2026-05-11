from fpdf import FPDF, XPos, YPos

class PDF(FPDF):
    def header(self):
        self.set_fill_color(30, 60, 114)
        self.rect(0, 0, 210, 22, 'F')
        self.set_text_color(255, 255, 255)
        self.set_font('Helvetica', 'B', 13)
        self.set_y(6)
        self.cell(0, 10, 'CS201: Data Structures II  |  UniCore Project',
                  new_x=XPos.LMARGIN, new_y=YPos.NEXT, align='C')
        self.set_text_color(0, 0, 0)
        self.ln(12)

    def footer(self):
        self.set_y(-13)
        self.set_font('Helvetica', 'I', 8)
        self.set_text_color(120, 120, 120)
        self.cell(0, 10, f'Page {self.page_no()}', align='C')

    def section_title(self, title):
        self.set_font('Helvetica', 'B', 12)
        self.set_text_color(30, 60, 114)
        self.set_fill_color(235, 240, 255)
        self.cell(0, 8, title, new_x=XPos.LMARGIN, new_y=YPos.NEXT, fill=True)
        self.set_text_color(0, 0, 0)
        self.ln(1)

    def sub_title(self, title):
        self.set_font('Helvetica', 'B', 10)
        self.set_text_color(50, 50, 150)
        self.cell(0, 7, title, new_x=XPos.LMARGIN, new_y=YPos.NEXT)
        self.set_text_color(0, 0, 0)

    def bullet(self, text, indent=8):
        self.set_font('Helvetica', '', 9.5)
        self.set_x(self.l_margin + indent)
        self.multi_cell(0, 6, '  ' + text)
        self.ln(0.5)

    def body_text(self, text):
        self.set_font('Helvetica', '', 9.5)
        self.multi_cell(0, 6, text)
        self.ln(1)


pdf = PDF()
pdf.set_auto_page_break(auto=True, margin=18)
pdf.add_page()
pdf.set_margins(15, 28, 15)

# Title block
pdf.set_font('Helvetica', 'B', 16)
pdf.set_text_color(30, 60, 114)
pdf.cell(0, 10, 'Member 4 - Viva Preparation Guide',
         new_x=XPos.LMARGIN, new_y=YPos.NEXT, align='C')
pdf.set_font('Helvetica', '', 10)
pdf.set_text_color(80, 80, 80)
pdf.cell(0, 6, 'Sohaib Muhammad  |  sm09751  |  Course Registration & Integration',
         new_x=XPos.LMARGIN, new_y=YPos.NEXT, align='C')
pdf.set_text_color(0, 0, 0)
pdf.ln(4)

# Team table
pdf.section_title('  Team & Project Overview')
members = [
    ('Member 1', 'Syed Ibad Asim (sa09753)',    'Student Records - subtreeSize augmentation, order statistics'),
    ('Member 2', 'Krish Kumar (kk09969)',         'Event Scheduler - maxEnd augmentation, conflict detection'),
    ('Member 3', 'Muhammad Saad (ms10054)',       'Library Manager - subtreeBooks augmentation, overdue detection'),
    ('Member 4', 'Sohaib Muhammad (sm09751)',     'Course Registration - composite key, waitlist, CLI, benchmarks'),
]
col_w = [22, 55, 103]
pdf.set_fill_color(220, 230, 255)
pdf.set_font('Helvetica', 'B', 9)
for h, w in zip(['Member', 'Name / ID', 'Responsibility'], col_w):
    pdf.cell(w, 7, h, border=1, fill=True)
pdf.ln()
for i, (m, n, r) in enumerate(members):
    is_you = (i == 3)
    pdf.set_fill_color(255, 255, 200) if is_you else pdf.set_fill_color(255, 255, 255)
    pdf.set_font('Helvetica', 'B' if is_you else '', 9)
    pdf.cell(col_w[0], 7, m, border=1, fill=is_you)
    pdf.cell(col_w[1], 7, n, border=1, fill=is_you)
    pdf.cell(col_w[2], 7, r, border=1, fill=is_you)
    pdf.ln()
pdf.ln(4)

# Shared work
pdf.section_title('  Shared Work (All 4 Members) - RBTree.hpp')
shared = [
    'Generic RBTree<Key, Value> template with color enums on nodes',
    'Left and right rotations',
    'Insert with fix-up  (uncle-is-red, uncle-is-black cases)',
    'Delete with fix-up  (all double-black propagation cases from CLRS Ch.13)',
    'Basic find, lower_bound, upper_bound, in-order traversal',
    'Rotation hook: called after every rotation so modules can update their augmentation fields',
]
for s in shared:
    pdf.bullet(s)
pdf.ln(3)

# Member 4 module
pdf.section_title('  Your Module - Course Registration & Integration')

pdf.sub_title('Data Structure')
pdf.bullet('RBT keyed by composite priority:  (CGPA desc,  batchYear asc,  timestamp asc)')
pdf.bullet('Highest-priority student is always the LEFTMOST node in the tree')
pdf.bullet('No extra augmentation field needed -- ordering IS the augmentation')
pdf.ln(2)

pdf.sub_title('Algorithms You Must Implement')
algos = [
    ('promoteFront()',
     'When a seat opens, pop the leftmost node in O(log n). Walk left until null, then delete that node.'),
    ('Dynamic re-prioritization',
     "If a student's CGPA changes: delete with old key, reinsert with new key. Cannot update in-place (breaks BST ordering)."),
    ('Cross-subsystem queries',
     '"Top 5 waitlisted from batch 2022": combine Module 1 year-range walk with your waitlist lookup.'),
    ('CLI  (main.cpp)',
     'Text menu that demos all four subsystems from one interface.'),
    ('Benchmarks',
     'Compare each module\'s core query against a sorted std::vector. Show actual O(log n) vs O(n) speedup on large datasets.'),
]
for name, desc in algos:
    pdf.set_font('Helvetica', 'B', 9.5)
    pdf.set_text_color(150, 0, 0)
    pdf.write(6, f'  {name}  ')
    pdf.set_font('Helvetica', '', 9.5)
    pdf.set_text_color(0, 0, 0)
    pdf.multi_cell(0, 6, f'- {desc}')
    pdf.ln(1)
pdf.ln(2)

# Composite key
pdf.section_title('  Composite Key - Deep Dive')
pdf.body_text(
    'The key is a struct with three fields. The comparator must implement strict weak ordering. '
    'Tiebreaker chain:'
)
tiebreakers = [
    '1st - CGPA (descending): higher CGPA has higher priority. Comparator flips sign so higher CGPA goes LEFT.',
    '2nd - batchYear (ascending): earlier batch year wins on CGPA tie. Smaller year goes left normally.',
    '3rd - timestamp (ascending): earlier signup wins on full tie. Smaller timestamp goes left normally.',
]
for t in tiebreakers:
    pdf.bullet(t)
pdf.ln(1)
pdf.body_text(
    'CRITICAL: all three tiebreaker cases must be handled. If any case is missing, '
    'equal-priority students may map to the same key and one will silently overwrite the other.'
)
pdf.ln(2)

# Viva Q&A
pdf.section_title('  Expected Viva Questions & Answers')
qa = [
    ('Why use a composite key instead of a separate priority field?',
     'The composite key IS the priority. The tree stays sorted by priority automatically after every insert/delete -- no extra bookkeeping needed.'),
    ('Why is the highest-priority student the leftmost node?',
     'Because CGPA is sorted descending in the comparator (flipped sign). CGPA 3.9 compares as "less than" 3.5 and goes to the left subtree.'),
    ('Why is promoteFront() O(log n) and not O(1)?',
     'Finding the leftmost node takes O(log n) to traverse down. Deletion then takes O(log n) for fix-up. There is no O(1) min pointer in a plain RBT.'),
    ('Why delete + reinsert for re-prioritization instead of updating the node?',
     'Updating the key in-place breaks the BST ordering invariant -- nodes to the right might now be less than the updated node. Delete + reinsert is the correct approach.'),
    ('How does the cross-subsystem query work?',
     '"Top 5 waitlisted from batch 2022": call Module 1\'s batchYear RBT to get all 2022 students, then for each check if they exist in your waitlist. Return top 5 by waitlist priority.'),
    ('What is the rotation hook and why is it important?',
     'A callback registered with the base RBTree that fires after every rotation. Augmentation fields depend on subtree structure, so they must be updated after rotations. Missing one rotation path causes silent corruption.'),
    ('What is your expected challenge?',
     'The composite key comparator must handle all three tiebreaker cases correctly. A bug here causes wrong insertion order which is very hard to debug.'),
    ('What does your benchmark compare?',
     'Each module\'s core query (e.g. promoteFront) vs. a sorted std::vector doing the same thing. RBT shows O(log n) vs O(n) for large datasets.'),
]
for i, (q, a) in enumerate(qa):
    pdf.set_fill_color(245, 245, 255)
    pdf.set_font('Helvetica', 'B', 9.5)
    pdf.set_text_color(30, 60, 114)
    pdf.multi_cell(0, 6, f'Q{i+1}: {q}', fill=True)
    pdf.set_font('Helvetica', '', 9.5)
    pdf.set_text_color(0, 0, 0)
    pdf.set_x(pdf.l_margin + 5)
    pdf.multi_cell(0, 6, f'A:  {a}')
    pdf.ln(2)

# Challenges
pdf.section_title('  Expected Challenges (from Proposal)')
challenges = [
    'Composite key comparator must handle ALL tiebreaker cases - wrong order if any case is missing',
    'Rotation hook must fire on EVERY rotation path - missing one causes augmentation to silently go wrong',
    'Delete fix-up double-black propagation is the hardest part of the shared RBTree',
    'Cross-subsystem queries require understanding the interface each other module exposes',
    'Benchmarks require generating large datasets and timing both approaches fairly',
]
for c in challenges:
    pdf.bullet(c)
pdf.ln(3)

# Quick-reference cheatsheet
pdf.section_title('  Quick-Reference Cheatsheet')
rows = [
    ('Module',            'Course Registration & Integration'),
    ('Key type',          'Composite struct { double cgpa, int batchYear, time_t timestamp }'),
    ('Tree ordering',     'Leftmost node = highest priority student'),
    ('Core operation',    'promoteFront()  --  O(log n)'),
    ('Re-prioritization', 'delete(old key)  +  insert(new key)'),
    ('CLI',               'main.cpp text menu covering all 4 subsystems'),
    ('Benchmarks',        'RBT vs sorted vector for each module\'s core query'),
    ('Cross-system',      'Coordinate with Module 1 (Student Records) for batch queries'),
    ('Augmentation',      'None extra -- ordering itself is the augmentation'),
    ('Reference',         'CLRS 4th ed. Ch.13 (RBT) + Ch.14 (Augmenting Data Structures)'),
]
pdf.set_font('Helvetica', '', 9.5)
for k, v in rows:
    pdf.set_fill_color(235, 240, 255)
    pdf.set_font('Helvetica', 'B', 9.5)
    pdf.cell(45, 7, f'  {k}', border=1, fill=True)
    pdf.set_font('Helvetica', '', 9.5)
    pdf.set_fill_color(255, 255, 255)
    pdf.cell(0, 7, f'  {v}', border=1, fill=True)
    pdf.ln()

out = r'c:\Users\SOHAIB MUHAMMAD\Downloads\unicore_ds2\Member4_Viva_Guide.pdf'
pdf.output(out)
print(f'Saved: {out}')
