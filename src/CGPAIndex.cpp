// Member 2

#include "CGPAIndex.hpp"

CGPAIndex::CGPAIndex() = default;

std::string CGPAIndex::makeKey(double cgpa, const std::string& id) {}

void CGPAIndex::insert(const Student& student) {}

bool CGPAIndex::remove(double cgpa, const std::string& id) {}

std::vector<Student> CGPAIndex::getInRange(double low, double high) const {}

int CGPAIndex::countInRange(double low, double high) const {}

Student CGPAIndex::kthSmallest(int k) const {}

std::vector<Student> CGPAIndex::topK(int k) const {}
