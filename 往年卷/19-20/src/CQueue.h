//your code here
template <class T>
class CQueue {
public:
	CQueue() : queueSize(10), head(0), rear(0) {
		data_buff = new T[queueSize];
	}

	CQueue(int size) : queueSize(size), head(0), rear(0) {
		data_buff = new T[queueSize];
	}

	~CQueue() {
		delete[] data_buff;
	}

	int getSize() const {
		return queueSize;
	}

	int getNumbers() const {
		return (rear - head + queueSize) % queueSize;
	}

	T getHead() const {
		if (isEmpty()) throw underflow_error("The queue is empty!");
		return data_buff[head];
	}

	void enQueue(T data) {
		if (isFull()) throw overflow_error("The queue is full!");
		data_buff[rear] = data;
		rear = (rear + 1) % queueSize;
	}

	T deQueue() {
		if (isEmpty()) throw underflow_error("The queue is empty!");
		T temp = data_buff[head];
		head = (head + 1) % queueSize;
		return temp;
	}

	bool isEmpty() const {
		return head == rear;
	}

	bool isFull() const {
		return (rear + 1) % queueSize == head;
	}

	void show() const {
		if (isEmpty()) throw underflow_error("The queue is empty!");
		int i = head;
		while (i != rear) {
			cout << data_buff[i] << " ";
			i = (i + 1) % queueSize;
		}
		cout << endl;
	}

private:
	const int queueSize;
	int head;
	int rear;
	T* data_buff;
};