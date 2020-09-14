#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <time.h>
#define NOMINMAX
#ifdef _WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif
#ifdef _WINDOWS
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif
using namespace std;
int div(vector<int> &a, int l, int r);
void quick_sort(vector<int> &a, int l, int r, int flag);
void find_quick_sort_mid(vector<int> &a, int l, int r); //mid to left
bool isdigit(string& str);
void find_mid(vector<int> &a, int l, int r);

uint64_t durationInUs() {
	struct timeval Current;
	gettimeofday(&Current, nullptr);
	uint64_t lastTime = Current.tv_sec * 1000000 + Current.tv_usec;
	return lastTime;
}

class AutoTime {
public:
	AutoTime(string name) :mName(name) {
		mStartTime = durationInUs();
	}
	~AutoTime() {
		mEndTime = durationInUs();
		printf("%s cost time: %f ms\n", mName.c_str(), (float)(mEndTime - mStartTime) / 1000.0f);
	}

private:
	string mName;
	uint64_t mEndTime;
	uint64_t mStartTime;
};


void arrayGenerate(vector<int>& arr, int n, int rangeL, int rangeR) {
	arr.resize(n);
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		arr[i] = rand() % (rangeR - rangeL + 1) + rangeL;
	}
}

void benchMark(int length) {
	vector<int> testData0, testData1;
	testData0.resize(length);
	arrayGenerate(testData0, length, 0, 10086);
	testData1.resize(length);
	testData1.assign(testData0.begin(), testData0.end());

	{
		AutoTime test1("quick sort(Random)");
		quick_sort(testData0, 0, length - 1, 2);
		/*        cout << "quick sort(Random) result is : " << endl;
		        for (int i = 0; i < length; i++)
		            cout << testData0[i] << " ";
		        cout << endl;	*/
	}

	{
		AutoTime test2("quick sort(avg)");
		quick_sort(testData1, 0, length - 1, 3);
		/*        cout << "quick sort(avg) result is : " << endl;
		        for (int i = 0; i < length; i++)
		            cout << testData1[i] << " ";
		        cout << endl;		*/
	}
	{
		AutoTime test3("quick sort(median)");
		quick_sort(testData1, 0, length - 1, 4);
		/*        cout << "quick sort(median) result is : " << endl;
		for (int i = 0; i < length; i++)
		cout << testData1[i] << " ";
		cout << endl;		*/
	}

}
void get_vector(vector<int> &a)
{
	char ch1;
	int num1, count = 0;
	int flag = 0;
	do {
		flag = 0;
		cout << "Please input your test array (Space bar to separate): " << std::endl;
		while ((ch1 = cin.get()) != '\n') {
			if (isspace(ch1))
				continue;
			cin.putback(ch1);
			if (cin >> num1) {
				a.push_back(num1);
				count++;
			}
			else {
				cout << "input data format error !!! " << endl;
				a.clear();	//clear 
				count = 0;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "input again:" << endl;
			}
		}
		int n = a.size();
		if (n == 0)
			flag = 1;
	} while (flag);
}

void InsertSort(vector<int> &a, int l, int r)
{
	for (int i = l + 1; i <= r; i++)
	{
		if (a[i - 1] > a[i])
		{
			int t = a[i];
			int j = i;
			while (j > l && a[j - 1] > t)
			{
				a[j] = a[j - 1];
				j--;
			}
			a[j] = t;
		}
	}
}

// 
int FindMid(vector<int> &a, int l, int r)
{
	if (l == r) return a[l];
	int i = 0;
	int n = 0;
	for (i = l; i < r - 5; i += 5)
	{
		InsertSort(a, i, i + 4);
		n = i - l;
		swap(a[l + n / 5], a[i + 2]);
	}

	//
	int num = r - i + 1;
	if (num > 0)
	{
		InsertSort(a, i, i + num - 1);
		n = i - l;
		swap(a[l + n / 5], a[i + num / 2]);
	}
	n /= 5;
	if (n == l) return a[l];
	return FindMid(a, l, l + n);
}

//find id 
int FindId(vector<int> &a, int l, int r, int num)
{
	for (int i = l; i <= r; i++)
		if (a[i] == num)
			return i;
	return -1;
}

int Partion(vector<int> &a, int l, int r, int p)
{
	swap(a[p], a[l]);
	int i = l;
	int j = r;
	int pivot = a[l];
	while (i < j)
	{
		while (a[j] >= pivot && i < j)
			j--;
		a[i] = a[j];
		while (a[i] <= pivot && i < j)
			i++;
		a[j] = a[i];
	}
	a[i] = pivot;
	return i;
}

int BFPTR(vector<int> &a, int l, int r, int k)
{
	int num = FindMid(a, l, r);
	int p = FindId(a, l, r, num);
	int i = Partion(a, l, r, p);
	int m = i - l + 1;
	if (m == k) return a[i];
	if (m > k)  return BFPTR(a, l, i - 1, k);
	return BFPTR(a, i + 1, r, k - m);
}

/* div vector by pivot */
int div(vector<int> &a, int l, int r)
{
	int pivot = a[l];
	while (l < r)
	{
		while (a[r] >= pivot && l < r)
			r--;
		a[l] = a[r]; // right -> left 
		while (a[l] <= pivot && l < r)
			l++;
		a[r] = a[l];
	}
	a[l] = pivot;
	return l; // return index 
}

/*   recursion */
void quick_sort(vector<int> &a, int l, int r, int flag)
{
	if (l < r)
	{
		if (flag == 3)
			find_quick_sort_mid(a, l, r);
		else if (flag == 4)
			find_mid(a, l, r);
		int index = div(a, l, r);
		quick_sort(a, l, index - 1, flag);
		quick_sort(a, index + 1, r, flag);
	}
}

/* use left, right, mid to find median */
void find_quick_sort_mid(vector<int> &a, int l, int r)//mid to left
{
	int mid = l + ((r - l) >> 1);
	if (a[mid] > a[r])
		swap(a[mid], a[r]);
	if (a[l] > a[r])
		swap(a[l], a[r]);
	if (a[mid] > a[l])
		swap(a[mid], a[l]);
	return;
}

void find_mid(vector<int> &a, int l, int r)
{
	int num = FindMid(a, l, r);
	int p = FindId(a, l, r, num);
	swap(a[l], a[p]);
	return;
}

bool isdigit(string& str)
{
	bool flag = true;
	for (unsigned int i = 0; i<str.length(); i++)
	{
		if (!isdigit(str[i]))
		{
			flag = false;
			break;
		}
	}
	return flag;
}

int main()
{
	int n;
	int flag = 0;
	char ch, ch1;
	vector<int> tempnum;
	bool flag1 = true;
	do {
		int num = INT_MIN;
	cout << "*******************************************************************" << endl;
	cout << "*******************************************************************" << endl;
	cout << "********************      1 : Find Median		********************" << endl;
	cout << "********************   2 : Random Quick Sort	********************" << endl;
	cout << "********************   3 : avg of l,m,r Quick Sort ********************" << endl;
	cout << "********************	4 : Median Quick Sort	********************" << endl;
	cout << "********************   5 : Performance Test	********************" << endl;
	cout << "********************          6 : Exit			********************" << endl;
	cout << "*******************************************************************" << endl;
	cout << "******************* Please Select Test Case Num *******************" << endl;
		int flag2;
		do {
			tempnum.clear();
			flag2 = 0;
			while ((ch = cin.get()) != '\n') {
				if (isspace(ch))
					continue;
				cin.putback(ch);
				if (cin >> num) {
					tempnum.push_back(num);
				}
				else {
					cout << "input type error !!! please input num " << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "input again:" << endl;
				}
			}
			if (tempnum.size() > 1)
			{
				cout << "input just one num !!!" << endl;
				cout << " please re-enter : " << endl;
				flag2 = 1;
			}
			else if (num == INT_MIN)
			{
				cout << "Please Input data" << endl;
				cout << " please re-enter : " << endl;
				flag2 = 1;
			}
			else if (num == 6)
				break;
			else if (num > 5 || num < 1)
			{
				cout << "Input data can only be between 1 and 6 !!!" << endl;
				cout << " please re-enter : " << endl;
				flag2 = 1;
			}
		} while (flag2 == 1);

		vector<int> a;
		int num2 = INT_MIN;
		switch (num)
		{
		case 1:
			get_vector(a);
			n = a.size();
			cout << "Please select the smaller number K" << endl;
			do {
				tempnum.clear();
				flag2 = 0;
				while ((ch = cin.get()) != '\n') {
					if (isspace(ch))
						continue;
					cin.putback(ch);
					if (cin >> num2) {
						tempnum.push_back(num2);
					}
					else {
						cout << "input type error !!! please input num " << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "input again:" << endl;
					}
				}
				if (tempnum.size() > 1)
				{
					cout << "input just one num !!!" << endl;
					cout << " please re-enter : " << endl;
					flag2 = 1;
				}
				else if (num2 == INT_MIN)
				{
					cout << "Please Input data" << endl;
					cout << " please re-enter : " << endl;
					flag2 = 1;
				}
				else if (num2 > n || num2 < 1)
				{
					cout << "Input data can only be between 1 and "<< n << "!!!" << endl;
					cout << " please re-enter : " << endl;
					flag2 = 1;
				}
			} while (flag2 == 1);
			printf( "The %d th number is : %d\n", num2, BFPTR(a, 0, n - 1, num2));
			break;
		case 2:
			get_vector(a);
			n = a.size();
			quick_sort(a, 0, n - 1, 2);
			cout << "quick sort(Random) result is : " << endl;
			for (int i = 0; i < n; i++)
				cout << a[i] << " ";
			cout << endl;
			break;
		case 3:
			get_vector(a);
			n = a.size();
			quick_sort(a, 0, n - 1, 3);
			cout << "quick sort(avg) result is : " << endl;
			for (int i = 0; i < n; i++)
				cout << a[i] << " ";
			cout << endl;
		case 4:
			get_vector(a);
			n = a.size();
			quick_sort(a, 0, n - 1, 4);
			cout << "quick sort(median) result is : " << endl;
			for (int i = 0; i < n; i++)
				cout << a[i] << " ";
			cout << endl;
			break;
		case 5:
			cout << "Please Input Array Length : " << endl;
			int randomArrayLength;
			cin >> randomArrayLength;
			benchMark(randomArrayLength);
			cin.clear();
			break;
		default:
			cout << "error num" << endl;
		}
		while (true) {
			cout << "continue? [y/n]: ";
			cin >> ch1;
			if (ch1 == 'y') {
				flag1 = true;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				a.clear();
				cout << endl;
			}
			else if (ch1 == 'n')flag1 = false;
			else {
				cout << "err" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			break;
		}
	} while (flag1);
}
