#include <iostream>
#include <fstream>
#include <cmath>

#include "moon_functions.h" // Включаем moon_functions.h

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    // Ввод года (формат "ГГГГ") и даты (формат "ДД.ММ.ГГГГ")
    char inputYear[5];
    char inputDate[11];
    cout << "Введите год (гггг): ";
    cin >> inputYear;
    cout << "Введите дату (дд.мм.гггг): ";
    cin >> inputDate;

    // Проверка формата даты: должно быть ровно 10 символов, точки на позициях 2 и 5
    if (my_strlen(inputDate) != 10 || inputDate[2] != '.' || inputDate[5] != '.') {
        cout << "Ошибка ввода! Используйте формат дд.мм.гггг" << endl;
        return 1;
    }

    // Преобразуем дату в формат "ГГГГММДД"
    char targetDate[9];
    convertDateFormat(inputDate, targetDate);

    // Формирование пути к файлу с использованием шаблона.
    // Шаблон: "..\input\moonYYYY.dat" — подстрока "YYYY" заменяется на введённый год.
    char filePath[256] = "..\\input\\moonYYYY.dat";
    char* pos = strstr(filePath, "YYYY");
    if (pos == nullptr) {
        cout << "Подстрока 'YYYY' не найдена!" << endl;
        return 1;
    }
    memcpy(pos, inputYear, 4);

    cout << "Обновленный путь: " << filePath << endl;

    // Открываем файл
    ifstream file(filePath);
    if (!file) {
        cout << "Ошибка открытия файла: " << filePath << endl;
        return 1;
    }

    // Пропускаем первую строку (заголовок)
    char header[256];
    file.getline(header, 256);

    // Первый проход: подсчитываем количество записей для указанной даты
    int count = 0;
    char file_date[16], file_time[16];
    double t, r, elevation, az, fi, lg;
    while (file >> file_date >> file_time >> t >> r >> elevation >> az >> fi >> lg) {
        if (stringsEqual(file_date, targetDate))
            count++;
    }
    if (count == 0) {
        cout << "Нет данных для указанной даты в файле." << endl;
        return 0;
    }

    // Выделяем динамический массив для хранения данных о Луне
    MoonData* data = new MoonData[count];

    // Возвращаемся в начало файла и снова пропускаем заголовок
    file.clear();
    file.seekg(0);
    file.getline(header, 256);

    int index = 0;
    while (file >> file_date >> file_time >> t >> r >> elevation >> az >> fi >> lg) {
        if (stringsEqual(file_date, targetDate)) {
            // Копируем дату (формат "ГГГГММДД")
            int i = 0;
            while (file_date[i] != '\0' && i < 8) {
                data[index].date[i] = file_date[i];
                i++;
            }
            data[index].date[i] = '\0';

            // Копируем время (формат "ЧЧММСС")
            i = 0;
            while (file_time[i] != '\0' && i < 6) {
                data[index].time[i] = file_time[i];
                i++;
            }
            data[index].time[i] = '\0';

            data[index].t_hours = timeToHours(file_time);
            data[index].elevation = elevation;
            index++;
        }
    }
    file.close();

    // Поиск восхода, захода и кульминации Луны
    bool moonriseFound = false, moonsetFound = false;
    char moonriseTime[7] = "", moonsetTime[7] = "";
    double prevElevation = data[0].elevation;
    // Если первая запись имеет положительную высоту, считаем, что восход уже произошёл
    if (prevElevation > 0) {
        moonriseFound = true;
        int i = 0;
        while (data[0].time[i] != '\0' && i < 6) {
            moonriseTime[i] = data[0].time[i];
            i++;
        }
        moonriseTime[i] = '\0';
    }

    int maxIndex = 0;
    double maxElevation = data[0].elevation;
    for (int i = 0; i < count; i++) {
        if (data[i].elevation > maxElevation) {
            maxElevation = data[i].elevation;
            maxIndex = i;
        }
        if (!moonriseFound && prevElevation < 0 && data[i].elevation >= 0) {
            moonriseFound = true;
            int j = 0;
            while (data[i].time[j] != '\0' && j < 6) {
                moonriseTime[j] = data[i].time[j];
                j++;
            }
            moonriseTime[j] = '\0';
        }
        if (!moonsetFound && prevElevation >= 0 && data[i].elevation < 0) {
            moonsetFound = true;
            int j = 0;
            while (data[i].time[j] != '\0' && j < 6) {
                moonsetTime[j] = data[i].time[j];
                j++;
            }
            moonsetTime[j] = '\0';
        }
        prevElevation = data[i].elevation;
    }

    // Параболическая аппроксимация для кульминации Луны
    char culminationTime[7];
    {
        int j = 0;
        while (data[maxIndex].time[j] != '\0' && j < 6) {
            culminationTime[j] = data[maxIndex].time[j];
            j++;
        }
        culminationTime[j] = '\0';
    }
    // Если есть данные до и после максимума – уточняем время кульминации
    if (count >= 3 && maxIndex > 0 && maxIndex < count - 1) {
        double A[3][3] = {
            { data[maxIndex - 1].t_hours * data[maxIndex - 1].t_hours, data[maxIndex - 1].t_hours, 1.0 },
            { data[maxIndex].t_hours * data[maxIndex].t_hours, data[maxIndex].t_hours, 1.0 },
            { data[maxIndex + 1].t_hours * data[maxIndex + 1].t_hours, data[maxIndex + 1].t_hours, 1.0 }
        };
        double B[3] = { data[maxIndex - 1].elevation, data[maxIndex].elevation, data[maxIndex + 1].elevation };
        double invA[3][3];
        if (invertMatrix(A, invA)) {
            double coeff[3];
            multiplyMatrixVector(invA, B, coeff);
            double a = coeff[0], b = coeff[1], c = coeff[2];
            double t_culm = -b / (2 * a);
            // Вычисляем уточнённую высоту кульминации (можно не выводить, если важен только момент)
            double culmElevation = a * t_culm * t_culm + b * t_culm + c;
            hoursToHMS(t_culm, culminationTime);
        }
    }


    cout << "\nДата: " << inputDate << "\n";

    cout << "Восход Луны: ";
    if (moonriseFound)
        cout <<
        moonriseTime[0] << moonriseTime[1] << ":" <<
        moonriseTime[2] << moonriseTime[3] << ":" <<
        moonriseTime[4] << moonriseTime[5] << "\n";
    else
        cout << "не обнаружен\n";

    cout << "Кульминация Луны: " <<
        culminationTime[0] << culminationTime[1] << ":" <<
        culminationTime[2] << culminationTime[3] << ":" <<
        culminationTime[4] << culminationTime[5] << "\n";

    cout << "Заход Луны: ";
    if (moonsetFound)
        cout <<
        moonsetTime[0] << moonsetTime[1] << ":" <<
        moonsetTime[2] << moonsetTime[3] << ":" <<
        moonsetTime[4] << moonsetTime[5] << "\n";
    else
        cout << "не обнаружен\n";

    delete[] data;
    return 0;
}