#include <iostream>
#include <time.h>

using namespace std;

int main()
{
    time_t current_timestamp, birth_timestamp;
    struct tm* birth_time;

    int year, month, mday;
    int hour, min;

    cout << "Enter your birthdate: " << endl;

    cout << "Year: ";
    cin >> year;

    cout << "Month: ";
    cin >> month;

    cout << "Day: ";
    cin >> mday;

    cout << "Hour: ";
    cin >> hour;

    cout << "Minutes: ";
    cin >> min;

    // obtain current time
    time(&current_timestamp);

    // allocate tm structure and modify
    // according to birth date
    birth_time = localtime(&current_timestamp);
    birth_time->tm_year = year - 1900;
    birth_time->tm_mon = month - 1;
    birth_time->tm_mday = mday;
    birth_time->tm_hour = hour;
    birth_time->tm_min = min;
    birth_time->tm_sec = 0;

    birth_timestamp = mktime(birth_time);

    cout << (current_timestamp - birth_timestamp) << endl;

    return 0;
}