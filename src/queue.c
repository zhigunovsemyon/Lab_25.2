#include "queue.h" /*<stdio.h> <malloc.h> <stdint.h>*/

struct part
{
	int8_t num;
	struct part *next;
};

struct queue
{
	struct part *head;
	struct part *tail;
};

queue *QueueInit(void)
{
	return (queue *)calloc(1, sizeof(queue));
};

void QueueFree(queue **qptr)
{
	if (!(*qptr))
		return;
	
	if (!(*qptr)->head)
	{
		free(*qptr);
		*qptr = NULL;
		return;
	}

	struct part *tmp = (*qptr)->head;
	(*qptr)->head = (*qptr)->head->next;
	free(tmp);
	return QueueFree(qptr);
};

uint8_t QueuePushToHead(int8_t num, queue *q)
{
	struct part *buf = (struct part *)malloc(sizeof(struct part));
	if (!buf)
		return -1;

	buf->num = num;

	if (!q->head)
	{
		q->head = q->tail = buf;
		buf->next = NULL;
		return 0;
	}

	buf->next = q->head;
	q->head = buf;
	return 0;
};

uint8_t QueuePushToEnd(int8_t num, queue *q)
{
	struct part *buf = (struct part *)malloc(sizeof(struct part));
	if (!buf)
		return -1;

	buf->num = num;
	buf->next = NULL;

	if (!q->tail)
	{
		q->head = q->tail = buf;
		buf->next = NULL;
		return 0;
	}

	q->tail->next = buf;
	q->tail = buf;
	return 0;
};

uint8_t QueuePullFromHead(int8_t *ret, queue *q)
{
	if (!q->head)
		return 1;

	*ret = q->head->num;
	struct part *tmp = q->head;
	if(!(q->head = q->head->next))
		q->tail = NULL;
	free(tmp);
	return 0;
};

uint8_t QueuePullFromEnd(int8_t *ret, queue *q)
{
	if (!q->tail)
		return 1;

	*ret = q->tail->num;
	struct part *pre_tail = q->head;
	if (!pre_tail->next)
	{
		free(pre_tail);
		q->head = q->tail = NULL;
		return 0;
	}

	while (pre_tail->next->next)
		pre_tail = pre_tail->next;

	free(pre_tail->next);
	pre_tail->next = NULL;
	q->tail = pre_tail;
	return 0;
};

int16_t QueueIncreaseCmp(int8_t arg1, int8_t arg2)
{
	// Если arg1 > arg2 == 1 - 0, возврат 1
	// Если arg1 < arg2 == 0 - 1, возврат -1
	// Если arg1 = arg2 == 0 - 0, возврат 0
	return (arg1 > arg2) - (arg1 < arg2);
}

int16_t QueueDecreaseCmp(int8_t arg1, int8_t arg2)
{
	// Если arg1 < arg2 == 1 - 0, возврат 1
	// Если arg1 > arg2 == 0 - 1, возврат -1
	// Если arg1 = arg2 == 0 - 0, возврат 0
	return (arg1 < arg2) - (arg1 > arg2);
}

static void SwapValue(int8_t *a, int8_t *b)
{
	int8_t tmp = *a;
	*a = *b;
	*b = tmp;
}

uint8_t QueueSort(queue *q, int16_t(cmp)(int8_t, int8_t))
{	//Создание левого и правого плеча сортировки
	queue *left = QueueInit();
	queue *right = QueueInit();
	if (!(left && right))
	{
		perror("QueueInit");
		return 1;
	}

	//Извлечение пивота
	struct part *pivot = q->head;
	if (!(q->head = q->head->next))
		q->tail = NULL;
	pivot->next = NULL;

	//Цикл продолжается, пока не закончится переданная очередь
	for (struct part *pulld; q->head;)
	{	//Извлечение проверяемого элемента
		pulld = q->head;
		if (!(q->head = q->head->next))
			q->tail = NULL;
		pulld->next = NULL;

		//Выбор левого, либо правого плеча
		queue *chosen;
		chosen = (cmp(pivot->num, pulld->num) > 0) ? left : right;

		//Если очередь пустая, элемент записывается и в хвост, и в голову
		if (!chosen->tail)
		{
			chosen->head = chosen->tail = pulld;
			continue;
		}
		//Запись элемента в указателю хвоста нужной очереди, обновление хвоста
		chosen->tail->next = pulld;
		chosen->tail = chosen->tail->next;
	}

	//Вставка пивота в левое плечо, либо в хвост, либо и хвост, и голову
	if (left->tail)
		left->tail->next = pivot;
	else
		left->head = left->tail = pivot;
	
	//Если левое плечо содержит элементы, помимо пивота, нужно его отсортировать
	if (left->head->next)
		if (QueueSort(left, cmp))
		{	//Освобождение плечей при падении сортировщика
			QueueFree(&left);
			QueueFree(&right);
			return 1;
		}

	// Если правое плечо содержит >1 элемента, нужно его отсортировать
	if (right->head)
		if (right->head->next)
			if(QueueSort(right, cmp))
			{	//Освобождение плечей при падении сортировщика
				QueueFree(&left);
				QueueFree(&right);
				return 1;
			}

	//Конкатинация левого и правого плеча
	left->tail->next = right->head;

	//Перезапись начала левого плеча в исходную очередь
	q->head = left->head;

	//Перезапись конца правого плеча, если оно не пустое, либо конца левого плеча
	q->tail = ((right->tail) ? right : left)->tail;

	//Зануление левого и правого плеча, освобождение
	left->head = left->tail = right->head = right->tail = NULL;
	QueueFree(&left);
	QueueFree(&right);
	return 0;
}
