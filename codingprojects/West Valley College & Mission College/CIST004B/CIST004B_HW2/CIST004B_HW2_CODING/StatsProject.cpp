// Caden Roberts 2/12/23 CIST004B
#include "stats.h"
#include "assert.h"
using namespace std;
using namespace main_savitch_2C;
statistician::statistician() {
	count = 0;
	total = 0;
	largest = 0;
	tinyest = 0;
}
void statistician::next(double r) {
	if (count == 0) {
		tinyest = r;
		largest = r;
	}
	count += 1;
	total += r;
	tinyest = tinyest < r ? tinyest : r;
	largest = largest > r ? largest : r;
}
void statistician::reset() {
	statistician s;
	*this = s;
}
int statistician::length() const {
	return count;
}
double statistician::sum() const {
	return total;
}
double statistician::mean() const {
	assert(count > 0);
	return total / (static_cast<double>(count));
}
double statistician::minimum() const {
	assert(count>0);
	return tinyest;
}
double statistician::maximum() const {
	assert(count > 0);
	return largest;
}
statistician main_savitch_2C::operator + (const statistician& s1, const statistician& s2) {
	if (s1.length() == 0) return s2;
	if (s2.length() == 0) return s1;
	statistician s;
	for (double i = static_cast<double>(s1.length() + s2.length() - 2), a = (static_cast<double>(s1.sum() + s2.sum() - min(s1.minimum(), s2.minimum()) - max(s1.maximum(), s2.maximum())) / i); i > 0; i--) s.next(a);
	s.next(min(s1.minimum(), s2.minimum()));
	s.next(max(s1.maximum(), s2.maximum()));
	return s;
}
statistician main_savitch_2C::operator * (double scale, const statistician& s) {
	if (s.length() == 0) return s;
	statistician s1;
	for (int i = s.length() - 2, a = (s.sum() * scale - s.minimum() * scale - s.maximum() * scale) / i; i > 0; i--) s1.next(a);
	s1.next(s.minimum() * scale);
	s1.next(s.maximum() * scale);
	return s1;
}
bool main_savitch_2C::operator == (const statistician& s1, const statistician& s2) {
	if ((s1.length() > 0) && (s2.length() > 0)) return ((s1.minimum() == s2.minimum()) && (s1.maximum() == s2.maximum()) && (s1.length() == s2.length()) && (s1.mean() == s2.mean()) && (s1.sum() == s2.sum()));
	else if ((s1.length() == 0) && (s2.length() == 0)) return true;
	else return false;
}