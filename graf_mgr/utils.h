
int strToInt(std::string s) {
	bool m = false;
	int tmp = 0;
	int i = 0;
	if (s[0] == '-') {
		i++;
		m = true;
	}
	while (i < s.size()) {
		tmp = 10 * tmp + s[i] - 48;
		i++;
	}
	return m ? -tmp : tmp;
}

std::string intToStr(int n) {
	std::string tmp, ret;
	if (n < 0) {
		ret = "-";
		n = -n;
	}
	do {
		tmp += n % 10 + 48;
		n -= n % 10;
	} while (n /= 10);
	for (int i = tmp.size() - 1; i >= 0; i--) ret += tmp[i];
	return ret;
}

std::string trim(std::string &str) {
	if (str.empty()) return str;
	std::size_t firstScan = str.find_first_not_of(' ');
	std::size_t first = firstScan == std::string::npos ? str.length() : firstScan;
	std::size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}