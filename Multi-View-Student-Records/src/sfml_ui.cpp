#include "StudentRecords.hpp"
#include "EventScheduler.hpp"
#include "LibraryManager.hpp"
#include "CourseRegistration.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

struct Button {
    sf::RectangleShape box;
    sf::Text text;
    std::string action;
    int tab;
};

static std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) {
        while (!item.empty() && item.front() == ' ') item.erase(item.begin());
        while (!item.empty() && item.back() == ' ') item.pop_back();
        parts.push_back(item);
    }
    return parts;
}

static bool loadFont(sf::Font& font) {
    std::vector<std::string> paths = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf"
    };
    for (const std::string& p : paths) {
        if (font.loadFromFile(p)) return true;
    }
    return false;
}

static sf::Text makeText(const sf::Font& font, const std::string& value, unsigned int size, float x, float y) {
    sf::Text t;
    t.setFont(font);
    t.setString(value);
    t.setCharacterSize(size);
    t.setFillColor(sf::Color(25, 25, 25));
    t.setPosition(x, y);
    return t;
}


static std::string wrapText(const sf::Font& font, const std::string& text, unsigned int size, float maxWidth) {
    std::stringstream input(text);
    std::string paragraph;
    std::string wrapped;

    while (std::getline(input, paragraph)) {
        std::stringstream words(paragraph);
        std::string word;
        std::string line;

        while (words >> word) {
            std::string testLine = line.empty() ? word : line + " " + word;
            sf::Text tester;
            tester.setFont(font);
            tester.setCharacterSize(size);
            tester.setString(testLine);

            if (tester.getLocalBounds().width > maxWidth && !line.empty()) {
                wrapped += line + "\n";
                line = word;
            } else {
                line = testLine;
            }
        }

        if (!line.empty()) wrapped += line;
        wrapped += "\n";
    }

    if (!wrapped.empty()) wrapped.pop_back();
    return wrapped;
}

static Button makeButton(const sf::Font& font, const std::string& label, const std::string& action,
                         int tab, float x, float y, float w, float h) {
    Button b;
    b.box.setPosition(x, y);
    b.box.setSize(sf::Vector2f(w, h));
    b.box.setFillColor(sf::Color(235, 238, 246));
    b.box.setOutlineThickness(1.f);
    b.box.setOutlineColor(sf::Color(70, 90, 130));
    b.text = makeText(font, label, 15, x + 10, y + 9);
    b.action = action;
    b.tab = tab;
    return b;
}

static Button makeTab(const sf::Font& font, const std::string& label, int tab, float x, float y, float w, float h) {
    Button b = makeButton(font, label, "tab", tab, x, y, w, h);
    b.text.setCharacterSize(16);
    return b;
}

static std::string studentLine(const Student& s) {
    std::ostringstream out;
    out << s.id << " | " << s.name << " | CGPA " << s.cgpa << " | Batch " << s.batchYear;
    return out.str();
}

static std::string eventLine(const Event& e) {
    std::ostringstream out;
    out << e.id << " | " << e.title << " | " << e.start << "-" << e.end << " | " << e.location;
    return out.str();
}

static std::string bookLine(const Book& b) {
    std::ostringstream out;
    out << b.id << " | " << b.title << " | " << b.author << " | Due ";
    if (b.available) out << "N/A";
    else out << LibraryManager::displayDate(b.dueDate);
    out << " | " << (b.available ? "available" : "checked out");
    return out.str();
}

static std::string waitLine(const WaitlistEntry& w) {
    std::ostringstream out;
    out << w.courseCode << " | Student " << w.studentId << " | CGPA " << w.cgpa
        << " | Batch " << w.batchYear << " | Time " << w.timestamp;
    return out.str();
}

static void addSampleData(StudentRecords& students, EventScheduler& events, LibraryManager& library,
                          CourseRegistration& courses, std::string& output) {
    students.addStudent(1, "Ali Khan", 3.70, 2022);
    students.addStudent(2, "sara Ahmed", 3.92, 2021);
    students.addStudent(3, "Bilal Raza", 3.20, 2023);
    students.addStudent(4, "ayesha Noor", 3.82, 2022);
    students.addStudent(5, "Ahmed Iqbal", 3.55, 2022);

    events.addEvent("DS-II Lecture", 900, 1000, "Room A1");
    events.addEvent("Robotics Lab", 1100, 1230, "Lab 2");
    events.addEvent("Society Meetup", 1500, 1600, "Auditorium");

    library.addBook("CLRS", "Cormen");
    library.addBook("Data Structures", "Weiss");
    library.addBook("Algorithms", "Sedgewick");
    library.checkoutBookByTitle("Data Structures", 31052025);

    const Student* s1 = students.findById(1);
    const Student* s2 = students.findById(2);
    const Student* s4 = students.findById(4);
    if (s1) courses.addToWaitlist("CS201", *s1);
    if (s2) courses.addToWaitlist("CS201", *s2);
    if (s4) courses.addToWaitlist("CS201", *s4);

    output = "Sample data loaded. Use the four tabs above to run queries for each subsystem.\n"
             "Student prefix search is case-insensitive, so ali, ALI, and Ali all match Ali Khan.";
}

static std::string helpText(int tab) {
    if (tab == 0) {
        return "Student Records tab\n"
               "Add Student: id,name,cgpa,batch\n"
               "Remove Student: id\n"
               "Update CGPA: id,newCgpa\n"
               "Find by ID: id\n"
               "Prefix Search: any name prefix, e.g. ali or AL\n"
               "Rank by CGPA: cgpa, e.g. 3.70\n"
               "k-th CGPA: k, e.g. 2\n"
               "CGPA Range: low,high, e.g. 3.4,3.9\n"
               "Students in Batch: batchYear, e.g. 2022";
    }
    if (tab == 1) {
        return "Event Scheduler tab\n"
               "Add Event: title,start,end,location\n"
               "Remove Event: eventId only\n"
               "Show All Conflicts: start,end\n"
               "Range Query: start,end\n"
               "Next Event needs no input";
    }
    if (tab == 2) {
        return "Library Management tab\n"
                "Add Book: title,author\n"
                "Checkout Book: bookId,dueDate(DDMMYYYY), e.g. 1,31052025\n"
                "Return Book: bookId only\n"
                "Available Count: no input needed\n"
                "Due Range: d1,d2 in DDMMYYYY for checked-out books\n"
                "Overdue Books: todayDate in DDMMYYYY";
    }
    return "Course Registration tab\n"
           "Add Waitlist: courseCode,studentId\n"
           "Show Waitlist: courseCode\n"
           "Promote Front: courseCode\n"
           "Cross Batch Query: courseCode,batchYear,limit";
}

static std::string runAction(const std::string& action, const std::string& input,
                             StudentRecords& students, EventScheduler& events,
                             LibraryManager& library, CourseRegistration& courses) {
    std::ostringstream out;

    try {
        if (action == "show_students") {
            auto list = students.alphabeticalList();
            out << "All students:\n";
            for (const Student& s : list) out << studentLine(s) << "\n";
            if (list.empty()) out << "No students yet. Click Load Sample Data or add one.\n";
        } else if (action == "add_student") {
            auto p = split(input, ',');
            if (p.size() != 4) return "Format: id,name,cgpa,batchYear";
            bool ok = students.addStudent(std::stoi(p[0]), p[1], std::stod(p[2]), std::stoi(p[3]));
            out << (ok ? "Student added." : "Student ID already exists or insert failed.");
        } else if (action == "prefix") {
            auto list = students.prefixSearch(input);
            out << "Case-insensitive prefix results for \"" << input << "\":\n";
            for (const Student& s : list) out << studentLine(s) << "\n";
            if (list.empty()) out << "No matching names.\n";
        } else if (action == "rank") {
            double cgpa = std::stod(input);
            out << "Rank of CGPA " << cgpa << " = " << students.rankByCgpa(cgpa);
        } else if (action == "kth") {
            int k = std::stoi(input);
            Student* s = students.kthSmallestCgpa(k);
            if (s) out << "k-th lowest CGPA student: " << studentLine(*s);
            else out << "Invalid k.";
                } else if (action == "remove_student") {
            int id = std::stoi(input);
            bool ok = students.removeStudent(id);
            out << (ok ? "Student removed successfully." : "Student not found.");
        } else if (action == "update_cgpa") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: studentId,newCgpa";
            int id = std::stoi(p[0]);
            double newCgpa = std::stod(p[1]);
            bool ok = students.updateCgpa(id, newCgpa);
            out << (ok ? "CGPA updated successfully." : "Student not found.");
        } else if (action == "find_id") {
            int id = std::stoi(input);
            Student* s = students.findById(id);
            if (s) out << "Found: " << studentLine(*s);
            else out << "Student not found.";
        } else if (action == "batch_query") {
            int batch = std::stoi(input);
            auto list = students.studentsInBatch(batch);
            out << "Students in batch " << batch << ":\n";
            for (const Student& s : list) out << studentLine(s) << "\n";
            if (list.empty()) out << "No students in this batch.";
        } else if (action == "show_events") {
            auto list = events.allEvents();
            out << "All events:\n";
            for (const Event& e : list) out << eventLine(e) << "\n";
            if (list.empty()) out << "No events yet.\n";
        } else if (action == "add_event") {
            auto p = split(input, ',');
            if (p.size() != 4) return "Format: title,start,end,location";
            long long start = std::stoll(p[1]);
            long long end = std::stoll(p[2]);
            Event conflict;
            if (end <= start) {
                out << "Event not added. End time must be greater than start time.";
            } else if (events.findFirstConflict(start, end, conflict)) {
                out << "Event not added because it conflicts with:\n" << eventLine(conflict);
            } else if (events.addEvent(p[0], start, end, p[3])) {
                out << "Event added.";
            } else {
                out << "Event not added.";
            }
        } else if (action == "remove_event") {
            if (input.empty()) return "Format: eventId";
            int id = std::stoi(input);
            bool ok = events.removeEvent(id);
            out << (ok ? "Event removed." : "Event not found.");
        } else if (action == "all_conflicts") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: start,end";
            auto list = events.allConflicts(std::stoll(p[0]), std::stoll(p[1]));
            out << "Conflicts for " << input << ":\n";
            for (const Event& e : list) out << eventLine(e) << "\n";
            if (list.empty()) out << "No conflict.\n";
        } else if (action == "event_range") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: start,end";
            auto list = events.rangeQuery(std::stoll(p[0]), std::stoll(p[1]));
            out << "Events in range:\n";
            for (const Event& e : list) out << eventLine(e) << "\n";
            if (list.empty()) out << "No events in range.\n";
        } else if (action == "next_event") {
            Event* e = events.nextEvent();
            if (e) out << "Next event: " << eventLine(*e);
            else out << "No events scheduled.";
        } else if (action == "show_books") {
            auto list = library.allBooks();
            out << "All books:\n";
            for (const Book& b : list) out << bookLine(b) << "\n";
            if (list.empty()) out << "No books yet.\n";
        } else if (action == "add_book") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: title,author";
            bool ok = library.addBook(p[0], p[1]);
            out << (ok ? "Book added as available. Due date = N/A." : "Book insert failed.");
        } else if (action == "available") {
            out << "Currently available books = " << library.availableCount();
        } else if (action == "due_range") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: d1,d2";
            auto list = library.checkedOutDueInRange(std::stoll(p[0]), std::stoll(p[1]));
            out << "Checked-out books due in range:\n";
            for (const Book& b : list) out << bookLine(b) << "\n";
            if (list.empty()) out << "No checked-out books due in this range.";
        } else if (action == "overdue") {
            long long today = std::stoll(input);
            auto list = library.overdueBooks(today);
            out << "Overdue checked-out books:\n";
            for (const Book& b : list) out << bookLine(b) << " | Fine " << library.estimateFine(b, today) << "\n";
            if (list.empty()) out << "No overdue books.\n";
        } else if (action == "checkout") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: bookTitle,dueDate(DDMMYYYY), example: CLRS,31052025";
            long long dueDate = std::stoll(p[1]);
            bool ok = library.checkoutBookByTitle(p[0], dueDate);
            if (ok) out << "Book checked out. Due date = " << LibraryManager::displayDate(LibraryManager::pakToInternal(dueDate));
            else out << "Book not found, not available, or already checked out.";
        } else if (action == "return") {
            if (input.empty()) return "Format: bookId";
            bool ok = library.returnBook(std::stoi(input));
            out << (ok ? "Book returned. Due date is now N/A." : "Book not found or already available.");
        } else if (action == "add_wait") {
            auto p = split(input, ',');
            if (p.size() != 2) return "Format: courseCode,studentId";
            std::string code = p[0];
            for (char& c : code) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            Student* s = students.findById(std::stoi(p[1]));
            if (!s) return "Student ID not found. Add student first or load sample data.";
            bool ok = courses.addToWaitlist(code, *s);
            out << (ok ? "Student added to waitlist." : "Student already in waitlist.");
        } else if (action == "show_wait") {
            if (input.empty()) {
                auto all = courses.allWaitlistEntries();
                out << "All waitlist entries:\n";
                for (const WaitlistEntry& w : all) out << waitLine(w) << "\n";
                if (all.empty()) out << "No waitlist entries. Click Load Sample Data first.\n";
            } else {
                std::string code = input;
                for (char& c : code) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                auto list = courses.courseWaitlist(code);
                out << "Waitlist for " << code << ":\n";
                for (const WaitlistEntry& w : list) out << waitLine(w) << "\n";
                if (list.empty()) out << "No waitlist entries for \"" << code << "\". Leave empty to show all.\n";
            }
        } else if (action == "promote") {
            std::string code = input;
            for (char& c : code) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            WaitlistEntry w;
            if (courses.promoteFront(code, w)) out << "Promoted: " << waitLine(w);
            else out << "No student available for this course.";
        } else if (action == "cross") {
            auto p = split(input, ',');
            if (p.size() != 3) return "Format: courseCode,batchYear,limit";
            std::string code = p[0];
            for (char& c : code) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            auto list = courses.topWaitlistedFromBatch(code, std::stoi(p[1]), std::stoi(p[2]));
            out << "Top waitlisted from batch " << p[1] << ":\n";
            for (const WaitlistEntry& w : list) out << waitLine(w) << "\n";
            if (list.empty()) out << "No matches.\n";
        }
    } catch (const std::exception& e) {
        out << "Input error: " << e.what();
    }

    return out.str();
}

static void addButtonsForTab(std::vector<Button>& buttons, const sf::Font& font, int tab) {
    float x = 35, y = 185, w = 245, h = 42, gap = 12;
    if (tab == 0) {
        buttons.push_back(makeButton(font, "Show All Students", "show_students", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Add Student", "add_student", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Remove Student", "remove_student", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Update CGPA", "update_cgpa", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Find by ID", "find_id", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Prefix Search Name", "prefix", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Rank by CGPA", "rank", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "k-th Lowest CGPA", "kth", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "CGPA Range Query", "cgpa_range", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Students in Batch", "batch_query", tab, x, y, w, h));
    } else if (tab == 1) {
        buttons.push_back(makeButton(font, "Show All Events", "show_events", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Add Event", "add_event", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Remove Event", "remove_event", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Show All Conflicts", "all_conflicts", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Event Range Query", "event_range", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Next Event", "next_event", tab, x, y, w, h));
    } else if (tab == 2) {
        buttons.push_back(makeButton(font, "Show All Books", "show_books", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Add Book", "add_book", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Available Count", "available", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Checked-Out Due Range", "due_range", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Overdue Books", "overdue", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Checkout Book", "checkout", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Return Book", "return", tab, x, y, w, h));
    } else {
        buttons.push_back(makeButton(font, "Add to Waitlist", "add_wait", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Show Course Waitlist", "show_wait", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Promote Front Student", "promote", tab, x, y, w, h)); y += h + gap;
        buttons.push_back(makeButton(font, "Cross Batch Query", "cross", tab, x, y, w, h));
    }
}

int main() {
    sf::Font font;
    if (!loadFont(font)) {
        std::cerr << "Could not load a system font. Edit loadFont() in src/sfml_ui.cpp if needed.\n";
        return 1;
    }

    StudentRecords students;
    EventScheduler events;
    LibraryManager library;
    CourseRegistration courses;

    sf::RenderWindow window(sf::VideoMode(1220, 760), "UniCore - Tabbed SFML UI");
    window.setFramerateLimit(60);

    std::vector<Button> tabs;
    tabs.push_back(makeTab(font, "Student Records", 0, 25, 72, 285, 48));
    tabs.push_back(makeTab(font, "Event Scheduler", 1, 315, 72, 285, 48));
    tabs.push_back(makeTab(font, "Library Management", 2, 605, 72, 285, 48));
    tabs.push_back(makeTab(font, "Course Registration", 3, 895, 72, 285, 48));

    int activeTab = 0;
    std::vector<Button> queryButtons;
    addButtonsForTab(queryButtons, font, activeTab);

    std::string input;
    std::string output = "Welcome to UniCore. Choose one of the four application tabs above, type the query input, then click the relevant query button.\nClick Load Sample Data first for a quick demo.";
    bool typing = false;

    sf::RectangleShape inputBox(sf::Vector2f(820, 42));
    inputBox.setPosition(315, 140);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(sf::Color(70, 90, 130));

    Button loadButton = makeButton(font, "Load Sample Data", "load", -1, 35, 135, 245, 42);
    loadButton.box.setFillColor(sf::Color(220, 235, 225));

    sf::RectangleShape queryPanel(sf::Vector2f(270, 535));
    queryPanel.setPosition(25, 170);
    queryPanel.setFillColor(sf::Color(250, 250, 252));
    queryPanel.setOutlineThickness(1.f);
    queryPanel.setOutlineColor(sf::Color(180, 185, 195));

    sf::RectangleShape outputBox(sf::Vector2f(570, 465));
    outputBox.setPosition(315, 240);
    outputBox.setFillColor(sf::Color(250, 250, 250));
    outputBox.setOutlineThickness(1.f);
    outputBox.setOutlineColor(sf::Color(150, 150, 150));

    sf::RectangleShape helpBox(sf::Vector2f(275, 465));
    helpBox.setPosition(905, 240);
    helpBox.setFillColor(sf::Color(250, 250, 250));
    helpBox.setOutlineThickness(1.f);
    helpBox.setOutlineColor(sf::Color(150, 150, 150));

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();

            if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                typing = inputBox.getGlobalBounds().contains(mouse);

                for (const Button& tab : tabs) {
                    if (tab.box.getGlobalBounds().contains(mouse)) {
                        activeTab = tab.tab;
                        queryButtons.clear();
                        addButtonsForTab(queryButtons, font, activeTab);
                        output = "Selected tab changed.\n" + helpText(activeTab);
                    }
                }

                if (loadButton.box.getGlobalBounds().contains(mouse)) {
                    addSampleData(students, events, library, courses, output);
                }

                for (const Button& b : queryButtons) {
                    if (b.box.getGlobalBounds().contains(mouse)) {
                        output = runAction(b.action, input, students, events, library, courses);
                    }
                }
            }

            if (ev.type == sf::Event::TextEntered) {
                if (ev.text.unicode == 8) {
                    if (!input.empty()) input.pop_back();
                } else if (ev.text.unicode >= 32 && ev.text.unicode < 127) {
                    input.push_back(static_cast<char>(ev.text.unicode));
                }
            }
        }

        window.clear(sf::Color(242, 244, 248));

        sf::Text title = makeText(font, "UniCore University Management Suite", 27, 25, 20);
        title.setFillColor(sf::Color(20, 35, 65));
        window.draw(title);

        for (Button tab : tabs) {
            if (tab.tab == activeTab) {
                tab.box.setFillColor(sf::Color(70, 95, 150));
                tab.text.setFillColor(sf::Color::White);
            } else {
                tab.box.setFillColor(sf::Color(225, 230, 240));
                tab.text.setFillColor(sf::Color(30, 30, 30));
            }
            window.draw(tab.box);
            window.draw(tab.text);
        }

        window.draw(loadButton.box);
        window.draw(loadButton.text);

        window.draw(inputBox);
        sf::Text inputLabel = makeText(font, "Query Input", 15, 315, 118);
        inputLabel.setFillColor(sf::Color(70, 70, 70));
        window.draw(inputLabel);
        sf::Text inputText = makeText(font, input.empty() ? "Click here and type input for the selected query..." : input, 17, 325, 150);
        inputText.setFillColor(input.empty() ? sf::Color(125, 125, 125) : sf::Color(20, 20, 20));
        window.draw(inputText);

        window.draw(queryPanel);
        sf::Text qTitle = makeText(font, "Queries", 20, 35, 178);
        qTitle.setFillColor(sf::Color(20, 35, 65));
        window.draw(qTitle);
        for (const Button& b : queryButtons) {
            window.draw(b.box);
            window.draw(b.text);
        }

        window.draw(outputBox);
        sf::Text outTitle = makeText(font, "Output", 20, 315, 210);
        outTitle.setFillColor(sf::Color(20, 35, 65));
        window.draw(outTitle);
        std::string wrappedOutput = wrapText(font, output, 15, 535.f);
        sf::Text outText = makeText(font, wrappedOutput, 15, 330, 255);
        outText.setLineSpacing(1.15f);
        window.draw(outText);

        window.draw(helpBox);
        sf::Text hTitle = makeText(font, "Input Guide", 20, 915, 210);
        hTitle.setFillColor(sf::Color(20, 35, 65));
        window.draw(hTitle);
        std::string wrappedGuide = wrapText(font, helpText(activeTab), 14, 245.f);
        sf::Text guide = makeText(font, wrappedGuide, 14, 920, 255);
        guide.setLineSpacing(1.2f);
        window.draw(guide);

        window.display();
    }

    return 0;
}
