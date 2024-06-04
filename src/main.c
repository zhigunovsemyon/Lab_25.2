#include "queue.h" /*<stdio.h> <malloc.h> <stdint.h>*/

/* Дан файл из целых чисел.Используя очередь,
за один просмотр файла напечатать сначала все отрицательные числа,
затем все положительные числа, сохраняя исходный порядок в каждой группе 
EXTRA: Отсортировать эти числа*/
int main(int argc, const char *args[])
{
	if (argc < 2)
	{
		puts("Не было передано имя файла!");
		return 1;
	}

	FILE *source = fopen(args[1], "rt");
	if (!source)
	{
		perror(args[1]);
		return 1;
	}

	queue *q_positive = QueueInit(), *q_negative = QueueInit();
	if (!(q_negative && q_positive))
	{
		perror("Queue Init");
		fclose(source);
		return 2;
	}

	for (int8_t errCode = 0, buf;		   // Буфер
		 fscanf(source, "%hhd", &buf) > 0; // Чтение из файла в буфер
		 errCode = QueuePushToEnd(
			 buf, (buf < 0) ? // Добавление из буфера в очередь
					  q_negative // Отрицательных чисел
							: q_positive)) // Неотрицательных чисел
		if (errCode)
		{
			QueueFree(&q_positive);
			QueueFree(&q_negative);
			perror("queue push");
			fclose(source);
			return 2;
		};

	fclose(source);

	QueueSort(q_positive, QueueDecreaseCmp);
	QueueSort(q_negative, QueueDecreaseCmp);

	puts("Очередь неотрицательных чисел:");
	for (int8_t buf;						   // Буфер
		 !QueuePullFromHead(&buf, q_positive); // Чтение из очереди в буфер
		 printf("%hhd ", buf)) // Вывод из буфера на экран
		;
	putchar('\n');

	puts("Очередь отрицательных чисел:");
	for (int8_t buf;						   // Буфер
		 !QueuePullFromHead(&buf, q_negative); // Чтение из очереди в буфер
		 printf("%hhd ", buf)) // Вывод из буфера на экран
		;
	putchar('\n');

	QueueFree(&q_positive);
	QueueFree(&q_negative);
	return 0;
}
