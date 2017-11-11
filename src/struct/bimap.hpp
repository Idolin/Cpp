/*template<typename T1, typename T2>
struct bimap
{
    struct node
    {
        T1 left_data;
        node* left_left;
        node* left_right;
        node* left_parent;

        T2 right_data;
        node* right_left;
        node* right_right;
        node* right_parent;
    };

    struct left_iterator;
    struct right_iterator;

    // Создает bimap не содержащий ни одной пары.
    bimap();

    // Деструктор. Вызывается при удалении объектов bimap.
    // Инвалидирует все итераторы ссылающиеся на элементы этого bimap
    // (включая итераторы ссылающиеся на элементы следующие за последними).
    ~bimap();

    // Вставка пары (left, right), возвращает итератор на left.
    // Если такой left или такой right уже присутствуют в bimap, вставка не
    // производится и возвращается end_left().
    left_iterator insert(left_t const& left, right_t const& right);

    // Удаляет элемент и соответствующий ему парный.
    // erase невалидного итератора неопределен.
    // erase(end_left()) и erase(end_right()) неопределены.
    // Пусть it ссылается на некоторый элемент e.
    // erase инвалидирует все итераторы ссылающиеся на e и на элемент парный к e.
    void erase(left_iterator it);
    void erase(right_iterator it);

    // Возвращает итератор по элементу. В случае если элемент не найден, возвращает
    // end_left()/end_right() соответственно.
    left_iterator  find_left(left_t const& left) const
    right_iterator find_right(right_t const& right) const

    // Возващает итератор на минимальный по величине left.
    left_iterator begin_left() const
    // Возващает итератор на следующий за последним по величине left.
    left_iterator end_left() const

    // Возващает итератор на минимальный по величине right.
    right_iterator begin_right() const
    // Возващает итератор на следующий за последним по величине right.
    right_iterator end_right() const
};

struct bimap::left_iterator
{
    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end_left() неопределено.
    // Разыменование невалидного итератора неопределено.
    left_t const& operator*() const

    // Переход к следующему по величине left'у.
    // Инкремент итератора end_left() неопределен.
    // Инкремент невалидного итератора неопределен.
    left_iterator& operator++();
    left_iterator operator++(int);

    // Переход к предыдущему по величине left'у.
    // Декремент итератора begin_left() неопределен.
    // Декремент невалидного итератора неопределен.
    left_iterator& operator--();
    left_iterator operator--(int);

    // left_iterator ссылается на левый элемент некоторой пары.
    // Эта функция возвращает итератор на правый элемент той же пары.
    // end_left().flip() возращает end_right().
    // end_right().flip() возвращает end_left().
    // flip() невалидного итератора неопределен.
    right_iterator flip() const
};

struct bimap::right_iterator
{
    // Здесь всё аналогично left_iterator.
    right_t const& operator*() const

    right_iterator& operator++();
    right_iterator operator++(int);

    right_iterator& operator--();
    right_iterator operator--(int);

    left_iterator flip() const
};
*/