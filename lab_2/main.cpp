/*
 * Example
 *
 * Input data:
 * 1 4 5 6 2 3 4 2^Z
 *
 * Output:
 * median index: 3
 */

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

// Класс, используемый для сигнализации ошибки при проведении юнит-тестов
class unit_tests_exception : public std::exception
{
public:
	unit_tests_exception(const std::string& msg) : _msg("Unit tests exception: " + msg) {}

	/* virtual */ const char* what() const override
	{
		return _msg.c_str();
	}

private:
	const std::string _msg;
};

// В качестве массива в данном случае используется std::vector<int>
typedef std::vector<int> arr_t;

// Структура, содержащая информацию о сумме левой и правой части массива (относительно некоторого индекса)
struct sums
{
	int left_part;
	int right_part;
};

// Функция, возвращающая информацию о сумме левой и правой части массива
// Принимает в качестве аргументов сам массив,
// его размер (учитывая, что здесь используется std::vector, можно было бы и не передавать, но для унификации пусть будет)
// и текущий индекс, относительно которого мы и будем вычислять суммы левой и правой частей
sums get_sums(
	const arr_t& arr
	, arr_t::size_type arr_size
	, arr_t::size_type middle_index
)
{
	sums arr_sums;

	int left_part_sum = 0;
	for (int i = 0; i < middle_index; ++i)
	{
		left_part_sum += arr[i];
	}
	arr_sums.left_part = left_part_sum;

	int right_part_sum = 0;
	for (int i = middle_index + 1; i < arr_size; ++i)
	{
		right_part_sum += arr[i];
	}
	arr_sums.right_part = right_part_sum;

	return arr_sums;
}

// Функция, возвращающая индекс медианы заданного массива
// Принимает в качестве аргумента сам массив
arr_t::size_type get_median_index(const arr_t& arr)
{
	// В качестве значений для инициализации переменных возьмём максиммально возможные значения для данных типов
	int median_sums_diff = std::numeric_limits<int>::max();
	arr_t::size_type median_index = std::numeric_limits<arr_t::size_type>::max();
	for (
		arr_t::size_type i = 0, arr_size = arr.size();
		i < arr_size;
		++i
	)
	{
		const sums cur_sums = get_sums(arr, arr_size, i); // Получаем суммы левой и правой части отноесительно текущего индекса
		int sums_diff = std::abs(cur_sums.left_part - cur_sums.right_part); // Узнаём разницу между ними
		if (sums_diff < median_sums_diff) // Проверяем, меньше ли она ранее найденной минимальной разницы
		{
			// Если меньше, то запоминаем разницу и индекс
			median_sums_diff = sums_diff;
			median_index = i;
		}
	}

	return median_index;
}

// Функция, запускающая на выполнение юнит-тесты
// Ничего не принимает в качестве аргумента и не возвращает никакого значения
// Об ошибке в проведении юнит-тестирования сообщает выбрасыванием исключения в виде объекта класса unit_tests_exception
// Хотелось бы использовать для юнит-тестов что-нибудь наподобие boost test,
// но, если я правильно понимаю, по условиям задачи подобные вещи не разрешены
// В юнит-тестах, само собой разумеется, пришлось использовать магические константы, заводить для каждой из ни отдельную переменную - это, на мой взгляд, глупо
void run_unit_tests()
{
	// Хотелось бы использовать что-то наподобие boost::asign::list_of или initializer lists из C++11, но не знаю, разрешено ли это

	std::cout << "Trying to run 1st unit test... ";
	// 1 4 5 6 2 3 4 2
	// Индекс (начиная с 0) - сумма левой части / сумма правой части
	// 1 - 1 / 22
	// 2 - 5 / 17
	// 3 - 10 / 11 - медиана массива
	// 4 - 16 / 9
	// 5 - 18 / 6
	// 6 - 21 / 2
	std::vector<int> arr_1;
	arr_1.push_back(1);
	arr_1.push_back(4);
	arr_1.push_back(5);
	arr_1.push_back(6);
	arr_1.push_back(2);
	arr_1.push_back(3);
	arr_1.push_back(4);
	arr_1.push_back(2);
	if (get_median_index(arr_1) != 3)
	{
		throw unit_tests_exception("An error occurred while running 1st unit test");
	}
	std::cout << "Success \n";

	std::cout << "Trying to run 2nd unit test... ";
	// 5 4 1 1 2 3 4 3
	// Индекс (начиная с 0) - сумма левой части / сумма правой части
	// 1 - 5 / 14
	// 2 - 9 / 13
	// 3 - 10 / 12
	// 4 - 11 / 10 - медиана массива
	// 5 - 13 / 7
	// 6 - 16 / 3
	std::vector<int> arr_2;
	arr_2.push_back(5);
	arr_2.push_back(4);
	arr_2.push_back(1);
	arr_2.push_back(1);
	arr_2.push_back(2);
	arr_2.push_back(3);
	arr_2.push_back(4);
	arr_2.push_back(3);
	if (get_median_index(arr_2) != 4)
	{
		throw unit_tests_exception("An error occurred while running 2nd unit test");
	}
	std::cout << "Success \n";

	std::cout << "Trying to run 3rd unit test... ";
	// 5 4 1 1 2 3 4
	// Индекс (начиная с 0) - сумма левой части / сумма правой части
	// 1 - 5 / 11
	// 2 - 9 / 10 - медиана массива
	// 3 - 10 / 9 - тоже медиана массива, однако алгоритм работает таким образом,
	// что первый найденный индекс с минимальной разницей сумм левой и правой частей массива и является его медианой
	// 4 - 11 / 7
	// 5 - 13 / 4
	std::vector<int> arr_3;
	arr_3.push_back(5);
	arr_3.push_back(4);
	arr_3.push_back(1);
	arr_3.push_back(1);
	arr_3.push_back(2);
	arr_3.push_back(3);
	arr_3.push_back(4);
	if (get_median_index(arr_3) != 2)
	{
		throw unit_tests_exception("An error occurred while running 3rd unit test");
	}
	std::cout << "Success \n";

	std::cout << "Trying to run 4th unit test... ";
	// 5 4 1 1 2 3 9
	// Индекс (начиная с 0) - сумма левой части / сумма правой части
	// 1 - 5 / 16
	// 2 - 9 / 15
	// 3 - 10 / 14
	// 4 - 11 / 12 - медиана массива
	// 5 - 13 / 9
	std::vector<int> arr_4;
	arr_4.push_back(5);
	arr_4.push_back(4);
	arr_4.push_back(1);
	arr_4.push_back(1);
	arr_4.push_back(2);
	arr_4.push_back(3);
	arr_4.push_back(9);
	if (get_median_index(arr_4) != 4)
	{
		throw unit_tests_exception("An error occurred while running 4th unit test");
	}
	std::cout << "Success \n";

	std::cout << "Trying to run 5th unit test... ";
	// 1 4 1 1 2 3 9
	// Индекс (начиная с 0) - сумма левой части / сумма правой части
	// 1 - 1 / 16
	// 2 - 5 / 15
	// 3 - 6 / 14
	// 4 - 7 / 12
	// 5 - 9 / 9 - медиана массива
	std::vector<int> arr_5;
	arr_5.push_back(1);
	arr_5.push_back(4);
	arr_5.push_back(1);
	arr_5.push_back(1);
	arr_5.push_back(2);
	arr_5.push_back(3);
	arr_5.push_back(9);
	if (get_median_index(arr_5) != 5)
	{
		throw unit_tests_exception("An error occurred while running 5th unit test");
	}
	std::cout << "Success \n";

	std::cout << "Trying to run 6th unit test... ";
	// Пустой объект класса std::vector<int>
	std::vector<int> arr_6;
	if (get_median_index(arr_6) != std::numeric_limits<arr_t::size_type>::max())
	{
		throw unit_tests_exception("An error occurred while running 6th unit test");
	}
	std::cout << "Success \n";
}

int main()
{
	std::cout << "This program finds a median of array \n"
			  << "Author: Trophimov N. A. \n\n";
	
	try
	{
		std::cout << "Trying to run unit tests... \n";
		run_unit_tests();
		std::cout << "Success \n\n";
	}
	catch (const unit_tests_exception& ex)
	{
		std::cerr << ex.what() << '\n';
		return EXIT_FAILURE;
	}

	std::cout << "Please enter the values for array separated by whitespace character \n"
			  << "Enter EOF symbol (Ctrl-Z in case of Windows) to stop \n";
	std::vector<int> arr;
	std::copy(
		std::istream_iterator<int>(std::cin)
		, std::istream_iterator<int>()
		, std::back_inserter(arr)
	);

	arr_t::size_type median_index = get_median_index(arr);
	std::cout << "median index: " << median_index << '\n';
}
